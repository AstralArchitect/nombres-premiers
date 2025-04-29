project('nombres-premiers', 'cpp', 'cuda', version : '3.0.0')

cpp_compiler = meson.get_compiler('cpp')

if cpp_compiler.get_id() == 'gcc' or cpp_compiler.get_id() == 'clang'
  message('Compilateur GCC/clang détecté, activation des optimisations supplémentaires...')
  add_global_arguments('-Ofast', '-march=native', language: 'cpp')
else
  message('Un autre compilateur que GCC est utilisé, aucune optimisation supplémentaire du compilateur ne sera effectué')
endif

# Define sources
sources = ['src/main.cc', 'src/search.cu']

# Define the executable
executable('main', sources, include_directories: 'Include/')
bench = executable('bench', sources, include_directories: 'Include/', cpp_args: '-DBENCH')