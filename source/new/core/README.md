ArkBotEngine
---------------

[![Build & Tests](https://github.com/SergioMartin86/ArkBotEngine/actions/workflows/make.yml/badge.svg)](https://github.com/SergioMartin86/ArkBotEngine/actions/workflows/make.yml)


ArkBotEngine is a fork [ArkBot](https://github.com/sbroger/arkbot) for adapting its engine for compilation on Linux. This fork removes the solver bot part and preserves the game engine for easy connection with [JaffarPlus](https://github.com/SergioMartin86/jaffarPlus)
  
Credits
---------

- ArkBot was developed by [sbroger](https://github.com/sbroger) (ChefStef), under the Unlicense License

All base code for this project was found under open source licenses, which I preserved in their corresponding files/folders. Any non-credited work is unintentional and shall be immediately rectfied.


Original README:

--------------------------

# arkbot
Brute-force solver for the NES game Arkanoid.

Rather than try to emulate the game, the core Arkanoid game engine is re-implemented in C++ so it can be simulated as fast as possible. The brute-force evaluator runs the engine with a variety of rules and settings to try and find optimal solutions to Arkanoid levels.

See the original TASVideos submission for more details: http://tasvideos.org/6347S.html

# Building and Running
This project is targeted for Windows and compiles with Visual Studio 2017. You can download a free version of the IDE here: visualstudio.microsoft.com/vs/older-downloads

If you make substantive changes you'll want to run unit tests to ensure you didn't break the simulated game engine. Currently unit tests are executable by unzipping the archive TestData/TestData.zip, compiling with the _Pedantic_ flag set in GameState.h, and then running the executable.
