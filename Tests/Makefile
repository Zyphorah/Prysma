.PHONY: all build clean rebuild

all: build

build:
	@cmake -B build
	@cmake --build build

clean:
	@rm -rf build

rebuild: clean build
