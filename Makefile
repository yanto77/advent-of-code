
help:
	echo "see Makefile"

format:
	find src/ -iname *.h -o -iname *.cpp | xargs clang-format -i

init:
	mkdir -p build
	/usr/bin/cmake -S . -B build/

build:
	/usr/bin/cmake --build build/ --config Debug --target all -- -j 14

clean:
	cd build/ && make clean

measure:
	sudo perf stat \
		-r 5 \
		-e task-clock,instructions,branches,branch-misses \
		-e cycles,stalled-cycles-frontend,stalled-cycles-backend \
		./build/advent2020

.PHONY: help format init build clean