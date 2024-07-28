#pragma once

#include <vector>
#include <cstdint>

namespace GameConsts
{
const uint8_t  FieldMinX          = 0x10;
const uint8_t  FieldMaxX          = 0xbc;
const uint8_t  FieldMinY          = 0x10;
const uint8_t  FieldMaxY          = 240;
const uint8_t  PaddleSpeed        = 3;
const uint8_t  PaddleTop          = 208;
const uint8_t  PaddleWidth        = 28;
const uint8_t  PaddleHeight       = 8;
const uint8_t  PaddleMin          = 16;
const uint8_t  PaddleRightEdgeMin = PaddleMin + 8 + 8 + 8 + 8;
const uint8_t  PaddleMax          = 160;
const uint8_t  PaddleStart        = 0x58;
const uint8_t  BlocksPerRow       = 11;
const uint8_t  BlocksPerCol       = 20;
const uint16_t BlockTableSize     = BlocksPerRow * BlocksPerCol;
const uint8_t  BlockWidth         = 16;
const uint8_t  BlockHeight        = 8;
const uint16_t MaxBlocks          = BlocksPerRow * 40;
const uint8_t  BossLevel          = 0x24;

/*
    Block information
    Top word = block type:
     1 = white
     4 = green
     5 = red
     6 = orange
     7 = pink
     8 = blue
     e = silver (two-hit)
     f = gold (unbreakable)

    Bottom word = block flags
     1 001
     | |||
     | number of hits to break
     |
     powerup flag
    */

const uint8_t BlockHealthNibble = 0x7;
const uint8_t BlockPowerupBit   = 0x8;
const uint8_t BlockTypeNibble   = 0xF0;
} // namespace GameConsts

enum class BlockType
{
  White  = 0x10,
  Salmon = 0x20,
  Cyan   = 0x30,
  Green  = 0x40,
  Red    = 0x50,
  Orange = 0x60,
  Pink   = 0x70,
  Blue   = 0x80,
  Silver = 0xe0,
  Gold   = 0xf0
};

enum class Powerup
{
  None        = 0,
  Slow        = 1,
  Sticky      = 2,
  LargePaddle = 3,
  Multiball   = 4,
  Laser       = 5,
  Warp        = 6,
  ExtraLife   = 7,
  Invalid     = 0xff
};

enum class Angle
{
  Steep   = 0,
  Normal  = 1,
  Shallow = 2,
  Invalid = 3
};

enum class OperationalState
{
  Init              = 0,
  ShowingTitle      = 1,
  LoadingStory      = 2,
  ShowingStory      = 3,
  GameLaunching     = 4,
  RoundIntro        = 5,
  TransitionToLevel = 6,
  LevelIntro        = 7,
  BallNotLaunched   = 8,
  BallLaunched      = 0x10,
  BallLost          = 0x18,
  Paused            = 0x20,
  Invalid           = 0xff
};

enum class MovementType
{
  Normal   = 0,
  Circling = 1,
  Downward = 2
};

enum class CircleHalf
{
  Bottom = 0,
  Top    = 1
};

enum class CircleDir
{
  Clockwise        = 0,
  Counterclockwise = 1
};
