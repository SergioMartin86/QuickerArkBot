project('quickerArkBot','c','cpp',
  version: '1.0.0',
  license: 'GPL-3.0-only',
  default_options : ['cpp_std=c++17', 'default_library=shared', 'buildtype=release'],
  subproject_dir : 'extern'
)

# Loading dependencies
subdir('source')

# Do not build any targets if this is a subproject
if meson.is_subproject() == false

# Common application flags
commonCompileArgs = [ '-Wall', '-Wfatal-errors' ]

# Grabbing jaffarCommon dependency

jaffarCommonSubproject = subproject('jaffarCommon')
jaffarCommonDependency = jaffarCommonSubproject.get_variable('jaffarCommonDependency')

# Building playback tool

if get_option('buildPlayer') == true

  quickerArkBotPlayer = executable('quickerArkBotPlayer',
    'source/player.cpp',
    cpp_args            : [ commonCompileArgs, '-DNCURSES', '-D_JAFFAR_PLAYER' ],
    dependencies        : [ quickerArkBotDependency,
                            jaffarCommonDependency
                          ],
    include_directories : include_directories(['source']),
    link_args           : [ '-lncurses' ],
  )

  baseArkBotPlayer = executable('baseArkBotPlayer',
    'source/player.cpp',
    cpp_args            : [ commonCompileArgs, '-DNCURSES' ],
    dependencies        : [ baseArkBotDependency,
                            jaffarCommonDependency,
                          ],
    include_directories : include_directories(['source']),
    link_args           : [ '-lncurses' ],
  )

endif

# Building tester tool for QuickerArkBot

quickerArkBotTester = executable('quickerArkBotTester',
  'source/tester.cpp',
  cpp_args            : [ commonCompileArgs ], 
  dependencies        : [ quickerArkBotDependency, jaffarCommonDependency ],
)

# Building tester tool for the original ArkBot

baseArkBotTester = executable('baseArkBotTester',
  'source/tester.cpp',
  cpp_args            : [ commonCompileArgs ],
  dependencies        : [ baseArkBotDependency, jaffarCommonDependency ],
)

# Building tests
subdir('tests')

endif # If not subproject
