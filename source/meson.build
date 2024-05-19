# Getting quicker nes dependency
quickerNESSubproject = subproject('quickerNES', default_options : 'buildQuickNES=false')
quickerNESDependency = quickerNESSubproject.get_variable('quickerNESDependency')
hqnDependency = quickerNESSubproject.get_variable('hqnDependency')

# Getting core configurations

# Do not get base repo if this is a subproject
if meson.is_subproject() == false
subdir('base')
endif

subdir('new')

