SRC_DIR = ./src/
BUILD_DIR = ./build/
BUILD_DIR_BIN = $(BUILD_DIR)bin/
RM_ARGS := -rf ./build
OPTIMIZATION = -O3 -funroll-loops
CC = g++
FLAGS = -std=c++2a -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Weffc++ -Wmisleading-indentation -Wold-style-cast -Wzero-as-null-pointer-constant -Wsign-promo -Woverloaded-virtual -Wctor-dtor-privacy
OS_NAME := $(shell uname -s)
ARCHITECTURE := $(shell uname -p)

SERVER_BIN_PATH = $(BUILD_DIR_BIN)server
CLIENT_BIN_PATH = $(BUILD_DIR_BIN)client

ifeq ($(OS_NAME),Darwin) # certaines options ne sont pas dispo sur g++ pour mac (et il est plus simple pour moi de compiler sur mac pendant que je code)
	RM_ARGS += *.dSYM # symboles pour debugage (sur mac)
	ifneq ($(ARCHITECTURE),arm)
		FLAGS := -mlong-double-128 -masm=intel -march=native
	endif
else
	OPTIMIZATION =  $(OPTIMIZATION) -fopenmp -frename-registers
	FLAGS += -masm=intel -march=native -fconcepts -mlong-double-128 -Wuseless-cast -Wstrict-null-sentinel -Wnoexcept -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override
endif

ifneq ($(NO_OPT),1)
	FLAGS += $(OPTIMIZATION)
endif

all: announce $(SERVER_BIN_PATH) $(CLIENT_BIN_PATH)
	@echo Done!

announce:
	@echo "Compiles with:\n $(CC) $(FLAGS) \n"
	@echo "Optimisations:\n $(OPTIMIZATION) \n"

$(SERVER_BIN_PATH): $(patsubst %.cpp, %.o, $(patsubst $(SRC_DIR)%, $(BUILD_DIR)%, $(wildcard $(SRC_DIR)server/*.cpp) $(wildcard $(SRC_DIR)common/*.cpp)))
	@echo linking... $@ $^
	mkdir -p $(@D)
	@$(CC) $(OPTIMIZATION) $(FLAGS) -o $@ $^

$(CLIENT_BIN_PATH): $(patsubst %.cpp, %.o, $(patsubst $(SRC_DIR)%, $(BUILD_DIR)%, $(wildcard $(SRC_DIR)client/*.cpp) $(wildcard $(SRC_DIR)common/*.cpp)))
	@echo linking... $@ $^
	mkdir -p $(@D)
	@$(CC) $(OPTIMIZATION) $(FLAGS) -o $@ $^

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp | $(BUILD_DIR)
	@echo compiling $< to $@
	mkdir -p $(@D)
	@$(CC) $(OPTIMIZATION) $(FLAGS) -c -o $@ $<

clean:
	rm $(RM_ARGS) 2>/dev/null

$(BUILD_DIR):
	@mkdir -p $@