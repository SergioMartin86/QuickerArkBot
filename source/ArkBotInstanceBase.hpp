#pragma once

#include <memory>
#include <jaffarCommon/hash.hpp>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/deserializers/base.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/contiguous.hpp>
#include "inputParser.hpp"

#define BLIT_SIZE 65536

namespace ark
{

// Size of image generated in graphics buffer
static const uint16_t image_width = 256;
static const uint16_t image_height = 240;

class EmuInstanceBase
{
  public:

  EmuInstanceBase(const uint8_t initialLevel, const unsigned int initialScore) :
    _initialLevel (initialLevel),
    _initialScore(initialScore)
     {
       _inputParser = std::make_unique<jaffar::InputParser>(nlohmann::json());
     };

  virtual ~EmuInstanceBase() = default;

  inline void advanceState(const jaffar::input_t& input)
  {
    advanceStateImpl(input);
  }

  void initialize()
  {
    initializeImpl();
    _stateSize = getStateSizeImpl();
  }

  virtual void initializeImpl() = 0;

  inline size_t getStateSize() const 
  {
    return _stateSize;
  }

  inline size_t getDifferentialStateSize() const
  {
    return _differentialStateSize;
  }

  inline jaffar::InputParser* getInputParser() const { return _inputParser.get(); }

  // Virtual functions

  virtual jaffarCommon::hash::hash_t getStateHash() const = 0;
  virtual void serializeState(jaffarCommon::serializer::Base& s) const = 0;
  virtual void deserializeState(jaffarCommon::deserializer::Base& d) = 0;

  virtual void doSoftReset() = 0;
  virtual void doHardReset() = 0;
  virtual std::string getCoreName() const = 0;

  virtual void printInformation() const = 0;
  virtual size_t getScore() const { return 0; }

  protected:

 virtual void advanceStateImpl(const jaffar::input_t& input) = 0;

  virtual void enableStateBlockImpl(const std::string& block) {};
  virtual void disableStateBlockImpl(const std::string& block) {};

  virtual size_t getStateSizeImpl() const = 0;
  virtual size_t getDifferentialStateSizeImpl() const = 0;
  
  // State size
  size_t _stateSize;

  // Initial level for Arkbot
  const uint8_t _initialLevel;

  // Initial score
  const unsigned int _initialScore;

  private:

  // Input parser instance
  std::unique_ptr<jaffar::InputParser> _inputParser;

  // Differential state size
  size_t _differentialStateSize;


};

} // namespace ark