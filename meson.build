project('nombres-premiers', 'cpp', version : '1.0.0')

add_global_arguments('-DBENCH', language: 'cpp')

cpp_compiler = meson.get_compiler('cpp')

if cpp_compiler.get_id() == 'gcc'
  message('Compilateur GCC détecté, activation des optimisations supplémentaires...')
  #add_global_arguments('-Ofast', language: 'cpp')
else
  message('Un autre compilateur que GCC est utilisé, aucune optimisation supplémentaire du compilateur ne sera effectué')
endif

# Define sources
sources = ['src/search.cc', 'src/main.cc']

# Define the executable
executable('main', sources, include_directories: 'Include/')