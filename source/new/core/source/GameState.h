#pragma once

#include <GameConsts.h>
#include <cstdint>
#include <functional>
#include <set>

struct Point
{
  uint8_t x = 0;
  uint8_t y = 0;
};

struct Vel
{
  int8_t vx = 0;
  int8_t vy = 0;
};

struct SpeedTableRow
{
  uint8_t cycleLen       = 0;
  uint8_t speedMult      = 0;
  uint8_t speedReduction = 0;
  uint8_t _unused        = 0;

  uint8_t vel[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

struct Ball
{
  Point pos = {0x77, 0x77};

  Vel vel   = {0x77, 0x77};
  Vel vSign = {0x77, 0x77};

  Angle angle = Angle::Invalid;

  uint8_t cycle = 0x77;

  bool exists = true;

  bool xCollis = true;
  bool yCollis = true;

  // This is part of the ball data since it varies based on the angle.
  uint8_t speedMult = 0x77;

  uint8_t speedStage  = 0x77;
  uint8_t speedStageM = 0x77;

  SpeedTableRow *speedRow    = nullptr;
  uint8_t        speedRowIdx = 0;

  // The game doesn't track this but it's useful for the AI.
  bool _paddleCollis = false;
};

typedef uint8_t Block;

struct Proximity
{
  bool thisBlock    = false;
  bool oneRight     = false;
  bool oneDown      = false;
  bool oneDownRight = false;
};

struct Enemy
{
  bool         active       = true;
  bool         exiting      = true;
  uint8_t      destrFrame   = 0x77;
  Point        pos          = {0x77, 0x77};
  MovementType movementType = MovementType::Circling;
  uint8_t      moveTimer    = 0x77;
  uint8_t      moveDir      = 0x77;
  uint8_t      descentTimer = 0x77;
  uint8_t      animTimer    = 0x77;
  uint8_t      circleStage  = 0x77;
  CircleHalf   circleHalf   = CircleHalf::Top;
  CircleDir    circleDir    = CircleDir::Counterclockwise;

  uint8_t _id = 0x77;
};

// Special flags to control simulation behavior.
// TODO use templates instead

constexpr uint8_t _Level            = 1;
constexpr bool    _CompileTimeLevel = false;

// Do extra updates and checks done by the original game engine, that aren't
// strictly necessary for the evaluation.
constexpr bool _Pedantic = false;

constexpr bool _EnableDebug   = false;
constexpr bool _PrintSolution = false;

constexpr bool _Debug() { return _EnableDebug; }

constexpr bool EnemySimTable[37] = {true, false, true, true, true, true,  true, true, true, true,  true, true, false, false, true, false, true, true, true,
                                    true, true,  true, true, true, false, true, true, true, false, true, true, true,  false, true, false, true, false};

constexpr bool GoldBlockTable[37] = {true, false, false, true, false, false, true, false, true,  true, true, false, true,  false, true,  false, true, true, true,
                                     true, true,  true,  true, false, false, true, true,  false, true, true, true,  false, true,  false, true,  true, false};

constexpr bool SimulateScoreTable[37] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true,
                                         true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false};

struct GameState
{
  // Overall operational state.
  OperationalState opState = OperationalState::Invalid;

  // Input data.
  uint8_t potPosX   = 0x00;
  bool    pressFire = false;

  // Paddle values.
  uint8_t paddleX      = 0x77;
  bool    paddleCollis = true;

  // Ball data.
  Ball ball[3];

  // Block data.
  Block     blocks[GameConsts::BlockTableSize - 22];
  uint8_t   totalBlocks         = 0x77;
  uint8_t   currentBlocks       = 0x77;
  uint8_t   blockCollisCount    = 0x77;
  Proximity blockCollisSide     = {true, true, true, true};
  Block     justHitBlock[3]     = {0x77, 0x77, 0x77};
  Point     justHitBlockCell[3] = {{0x77, 0x77}, {0x77, 0x77}, {0x77, 0x77}};
  bool      justDestrBlock[3]   = {true, true, true};
  Point     calculatedCell      = {0x77, 0x77};

  // Powerup data.
  Powerup spawnedPowerup     = Powerup::Invalid;
  Powerup ownedPowerup       = Powerup::Invalid;
  Point   powerupPos         = {0x77, 0x77};
  bool    justSpawnedPowerup = true;

  // Score data.
  uint32_t score        = 0x77;
  uint32_t pendingScore = 0x77;

  // Enemy data.
  Enemy    enemies[3];
  uint16_t enemySpawnTimers[3] = {0x77, 0x77, 0x77};
  uint8_t  enemySpawnIndex     = 0x77;
  uint8_t  enemyGateState      = 0x77;
  uint8_t  enemyGateIndex      = 0x77;
  uint16_t enemyGateTimer      = 0x77;

  // Other stuff.
  uint16_t overallSpeedStage  = 0x77;
  uint16_t overallSpeedStageM = 0x77;
  uint8_t  speedStageCounter  = 0x77;
  uint8_t  speedReduction     = 0x77;
  uint8_t  level              = 0x77;
  uint16_t mysteryInput       = 0x77;
  uint8_t  bossHits           = 0x77;

  // Despite triggering off a single ball at a time, its value is consumed during the same
  // ball update cycle, so it's here in the global state block rather than the block state.
  bool ceilCollis = true;

  // Simulation-specific data - not used by the actual game logic.
  int8_t  _justMovedEnemy        = -1;
  uint8_t _enemyMysteryInput     = 0x77;
  uint8_t _enemyMoveOptions      = 0;
  int8_t  _queueEnemyDestruction = -1;
  
  uint8_t _wasRNGUsed1 = 0;
  uint8_t _wasRNGUsed2 = 0;
  uint8_t _wasRNGUsed3 = 0;
  uint8_t _wasRNGUsed4 = 0;
  uint8_t _enemyGateActive = 0;
};
