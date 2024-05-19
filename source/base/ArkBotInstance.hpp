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

typedef quickerNES::Emu emulator_t;

namespace ark
{

class EmuInstance : public EmuInstanceBase
{
 public:

  EmuInstance() : EmuInstanceBase()
  {
  }

  ~EmuInstance()
  {
  }

  virtual void initializeImpl(const std::string& romFilePath) override
  {
    std::string romData;
    jaffarCommon::file::loadStringFromFile(romData, romFilePath);

    // Loading rom data
    _nes.load_ines((uint8_t*)romData.data());

    // Allocating video buffer
    _video_buffer = (uint8_t *)malloc(image_width * image_height);
  }

  void initializeVideoOutput() override
  {
    _window = launchOutputWindow();

    // Setting video buffer
    _nes.set_pixels(_video_buffer, image_width + 8);

    // Loading Emulator instance HQN
    _hqnState.setEmulatorPointer(&_nes);
    static uint8_t video_buffer[image_width * image_height];
    _hqnState.m_emu->set_pixels(video_buffer, image_width + 8);

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
   _nes.serializeState(serializer);
  }

  void deserializeState(jaffarCommon::deserializer::Base& deserializer) override
  {
   _nes.deserializeState(deserializer);
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

  void doSoftReset() override { _nes.reset(false); }
  void doHardReset() override { _nes.reset(true); }

  std::string getCoreName() const override { return "ArkBot"; }

  uint8_t* getRamPointer() const override { return nullptr; }

  void advanceStateImpl(ark::Controller controller) override
  {
    if (_doRendering == true) 
    {
      _nes.emulate_frame(0,0);
      saveBlit(&_nes, _curBlit, hqn::HQNState::NES_VIDEO_PALETTE, 0, 0, 0, 0);
    }

    if (_doRendering == false) _nes.emulate_skip_frame(0,0);
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