BUILD_DIR = ./build/
BUILD_DIR_BIN = $(BUILD_DIR)bin/
RM_ARGS := -rf ./build
SRC_DIR = ./src/

SRC_SERVER_DIR = $(SRC_DIR)server/
SRC_CLIENT_DIR = $(SRC_DIR)client/
SRC_COMMON_DIR = $(SRC_DIR)common/

OS_NAME := $(shell uname -s)
ARCHITECTURE := $(shell uname -p)

OPTIMIZATION = -O3 -funroll-loops
CC = g++
FLAGS = -std=c++2a $(shell ncurses5.4-config --cflags) -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Weffc++ -Wmisleading-indentation -Wold-style-cast -Wzero-as-null-pointer-constant -Wsign-promo -Woverloaded-virtual -Wctor-dtor-privacy

LINKING_EMOJI = "🔗"
COMPILING_EMOJI = "🚀"


SERVER_BIN_PATH = $(BUILD_DIR_BIN)server
CLIENT_BIN_PATH = $(BUILD_DIR_BIN)client

ifeq ($(OS_NAME),Darwin) # certaines options ne sont pas dispo sur g++ pour mac (et il est plus simple pour moi de compiler sur mac pendant que je code)
	RM_ARGS += *.dSYM # symboles pour debugage (sur mac)
else
	OPTIMIZATION += -fopenmp -frename-registers
	FLAGS += -march=native -fconcepts -Wuseless-cast -Wstrict-null-sentinel -Wnoexcept -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override
endif

ifneq ($(ARCHITECTURE),arm) # M1 MacBook
ifneq ($(ARCHITECTURE),aarch64)
	FLAGS += -masm=intel -march=native
endif
endif

ifneq ($(NO_OPT),1)
	FLAGS += $(OPTIMIZATION)
endif

all: announce $(SERVER_BIN_PATH) $(CLIENT_BIN_PATH)
	@echo "😎 Done! \n\n"
	@echo "📍 The binaries are located at $(BUILD_DIR_BIN)\n"

announce:
	@echo "$(COMPILING_EMOJI) Compiles with:\n $(CC) $(FLAGS) \n"
	@echo "💪 Optimisations:\n $(OPTIMIZATION) \n"

$(SERVER_BIN_PATH): $(patsubst %.cpp, %.o, $(patsubst $(SRC_DIR)%, $(BUILD_DIR)%, $(foreach dir, $(SRC_SERVER_DIR) $(SRC_COMMON_DIR), $(wildcard $(dir)*.cpp))))
	@echo "\n$(LINKING_EMOJI) linking $@\n"
	mkdir -p $(@D)
	@$(CC) $(OPTIMIZATION) $(FLAGS) -o $@ $^

$(CLIENT_BIN_PATH): $(patsubst %.cpp, %.o, $(patsubst $(SRC_DIR)%, $(BUILD_DIR)%, $(foreach dir, $(SRC_CLIENT_DIR) $(SRC_COMMON_DIR), $(wildcard $(dir)*.cpp))))
	@echo "\n$(LINKING_EMOJI) linking $@\n"
	mkdir -p $(@D)
	@$(CC) $(OPTIMIZATION) $(FLAGS) -o $@ $^

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp | $(BUILD_DIR)
	@echo "$(COMPILING_EMOJI) compiling $< to $@"
	mkdir -p $(@D)
	@$(CC) $(OPTIMIZATION) $(FLAGS) -c -o $@ $<

clean:
	rm $(RM_ARGS) 2>/dev/null

$(BUILD_DIR):
	@mkdir -p $@