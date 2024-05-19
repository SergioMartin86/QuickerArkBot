#pragma once

#include <jaffarCommon/hash.hpp>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/deserializers/base.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/contiguous.hpp>
#include "controller.hpp"

#define BLIT_SIZE 65536

namespace ark
{

// Size of image generated in graphics buffer
static const uint16_t image_width = 256;
static const uint16_t image_height = 240;

class EmuInstanceBase
{
  public:

  EmuInstanceBase() = default;
  virtual ~EmuInstanceBase() = default;

  inline void advanceState(const std::string &move)
  {
    bool isInputValid = _controller.parseInputString(move);
    if (isInputValid == false) JAFFAR_THROW_LOGIC("Move provided cannot be parsed: '%s'\n", move.c_str());

    // Parsing power
    if (_controller.getPowerButtonState() == true) JAFFAR_THROW_LOGIC("Power button pressed, but not supported: '%s'\n", move.c_str());

    // Parsing reset
    if (_controller.getResetButtonState() == true) doSoftReset();

    // Parsing Controllers
    const auto controller1 = _controller.getController1Code();
    const auto controller2 = _controller.getController2Code();

    advanceStateImpl(controller1, controller2);
  }

  inline void setController1Type(const std::string& type)
  {
    bool isTypeRecognized = false;

    if (type == "None") { _controller.setController1Type(ark::Controller::controller_t::none); isTypeRecognized = true; }
    if (type == "Joypad") { _controller.setController1Type(ark::Controller::controller_t::joypad); isTypeRecognized = true; }

    if (isTypeRecognized == false) JAFFAR_THROW_LOGIC("Input type not recognized: '%s'\n", type.c_str());
  }

  inline void setController2Type(const std::string& type)
  {
    bool isTypeRecognized = false;

    if (type == "None") { _controller.setController2Type(ark::Controller::controller_t::none); isTypeRecognized = true; }
    if (type == "Joypad") { _controller.setController2Type(ark::Controller::controller_t::joypad); isTypeRecognized = true; }
    
    if (isTypeRecognized == false) JAFFAR_THROW_LOGIC("Input type not recognized: '%s'\n", type.c_str());
  }

  inline jaffarCommon::hash::hash_t getStateHash() const
  {
    MetroHash128 hash;
    
    // auto workRam = getRamPointer();

    jaffarCommon::hash::hash_t result;
    hash.Finalize(reinterpret_cast<uint8_t *>(&result));
    return result;
  }

  void initialize(const std::string& romFilePath)
  {
    initializeImpl(romFilePath);
    _stateSize = getStateSizeImpl();
  }

  virtual void initializeImpl(const std::string& romFilePath) = 0;
  virtual void initializeVideoOutput() = 0;
  virtual void finalizeVideoOutput() = 0;
  virtual void enableRendering() = 0;
  virtual void disableRendering() = 0;

  void enableStateBlock(const std::string& block) 
  {
    enableStateBlockImpl(block);
    _stateSize = getStateSizeImpl();
    _differentialStateSize = getDifferentialStateSizeImpl();
  }

  void disableStateBlock(const std::string& block)
  {
     disableStateBlockImpl(block);
    _stateSize = getStateSizeImpl();
    _differentialStateSize = getDifferentialStateSizeImpl();
  }

  inline size_t getStateSize() const 
  {
    return _stateSize;
  }

  inline size_t getDifferentialStateSize() const
  {
    return _differentialStateSize;
  }

  uint8_t* getVideoBuffer() const { return _video_buffer; } 
  uint8_t* getBlitPointer() const { return (uint8_t*)_curBlit; } 
  size_t getBlitSize() const { return sizeof(int32_t) * BLIT_SIZE; } 

  // Virtual functions

  virtual void updateRenderer() = 0;
  virtual void serializeState(jaffarCommon::serializer::Base& s) const = 0;
  virtual void deserializeState(jaffarCommon::deserializer::Base& d) = 0;

  virtual void doSoftReset() = 0;
  virtual void doHardReset() = 0;
  virtual std::string getCoreName() const = 0;

  protected:

 virtual void advanceStateImpl(const ark::Controller::port_t controller1, const ark::Controller::port_t controller2) = 0;

  virtual void enableStateBlockImpl(const std::string& block) {};
  virtual void disableStateBlockImpl(const std::string& block) {};

  virtual size_t getStateSizeImpl() const = 0;
  virtual size_t getDifferentialStateSizeImpl() const = 0;
  
  virtual uint8_t* getRamPointer() const = 0;
  
  // State size
  size_t _stateSize;

  // Video buffer
  uint8_t *_video_buffer;
  int32_t _curBlit[BLIT_SIZE];

  private:

  // Controller class for input parsing
  Controller _controller;

  // Differential state size
  size_t _differentialStateSize;

};

} // namespace ark