
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CORES := $(shell nproc)
else ifeq ($(UNAME_S),Darwin)
	CORES := $(shell sysctl -n hw.ncpu)
else
	CORES := 1
endif

# DATABASE_DIR := "$(shell pwd)/src/Server/Database/DDL_user_db.sql"

all:
	@cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	cmake --build build --config Release -- -j$(CORES)

debug :
	@cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	cmake --build build --config Debug -- -j$(CORES) 

clean:
	@rm -rf build

fclean: clean
	@rm -f server
	@rm -f battleship

run_debug: debug
	./server

run: all
	./server

re: clean all

re_debug: clean debug

.PHONY: all clean fclean re run_debug re_debug