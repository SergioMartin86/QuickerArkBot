#pragma once

#include "../ArkBotInstanceBase.hpp"
#include <string>
#include <vector>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/base.hpp>

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

  virtual void initializeImpl(const std::string& gameDataPath) override
  {
  }

  void initializeVideoOutput() override
  {
  }

  void finalizeVideoOutput() override
  {
  }

  void enableRendering() override
  {
  }

  void disableRendering() override
  {
  }

  void serializeState(jaffarCommon::serializer::Base& s) const override
  {
  }

  void deserializeState(jaffarCommon::deserializer::Base& d) override
  {
  }

  size_t getStateSizeImpl() const override
  {
    return 0;
  }

  void updateRenderer() override
  {
  }

  inline size_t getDifferentialStateSizeImpl() const override { return 0; }

  void enableStateBlockImpl(const std::string& block) override
  {
  }

  void disableStateBlockImpl(const std::string& block) override
  {
  }

  void doSoftReset() override
  {
  }
  
  void doHardReset() override
  {
  }

  std::string getCoreName() const override { return "ArkBot"; }

  uint8_t* getRamPointer() const override { return nullptr; }

  void advanceStateImpl(const ark::Controller::port_t controller1, const ark::Controller::port_t controller2) override
  {
  }


  private:

};

} // namespace ark