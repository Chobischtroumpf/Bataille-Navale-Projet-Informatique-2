
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CORES := $(shell nproc)
else ifeq ($(UNAME_S),Darwin)
	CORES := $(shell sysctl -n hw.ncpu)
else
	CORES := 1
endif

all:
	@cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
	cmake --build build --config Release -- -j$(CORES)

debug :
	@cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug
	cmake --build build --config Debug -- -j$(CORES) 

clean:
	@rm -rf build

fclean: clean
	@rm -f server
	@rm -f battleship

run_debug: debug
	./server

re: clean all

re_debug: clean debug