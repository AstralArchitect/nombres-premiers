project('nombres-premiers', 'cpp', 'cuda', version : '3.0.0')

# Define sources
main_sources = ['src/search.cu', 'src/main.cc']
bench_sources = ['src/search.cu', 'src/bench.cc']

# Define the executable
executable('main', main_sources)
executable('bench', bench_sources)