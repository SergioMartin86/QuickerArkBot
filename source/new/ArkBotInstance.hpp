#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/base.hpp>
#include "../ArkBotInstanceBase.hpp"
#include <core/emu.hpp>
#include <hqn.h>
#include <hqn_gui_controller.h>
#include "core/source/GameOperations.h"
#include "core/source/GameData.h"

typedef quickerNES::Emu emulator_t;

namespace ark
{

class EmuInstance : public EmuInstanceBase
{
 public:

  EmuInstance(const uint8_t level, const unsigned int initialScore, const bool useVerification) : EmuInstanceBase(level, initialScore, useVerification)
  {
  }

  ~EmuInstance()
  {
  }

  virtual void initializeImpl(const std::string& romFilePath) override
  {
    // Only load quickerNES if verification is needed
    if (_useVerification)
    {
      std::string romData;
      jaffarCommon::file::loadStringFromFile(romData, romFilePath);

      // Loading rom data
      _nes.load_ines((uint8_t*)romData.data());

      // Allocating video buffer
      _video_buffer = (uint8_t *)malloc(getBlitSize());
    }

    // Initializing Arkbot
    _arkEngine.Init(_arkState);

    // Setting current level
    _arkEngine.AdvanceToLevel(_arkState, _initialLevel);

    // Setting initial score
    _arkState.score = _initialScore;
  }

  void initializeVideoOutput() override
  {
    _window = launchOutputWindow();

    // Setting video buffer
    _nes.set_pixels(_video_buffer, image_width + 8);

    // Loading Emulator instance HQN
    _hqnState.setEmulatorPointer(&_nes);
    _hqnState.m_emu->set_pixels(_video_buffer, image_width + 8);

    // Enabling emulation rendering
    enableRendering();

    // Creating HQN GUI
    _hqnGUI = hqn::GUIController::create(_hqnState, _window);
    _hqnGUI->setScale(1);
  }

  void finalizeVideoOutput() override
  {
    free(_video_buffer);
  }

  void enableRendering() override
  {
    _doRendering = true;
  }

  void disableRendering() override
  {
    _doRendering = false; 
  }

  void serializeState(jaffarCommon::serializer::Base& serializer) const override
  {
   if (_useVerification) _nes.serializeState(serializer);
   serializer.push(&_arkState, sizeof(_arkState));
  }

  void deserializeState(jaffarCommon::deserializer::Base& deserializer) override
  {
   if (_useVerification)_nes.deserializeState(deserializer);
   deserializer.pop(&_arkState, sizeof(_arkState));
  }

  size_t getStateSizeImpl() const override
  {
    jaffarCommon::serializer::Contiguous serializer;
    serializeState(serializer);
    return serializer.getOutputSize();
  }

