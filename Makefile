BUILD_DIR = ./build
OUTPUT_EXEC = ./build/advent_of_code

# Ref: https://github.com/aras-p/ClangBuildAnalyzer/releases/tag/v1.4.0
TMP_CAPTURE_FILE = capture_file.tmp
BUILD_TIME_TXT = build_time_analysis.txt
BUILD_ANALYZER_TOOL = ~/Downloads/software/ClangBuildAnalyzer-linux

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

test-hotspots:
	sudo perf record \
		--freq 500 \
		-e cycles \
		-e sched:sched_switch --switch-events \
		--sample-cpu \
		-m 8M \
		--aio \
		--call-graph dwarf \
		${OUTPUT_EXEC}

	sudo chown user:user perf.data
	hotspot perf.data

test-build-time:
	@rm -rf ${BUILD_DIR}
	@mkdir ${BUILD_DIR}

	@${BUILD_ANALYZER_TOOL} --start ${BUILD_DIR}
	@cd ${BUILD_DIR} \
		&& cmake \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_CXX_COMPILER=/bin/clang++-14 \
			-DENABLE_TIME_TRACE=ON \
			.. \
		&& cmake --build . -j 
	@${BUILD_ANALYZER_TOOL} --stop ${BUILD_DIR} ${TMP_CAPTURE_FILE}
	@${BUILD_ANALYZER_TOOL} --analyze ${TMP_CAPTURE_FILE} > ${BUILD_TIME_TXT}
	@head ${BUILD_TIME_TXT} -n 6
	@ls -lh ${OUTPUT_EXEC}
	@rm ${TMP_CAPTURE_FILE}

.PHONY: help format init build clean