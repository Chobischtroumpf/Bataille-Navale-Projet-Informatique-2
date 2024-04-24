
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CORES := $(shell nproc)
else ifeq ($(UNAME_S),Darwin)
	CORES := $(shell sysctl -n hw.ncpu)
else
	CORES := 1
endif

all:
	@cmake -S . -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_EXPORT_COMPILE_COMMANDS=1
	cmake --build build --config Release -- -j$(CORES)

debug :
	@cmake -S . -B build -D CMAKE_BUILD_TYPE=Debug -D CMAKE_EXPORT_COMPILE_COMMANDS=1
	cmake --build build --config Debug -- -j$(CORES) 

install:
	@chmod +x ./install_dep
	@./install_dep 2> /tmp/install_dep.log || { @echo 'Installation failed, please check the log file (/tnp/install_dep.log) to see what went wrong, or run install_dep manually' && exit 1; }
	@echo "Installation successful"

clean:
	@rm -rf build

clean_dep:
	@rm -rf ./lib/*.deb
	@rm -rf ./lib/usr
	@rm -rf ./lib/etc
	@rm -rf ./lib/var

fclean: clean clean_dep
	@rm -f server
	@rm -f battleship

re: clean all

re_debug: clean debug

.PHONY: all clean fclean run_debug clean_dep re re_debug