  void updateRenderer() override
  {
    _hqnGUI->update_blit(_curBlit, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  }

  inline size_t getDifferentialStateSizeImpl() const override { return 0; }

  void enableStateBlockImpl(const std::string& block) override { _nes.enableStateBlock(block); };
  void disableStateBlockImpl(const std::string& block) override { _nes.disableStateBlock(block); };

  inline jaffarCommon::hash::hash_t getStateHash() const 
  {
    MetroHash128 hash;

    hash.Update(_arkState.ball[0].pos);
    hash.Update(_arkState.ball[0].vel);
    hash.Update(_arkState.ball[0].vSign);
    hash.Update(_arkState.ball[0].angle);
    hash.Update(_arkState.ball[0].cycle);
    hash.Update(_arkState.ball[0].exists);
    hash.Update(_arkState.ball[0].xCollis);
    hash.Update(_arkState.ball[0].yCollis);
    hash.Update(_arkState.ball[0].speedMult);
    hash.Update(_arkState.ball[0].speedStage);
    hash.Update(_arkState.ball[0].speedStageM);
    hash.Update(_arkState.ball[0].speedRowIdx);
    hash.Update(_arkState.ball[0]._paddleCollis);

    hash.Update(_arkState.ball[1].pos);
    hash.Update(_arkState.ball[1].vel);
    hash.Update(_arkState.ball[1].vSign);
    hash.Update(_arkState.ball[1].angle);
    hash.Update(_arkState.ball[1].cycle);
    hash.Update(_arkState.ball[1].exists);
    hash.Update(_arkState.ball[1].xCollis);
    hash.Update(_arkState.ball[1].yCollis);
    hash.Update(_arkState.ball[1].speedMult);
    hash.Update(_arkState.ball[1].speedStage);
    hash.Update(_arkState.ball[1].speedStageM);
    hash.Update(_arkState.ball[1].speedRowIdx);
    hash.Update(_arkState.ball[1]._paddleCollis);

    hash.Update(_arkState.ball[2].pos);
    hash.Update(_arkState.ball[2].vel);
    hash.Update(_arkState.ball[2].vSign);
    hash.Update(_arkState.ball[2].angle);
    hash.Update(_arkState.ball[2].cycle);
    hash.Update(_arkState.ball[2].exists);
    hash.Update(_arkState.ball[2].xCollis);
    hash.Update(_arkState.ball[2].yCollis);
    hash.Update(_arkState.ball[2].speedMult);
    hash.Update(_arkState.ball[2].speedStage);
    hash.Update(_arkState.ball[2].speedStageM);
    hash.Update(_arkState.ball[2].speedRowIdx);
    hash.Update(_arkState.ball[2]._paddleCollis);

    hash.Update(_arkState.score);
    hash.Update(_arkState.pendingScore);
    hash.Update(_arkState.blocks);
 
    jaffarCommon::hash::hash_t result;
    hash.Finalize(reinterpret_cast<uint8_t *>(&result));
    return result;
  }

  void doSoftReset() override
  {
    _arkEngine.AdvanceToLevel(_arkState, _initialLevel);
    _arkState.score = _initialScore;
  }

  void doHardReset() override
  {
    _arkEngine.AdvanceToLevel(_arkState, _initialLevel);
    _arkState.score = _initialScore;
  }

  std::string getCoreName() const override { return "QuickerArkBot"; }

  uint8_t* getRamPointer() const override { return nullptr; }

  void advanceStateImpl(const ark::Controller::port_t controller1, const ark::Controller::port_t controller2) override
  {
    // Advancing arkbot
    Input input = 0;
    if (controller1 & 0b01000000) input |= LeftInput;
    if (controller1 & 0b10000000) input |= RightInput;
    if (controller1 & 0b00000001) input |= AInput;
    _arkEngine.AdvanceFrame(_arkState, input);

    if (_useVerification == false) return; 

    // Advancing quickernes at the same time
    if (_doRendering == true) 
    {
      _nes.emulate_frame(controller1, controller2);
      saveBlit(&_nes, _curBlit, hqn::HQNState::NES_VIDEO_PALETTE, 0, 0, 0, 0);
    }

    if (_doRendering == false) _nes.emulate_skip_frame(controller1, controller2);
  }

  void printInformation() const override
  {
     jaffarCommon::logger::log("[] Arkbot   Paddle X: %u\n", _arkState.paddleX);
     if (_useVerification == true) jaffarCommon::logger::log("[] QuickNES Paddle X: %u\n", _nes.get_low_mem()[0x011A]);

     jaffarCommon::logger::log("[] Arkbot   Ball 1 Pos: (%u, %u)\n", _arkState.ball[0].pos.x, _arkState.ball[0].pos.y);
     if (_useVerification == true) jaffarCommon::logger::log("[] QuickNES Ball 1 Pos: (%u, %u)\n", _nes.get_low_mem()[0x0038], _nes.get_low_mem()[0x0037]);

     jaffarCommon::logger::log("[] Arkbot   Ball 2 Pos: (%u, %u)\n", _arkState.ball[1].pos.x, _arkState.ball[1].pos.y);
     if (_useVerification == true) jaffarCommon::logger::log("[] QuickNES Ball 2 Pos: (%u, %u)\n", _nes.get_low_mem()[0x0052], _nes.get_low_mem()[0x0051]);

     jaffarCommon::logger::log("[] Arkbot   Ball 3 Pos: (%u, %u)\n", _arkState.ball[2].pos.x, _arkState.ball[2].pos.y);
     if (_useVerification == true) jaffarCommon::logger::log("[] QuickNES Ball 3 Pos: (%u, %u)\n", _nes.get_low_mem()[0x006C], _nes.get_low_mem()[0x006B]);

     jaffarCommon::logger::log("[] Arkbot Score:   %u\n", _arkState.score);
      
     if (_useVerification == true)
     {
       unsigned int quickerNESScore = _nes.get_low_mem()[0x00370] * 100000 + 
                                      _nes.get_low_mem()[0x00371] * 10000 +   
                                      _nes.get_low_mem()[0x00372] * 1000 +
                                      _nes.get_low_mem()[0x00373] * 100 +
                                      _nes.get_low_mem()[0x00374] * 10;

       jaffarCommon::logger::log("[] QuickNES Score: %u\n", quickerNESScore);
     }

     jaffarCommon::logger::log("[] Arkbot Pending Score:   %u\n", _arkState.pendingScore);

     if (_useVerification == true)
     {
       unsigned int quickerNESPendingScore = _nes.get_low_mem()[0x0037C] * 100000 + 
                                             _nes.get_low_mem()[0x0037D] * 10000 +   
                                             _nes.get_low_mem()[0x0037E] * 1000 +
                                             _nes.get_low_mem()[0x0037F] * 100 +
                                             _nes.get_low_mem()[0x00380] * 10;

       jaffarCommon::logger::log("[] QuickNES Pending Score: %u\n", quickerNESPendingScore);
     }

     jaffarCommon::logger::log("[] Arkbot Block State:\n");
     for (uint8_t i = 0; i < GameConsts::BlocksPerCol; i++)
     {
      jaffarCommon::logger::log("[] ");
      for (uint8_t j = 0; j < GameConsts::BlocksPerRow; j++)
       if (_arkState.blocks[i*GameConsts::BlocksPerRow + j] == 0) 
         jaffarCommon::logger::log("---- ");
       else
         jaffarCommon::logger::log("0x%02X ", _arkState.blocks[i*GameConsts::BlocksPerRow + j]);
      jaffarCommon::logger::log("\n");
     }
  }

  private:

  SDL_Window *launchOutputWindow()
{
  // Opening rendering window
  SDL_SetMainReady();

  // We can only call SDL_InitSubSystem once
  if (!SDL_WasInit(SDL_INIT_VIDEO))
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) JAFFAR_THROW_LOGIC("Failed to initialize video: %s", SDL_GetError());

  auto window = SDL_CreateWindow("JaffarPlus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 100, 100, SDL_WINDOW_RESIZABLE);
  if (window == nullptr) JAFFAR_THROW_LOGIC("Coult not open SDL window");

  return window;
}

  // Game state from Arkbot
  GameState _arkState;

  // Game operations class from Arkbot
  GameOp _arkEngine;

  // Flag to determine whether to enable/disable rendering
  bool _doRendering = true;

  // Base Emulator instance
  emulator_t _nes;

  // Storage for the HQN state
  hqn::HQNState _hqnState;

  // Storage for the HQN GUI controller
  hqn::GUIController *_hqnGUI;

  // Flag to store whether to use the button overlay
  bool _useOverlay = false;

  // SDL Window
  SDL_Window* _window;
};

} // namespace ark