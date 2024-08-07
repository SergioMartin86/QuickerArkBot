#pragma once

#include "ArkBotInstance.hpp"
#include <string>
#include <jaffarCommon/hash.hpp>
#include <jaffarCommon/exceptions.hpp>

#define _INVERSE_FRAME_RATE 66667

struct stepData_t
{
  std::string inputString;
  jaffar::input_t input;
  uint8_t *stateData;
  uint8_t *videoBuffer;
  jaffarCommon::hash::hash_t hash;
};

class PlaybackInstance
{
  public:

  // Initializes the playback module instance
  PlaybackInstance(ark::EmuInstance *emu, const std::vector<std::string> &sequence, const std::vector<jaffar::input_t> &decodedSequence, const std::string& cycleType) :
   _emu(emu)
  {
    // Getting full state size
    _fullStateSize = _emu->getStateSize();  

    // Allocating temporary state data 
    uint8_t* stateData = (uint8_t*)malloc(_fullStateSize);

    // Building sequence information
    for (size_t i = 0; i < sequence.size(); i++)
    {
      // Adding new step
      stepData_t step;
      step.inputString = sequence[i];
      step.input = decodedSequence[i];

      // Serializing state
      jaffarCommon::serializer::Contiguous s(stateData, _fullStateSize);
      _emu->serializeState(s);
      step.hash = _emu->getStateHash();

      // Saving step data
      step.stateData = (uint8_t *)malloc(_fullStateSize);
      memcpy(step.stateData, stateData, _fullStateSize);

      // Adding the step into the sequence
      _stepSequence.push_back(step);

      // We advance depending on cycle type
      _emu->advanceState(step.input);

      if (cycleType == "Rerecord")
      {
        jaffarCommon::deserializer::Contiguous d(stateData, _fullStateSize);
        _emu->deserializeState(d);
        _emu->advanceState(step.input);
      }
    }

    // Adding last step with no input
    stepData_t step;
    step.inputString = "<End Of Sequence>";
    step.input = jaffar::input_t();

    // Serializing state
    jaffarCommon::serializer::Contiguous s(stateData, _fullStateSize);
    _emu->serializeState(s);
    step.hash = _emu->getStateHash();

    // Saving step data
    step.stateData = (uint8_t *)malloc(_fullStateSize);
    memcpy(step.stateData, stateData, _fullStateSize);

    // Adding the step into the sequence
    _stepSequence.push_back(step);

    // Freeing memory
    free(stateData);
  }

  // Function to render frame
  void renderFrame(const size_t stepId)
  {
    // Checking the required step id does not exceed contents of the sequence
    if (stepId > _stepSequence.size()) JAFFAR_THROW_RUNTIME("[Error] Attempting to render a step larger than the step sequence");
  }

  size_t getSequenceLength() const
  {
    return _stepSequence.size();
  }

  const std::string getInputString(const size_t stepId) const
  {
    // Checking the required step id does not exceed contents of the sequence
    if (stepId > _stepSequence.size()) JAFFAR_THROW_RUNTIME("[Error] Attempting to render a step larger than the step sequence");

    // Getting step information
    const auto &step = _stepSequence[stepId];

    // Returning step input
    return step.inputString;
  }

  const uint8_t *getStateData(const size_t stepId) const
  {
    // Checking the required step id does not exceed contents of the sequence
    if (stepId > _stepSequence.size()) JAFFAR_THROW_RUNTIME("[Error] Attempting to render a step larger than the step sequence");

    // Getting step information
    const auto &step = _stepSequence[stepId];

    // Returning step input
    return step.stateData;
  }

    const uint8_t *getStateVideoBuffer(const size_t stepId) const
  {
    // Checking the required step id does not exceed contents of the sequence
    if (stepId > _stepSequence.size()) JAFFAR_THROW_RUNTIME("[Error] Attempting to render a step larger than the step sequence");

    // Getting step information
    const auto &step = _stepSequence[stepId];

    // Returning step input
    return step.videoBuffer;
  }

  const jaffarCommon::hash::hash_t getStateHash(const size_t stepId) const
  {
    // Checking the required step id does not exceed contents of the sequence
    if (stepId > _stepSequence.size()) JAFFAR_THROW_RUNTIME("[Error] Attempting to render a step larger than the step sequence");

    // Getting step information
    const auto &step = _stepSequence[stepId];

    // Returning step input
    return step.hash;
  }

  const std::string getStateInput(const size_t stepId) const
  {
    // Checking the required step id does not exceed contents of the sequence
    if (stepId > _stepSequence.size()) JAFFAR_THROW_RUNTIME("[Error] Attempting to render a step larger than the step sequence");

    // Getting step information
    const auto &step = _stepSequence[stepId];

    // Returning step input
    return step.inputString;
  }

  private:
  
  // Internal sequence information
  std::vector<stepData_t> _stepSequence;

  // Pointer to the contained emulator instance
  ark::EmuInstance *const _emu;

  // Full size of the game state
  size_t _fullStateSize;
};
