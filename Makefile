CXXFLAGS += -std='c++20' -pedantic -g #-Wall -Wextra -Wsign-compare  # -fmax-errors=2
# Sur Mac, supprimer la ligne suivante
CXXFLAGS += -Walloc-zero -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wduplicated-branches -Wduplicated-cond -Wextra-semi -Wfloat-equal -Wformat-signedness -Winit-self -Wlogical-op -Wnon-virtual-dtor -Wnull-dereference -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wstrict-null-sentinel -Wsuggest-attribute=const -Wsuggest-override -Wswitch-default -Wswitch -Wundef -Wuseless-cast -Wvolatile -Wzero-as-null-pointer-constant -Wformat=2 -fsanitize=undefined,address,leak #-fmax-errors=2
CXX = g++

# Compile project

SRC_DIR = src_remise1/
BUILD_DIR = build/

HEADERS = $(wildcard ${SRC_DIR)*.hh)
SOURCES = $(wildcard ${SRC_DIR}*.cc)
OBJECTS = $(patsubst ${SRC_DIR}%.cc,${BUILD_DIR}%.o,${SOURCES})
DEPENDS = $(patsubst ${SRC_DIR}%.cc,${BUILD_DIR}%.d,$(SOURCES))

.PHONY: default
default: battleship

battleship: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${LDFLAGS} $^ -o $@ ${LOADLIBES} ${LDLIBS}

# Need to recompile on header change

-include $(DEPENDS)

${BUILD_DIR}%.o: ${SRC_DIR}%.cc Makefile
	@mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} ${LDFLAGS} -MMD -MP -c $< -o $@ ${LOADLIBES} ${LDLIBS}

# Run
.PHONY: run
run:
	@touch /tmp/battleship-err
	@echo
	@echo "I will now run ``./battleship`` with errors redirected to /tmp/battleship-err"
	@echo "You should use ``tail -f /tmp/battleship-err`` in another terminal to see the error messages"
	@echo "Press Enter to run battleship"
	@echo
	@read line
	./battleship 2> /tmp/battleship-err 


# make clean supprime les fichiers objets et dépendances
.PHONY: clean
clean:
	-rm ${BUILD_DIR}**/*.o ${BUILD_DIR}**/*.d

# make mrclean supprime les fichiers objets et les exécutables
.PHONY: mrclean
mrclean: clean
	-rm battleship

