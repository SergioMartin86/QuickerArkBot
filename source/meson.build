# Getting quicker nes dependency
quickerNESSubproject = subproject('quickerNES', default_options : 'buildQuickNES=false')

# Getting QuickerNES dependency
quickerNESDependency = quickerNESSubproject.get_variable('quickerNESDependency')

# Getting core configurations

# Do not get base repo if this is a subproject
if meson.is_subproject() == false
subdir('base')
endif

subdir('new')

