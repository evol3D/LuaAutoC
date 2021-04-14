project('luaautoc', 'c',
  default_options: [
    'default_library=static',
  ],
)

lac_src = files('lautoc.c')
lac_inc = include_directories('.')

lac_lib = library('luaautoc',
  lac_src,
  include_directories: lac_inc,
  c_args: ['-DLAC_DLL', '-DLAC_IMPL'],
)

lac_dep = declare_dependency(
  link_with: lac_lib,
  include_directories: lac_inc,
  compile_args: ['-DLAC_DLL'],
)

meson.override_dependency('luaautoc', lac_dep)
