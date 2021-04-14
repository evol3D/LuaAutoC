project('luaautoc', 'c',
  default_options: [
    'default_library=static',
    'c_std=c17',
  ],
)

lac_src = files('lautoc.c')
lac_inc = include_directories('.')

lac_deps = []

luajit_dep = dependency('luajit', required: false)
if luajit_dep.found()
  lac_deps += luajit_dep
else
  lua_dep = dependency('lua', required: false)
  if lua_dep.found()
    lac_deps += lua_dep
  else
    error('Can\'t find neither `lua` nor `luajit` dependencies')
  endif
endif

lac_lib = library('luaautoc',
  lac_src,
  include_directories: lac_inc,
  dependencies: [lac_deps],
  c_args: ['-DLAC_DLL', '-DLAC_IMPL'],
)

lac_dep = declare_dependency(
  link_with: lac_lib,
  include_directories: lac_inc,
  compile_args: ['-DLAC_DLL'],
)

meson.override_dependency('luaautoc', lac_dep)
