# baseArkBot Core sources

baseArkBotSrc =  [
]

baseArkBotIncludeDirs = [
 '.',
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