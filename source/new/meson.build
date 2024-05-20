# quickerArkBot Core sources

quickerArkBotSrc =  [
  'core/source/GameData.cpp',
  'core/source/GameOperations.cpp',
]

quickerArkBotIncludeDirs = [
 '.',
 'core/source/',
]

quickerArkBotCompileArgs = [
]

# quickerArkBot Core Configuration

 quickerArkBotDependency = declare_dependency(
  compile_args        : [  quickerArkBotCompileArgs ],
  include_directories : include_directories(quickerArkBotIncludeDirs),
  sources             : [ quickerArkBotSrc ],
  dependencies        : [  
                          quickerNESDependency,
                          hqnDependency,
                          dependency('sdl2',  required : true),
                        ]
 )