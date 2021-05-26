
help:
	echo "see Makefile"

format:
	find src/ -iname *.h -o -iname *.cpp | xargs clang-format -i

init:
	mkdir -p build
	/usr/bin/cmake -S . -B build/

build:
	/usr/bin/cmake --build build/ --config Release --target all -- -j

clean:
	cd build/ && make clean

.PHONY: help format init build clean