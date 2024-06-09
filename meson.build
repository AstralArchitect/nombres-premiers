project('nombres-premiers', 'c', 'cuda', version : '1.0.0')

thread = dependency('threads')

cpu_search = library('cpu_search', 'Code/cpu_search.c', dependencies: thread)

# Define sources
sources = ['Code/main.cu', 'Code/gpu_search.cu']

# Define the executable
executable('main', sources, link_with: cpu_search)