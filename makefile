.PHONY: all compile build clean run

all: build

compile:
	cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release


build: compile
	cmake --build build --config Release

clean:
	-cmake -E rm -rf build
	-cmake -E rm -rf clone
	-cmake -E rm -rf .cache
	-cmake -E rm -rf src/imgui/.cache
	-cmake -E rm -rf src/core/.cache

run:
	./build/prog

update-png: all
	./build/prog ./not-a-robot/alpha.png 1 alpha
	./build/prog ./not-a-robot/beta.png 1 beta

