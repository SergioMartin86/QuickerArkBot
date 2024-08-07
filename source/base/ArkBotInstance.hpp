#pragma once

#include <string>
#include <vector>
#include <jaffarCommon/exceptions.hpp>
#include <jaffarCommon/file.hpp>
#include <jaffarCommon/serializers/base.hpp>
#include <jaffarCommon/serializers/contiguous.hpp>
#include <jaffarCommon/deserializers/base.hpp>
#include "../ArkBotInstanceBase.hpp"
#include "core/source/GameOperations.h"
#include "core/source/GameData.h"

namespace ark
{

class EmuInstance : public EmuInstanceBase
{
 public:

  EmuInstance(const uint8_t level, const unsigned int initialScore) : EmuInstanceBase(level, initialScore)
  {
  }

  ~EmuInstance()
  {
  }

  virtual void initializeImpl() override
  {
  }

  void serializeState(jaffarCommon::serializer::Base& serializer) const override
  {
   serializer.push(&_arkState, sizeof(_arkState));
  }

  void deserializeState(jaffarCommon::deserializer::Base& deserializer) override
  {
   deserializer.pop(&_arkState, sizeof(_arkState));
  }

  size_t getStateSizeImpl() const override
  {
    jaffarCommon::serializer::Contiguous serializer;
    serializeState(serializer);
    return serializer.getOutputSize();
  }

  inline size_t getDifferentialStateSizeImpl() const override { return 0; }

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
    hash.Update(&_arkState.blocks[11], 187);
 
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

  std::string getCoreName() const override { return "ArkBot"; }

  void advanceStateImpl(const jaffar::input_t& input) override
  {
      // Gathering arkanoid controller info
      auto fire = input.fire;
      uint8_t position = input.position;

      // Adjusting arkanoid controller position
      auto adjustedPosition = position + 2;
      if (adjustedPosition < 16) adjustedPosition = 16;
      if (adjustedPosition > 160) adjustedPosition = 160;

      // Now setting paddle position
      _arkState.paddleX = adjustedPosition;

      // If  we still have the ball, we bring it with us
      if (_arkState.opState == OperationalState::BallNotLaunched) _arkState.ball[0].pos.x = adjustedPosition + 16;
      
      // Advancing arkbot
      Input arkInput = 0;
      if (fire) arkInput |= AInput;
      _arkEngine.ExecuteInput(_arkState, arkInput);
  }

  void printInformation() const override
  {
     jaffarCommon::logger::log("[] Paddle X: %u\n", _arkState.paddleX);
     jaffarCommon::logger::log("[] Ball 1 Pos: (%u, %u)\n", _arkState.ball[0].pos.x, _arkState.ball[0].pos.y);
     jaffarCommon::logger::log("[] Ball 2 Pos: (%u, %u)\n", _arkState.ball[1].pos.x, _arkState.ball[1].pos.y);
     jaffarCommon::logger::log("[] Ball 3 Pos: (%u, %u)\n", _arkState.ball[2].pos.x, _arkState.ball[2].pos.y);
     jaffarCommon::logger::log("[] Score:   %u\n", _arkState.score);
     jaffarCommon::logger::log("[] Pending Score:   %u\n", _arkState.pendingScore);
     jaffarCommon::logger::log("[] Block State:\n");

     for (uint8_t i = 0; i < GameConsts::BlocksPerCol-3; i++)
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

  // Game state from Arkbot
  GameState _arkState;

  // Game operations class from Arkbot
  GameOp _arkEngine;
};

} // namespace ark