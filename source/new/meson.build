# quickerArkBot Core sources

quickerArkBotSrc =  [
]

quickerArkBotIncludeDirs = [
 '.',
]

quickerArkBotCompileArgs = [
]

# quickerArkBot Core Configuration

 quickerArkBotDependency = declare_dependency(
  compile_args        : [  quickerArkBotCompileArgs ],
  include_directories : include_directories(quickerArkBotIncludeDirs),
  sources             : [ quickerArkBotSrc ],
  dependencies        : [  
                          dependency('sdl2',  required : true),
                        ]
 )