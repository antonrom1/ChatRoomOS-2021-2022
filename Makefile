SRC_DIR = ./src/
BUILD_DIR = ./build/
RM_ARGS := -rf ./build server
OPTIMIZATION = -O3 -funroll-loops
CC = g++
FLAGS = -std=c++2a -ggdb3 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Weffc++ -Wmisleading-indentation -Wold-style-cast -Wzero-as-null-pointer-constant -Wsign-promo -Woverloaded-virtual -Wctor-dtor-privacy
OS_NAME := $(shell uname -s)
ARCHITECTURE := $(shell uname -p)

ifeq ($(OS_NAME),Darwin) # certaines options ne sont pas dispo sur g++ pour mac (et il est plus simple pour moi de compiler sur mac pendant que je code)
	RM_ARGS += *.dSYM # symboles pour debugage (sur mac)
	ifneq ($(ARCHITECTURE),arm)
		FLAGS := -mlong-double-128 -masm=intel -march=native
	endif
else
	OPTIMIZATION =  $(OPRIMIZATION) -fopenmp -frename-registers
	FLAGS := $(FLAGS) -masm=intel -march=native -fconcepts -mlong-double-128-Wuseless-cast -Wstrict-null-sentinel -Wnoexcept -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override
endif

all: announce server client
	@echo Done!

announce:
	@echo "Compiles with:\n $(CC) $(FLAGS) \n"
	@echo "Optimisations:\n $(OPTIMIZATION) \n"

server: $(patsubst %.cpp, %.o, $(patsubst $(SRC_DIR)%, $(BUILD_DIR)%, $(wildcard $(SRC_DIR)server/*.cpp) $(wildcard $(SRC_DIR)common/*.cpp)))
	@echo linking... $@ $^
	@$(CC) $(OPTIMIZATION) $(FLAGS) -o $@ $^

client: $(patsubst %.cpp, %.o, $(patsubst $(SRC_DIR)%, $(BUILD_DIR)%, $(wildcard $(SRC_DIR)client/*.cpp) $(wildcard $(SRC_DIR)common/*.cpp)))
	@echo linking... $@ $^
	@$(CC) $(OPTIMIZATION) $(FLAGS) -o $@ $^

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp | $(BUILD_DIR)
	@echo compiling $< to $@
	mkdir -p $(@D)
	@$(CC) $(OPTIMIZATION) $(FLAGS) -c -o $@ $<

clean:
	rm $(RM_ARGS) 2>/dev/null

$(BUILD_DIR):
	@mkdir $@