# baseArkBot Core sources

baseArkBotSrc =  [
  'core/Arkanoid/GameData.cpp',
  'core/Arkanoid/GameOperations.cpp',
]

baseArkBotIncludeDirs = [
 '.',
 'core/Arkanoid/',
]

baseArkBotCompileArgs = [
]

# baseArkBot Core Configuration

 baseArkBotDependency = declare_dependency(
  compile_args        : [  baseArkBotCompileArgs ],
  include_directories : include_directories(baseArkBotIncludeDirs),
  sources             : [ baseArkBotSrc ],
  dependencies        : [  
                          dependency('sdl2',  required : true),
                        ]
 )