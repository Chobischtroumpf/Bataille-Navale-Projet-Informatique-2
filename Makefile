
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CORES := $(shell nproc)
else ifeq ($(UNAME_S),Darwin)
	CORES := $(shell sysctl -n hw.ncpu)
else
	CORES := 1
endif

all:
	@cmake -S . -B build
	cmake --build build --config Release -- -j$(CORES)

clean:
	@rm -rf build

fclean: clean
	@rm -f server
	@rm -f battleship

run: all
	./server &

re: clean all