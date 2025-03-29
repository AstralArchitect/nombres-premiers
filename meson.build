#project('nombres-premiers', 'cpp', 'cuda', version : '3.0.0')
project('nombres-premiers', 'cpp', version : '3.0.1')

cpp_compiler = meson.get_compiler('cpp')

if cpp_compiler.get_id() == 'gcc' or cpp_compiler.get_id() == 'clang'
  message('Compilateur GCC/clang détecté, activation des optimisations supplémentaires...')
  add_global_arguments('-Ofast', '-march=native', language: 'cpp')
else
  message('Un autre compilateur que GCC est utilisé, aucune optimisation supplémentaire du compilateur ne sera effectué')
endif

# Define sources
#sources = ['src/search.cc', 'src/main.cc', 'src/search.cu']
sources = ['src/search.cc', 'src/main.cc']

# Define the executable
executable('main', sources, include_directories: 'Include/')
cpu_bench = executable('bench', sources, include_directories: 'Include/', cpp_args: '-DBENCH')

test('bench', cpu_bench, is_parallel: false)

# uncoment this 3 lines if you're using cuda
#cuda = executable('cuda', sources, include_directories: 'Include')
#cuda_bench = executable('bench_cuda', sources, include_directories: 'Include/', cpp_args: '-DBENCH', cpp_args: '-DCUDA')
#test('cuda_bench', cuda_bench, is_parallel: false)