#pragma once

#include <GameState.h>

#include <vector>

typedef uint8_t Input;

static const Input NoInput     = 0;
static const Input AInput      = 0x80;
static const Input BInput      = 0x40;
static const Input SelectInput = 0x20;
static const Input StartInput  = 0x10;
static const Input UpInput     = 8;
static const Input DownInput   = 4;
static const Input LeftInput   = 2;
static const Input RightInput  = 1;
static const Input RightAInput = 0x81;
static const Input LeftAInput  = 0x82;
static const Input FutureInput = 0xf0;

extern unsigned int BlkHits(const Block block);
extern unsigned int BlkPowerup(const Block block);
extern BlockType    BlkType(const Block block);

inline bool InputA(const Input inp) { return inp & AInput; }

inline bool InputB(const Input inp) { return inp & BInput; }

inline bool InputLeft(const Input inp) { return inp & LeftInput; }

inline bool InputRight(const Input inp) { return inp & RightInput; }

inline bool InputUp(const Input inp) { return inp & UpInput; }

inline bool InputDown(const Input inp) { return inp & DownInput; }

inline bool InputStart(const Input inp) { return inp & StartInput; }

inline bool InputSelect(const Input inp) { return inp & SelectInput; }

inline unsigned int BlkHits(const Block block) { return block & 7; }

inline unsigned int BlkPowerup(const Block block) { return block & 8; }

inline BlockType BlkType(const Block block) { return static_cast<BlockType>(block & 0xf0); }

namespace Data
{
constexpr unsigned int EnemySpawnTimerTable[40][3] = {{0, 0, 0},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0, 0x0708, 0xe10},
                                                      {0, 0, 0},
                                                      {0, 0x708, 0xe10},
                                                      {0, 0x708, 0xe10},
                                                      {0, 0, 0x708},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0, 0, 0x708},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0, 0, 0},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0, 0x708, 0xe10},
                                                      {0, 0x708, 0xe10},
                                                      {0, 0, 0x708},
                                                      {0, 0, 0},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0, 0, 0},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0, 0, 0},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0, 0x708, 0xe10},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0x708, 0xe10, 0x1518},
                                                      {0xffff, 0xffff, 0xffff}};

extern std::vector<std::vector<Block>> LevelData;

constexpr unsigned int LevelBlockCount[] = {0x0,  0x42, 0x42, 0x40, 0x70, 0x43, 0x3d, 0x44, 0x07, 0x22, 0x19, 0x31, 0x08, 0x38, 0x4d, 0x84, 0x3c, 0x2f, 0x2c,
                                            0x2a, 0x14, 0x16, 0x40, 0x53, 0x35, 0x36, 0x0a, 0x42, 0x2d, 0x48, 0x37, 0x4e, 0x1a, 0x50, 0x3b, 0x34, 0x42};

constexpr unsigned int StartingSpeedStage[] = {7, 7, 7, 7, 7, 7, 7, 8, 7, 8, 6, 7, 6, 8, 7, 8, 6, 7, 7, 7, 7, 7, 8, 7, 7, 7, 8, 6, 8, 7, 7, 7, 7, 7, 7, 7, 7};

constexpr unsigned int SpeedStageThresholds[] = {0, 0xa, 0xf, 0x14, 0x1e, 0x28, 0x37, 0x50, 0x6e, 0x87, 0xa0, 0xb9, 0xd2, 0xe6, 0xf5, 0xff};

constexpr unsigned int RngTable[] = {0x57, 0x12, 0xf3, 0xbd, 0x33, 0x9c, 0x21, 0x4f, 0x61, 0xe7, 0x0f, 0xaa, 0x7e, 0x84, 0xc6, 0xde};

constexpr unsigned int GateStateSignalValues[] = {0, 0x90, 0xa0, 0x2d, 0x2d, 0, 0, 0xa0, 0x90, 0x80, 0xff};

constexpr unsigned int GateTimerDurations[] = {0, 8, 8, 8, 2, 0, 0, 8, 8, 0x10};

constexpr unsigned int BlockScoreTable[] = {
  50, // White
  60,
  70,
  80,  // Green
  90,  // Red
  100, // Orange
  110, // Pink
  120, // Blue
  0,
  0,
  0,
  0,
  0,
  0,
  0, // Gold (only used when optimizing away gold block logic)
};

constexpr unsigned int SpecialPowerupTable[] = {0x07, 0xdf, 0x3d, 0xb9, 0x1b, 0x5e};

extern std::vector<SpeedTableRow> SteepSpeedTable;
extern std::vector<SpeedTableRow> NormalSpeedTable;
extern std::vector<SpeedTableRow> ShallowSpeedTable;

constexpr unsigned int CeilingBounceSpeedStage[] = {0, 8, 8, 8, 0, 8, 8, 0, 0, 8, 7, 8, 8, 9, 7, 0, 7, 8, 8, 8, 0, 8, 0, 0, 0, 0, 0, 9, 0, 0, 8, 0, 0, 8, 8, 8, 8};

constexpr int CircleMovementTable[] = {
  2, 0,  2, -1, 2,  0,  2,  -1, 2,  0,  2,  -1, 2,  -1, 2,  -1, 2,  -1, 2,  -2, 2,  -2, 2,  -2, 2,  -3, 2,  -3, 1,  -3, 1,    -2,
  1, -3, 1, -4, -1, -3, -1, -3, -2, -3, -2, -3, -2, -3, -2, -2, -2, -2, -2, -1, -2, -1, -2, -1, -2, -1, -2, 0,  -2, -1, 0x80, 0,
};

constexpr unsigned int MinBlockRow[] = {0, 4, 2, 3, 4, 2, 3, 4, 3, 2, 0, 4, 2, 4, 3, 3, 2, 2, 3, 3, 3, 3, 3, 2, 5, 3, 4, 7, 3, 3, 4, 3, 3, 2, 3, 0, 0};

constexpr unsigned int MaxBlockRow[] = {0,  9,  12, 17, 17, 15, 13, 15, 13, 12, 16, 12, 14, 11, 16, 14, 15, 13, 12,
                                        11, 13, 13, 12, 14, 13, 13, 10, 13, 12, 12, 13, 16, 13, 15, 17, 14, 0};
} // namespace Data