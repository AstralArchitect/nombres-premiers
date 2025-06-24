# Target to compile the project
all:
	meson compile -C build/

# Target to run the compiled binary
run: all
	./build/main

test: all
	./build/main 100
	timeout /t 3 > nul
	./build/main 1000
	echo "104729"
	timeout /t 3 > nul
	./build/main 10000


# Target to set up the build directory
setupClang:
	meson setup build --native-file=native-clang.txt
setup:
	meson setup build
ifeq ($(OS),Windows_NT)
	@if exist build/build.ninja (powershell -NoLogo -NoProfile -Command "(Get-Content build\build.ninja) -replace ' ""msvcrt.lib""""', '' | Set-Content build\build.ninja")
endif

# Target to clean the build directory
clean:
ifeq ($(OS),Windows_NT)
	@if exist build (rmdir build /S /Q)
else
	@if [ -d build ]; then rm -rf build; fi
endif