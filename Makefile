EXEC = main
GCC = clang++

SRC_DIR = ./src
BUILD_DIR = ./build
INCLUDE_DIR = include

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(BUILD_DIR)/%.o, $(SRC_DIR)/%.cpp, $(SRCS))

LIBRARIES = -ldl -lglfw
CFLAGS = -Wall -Werror -g -std=c++20 -I$(INCLUDE_DIR)

$(EXEC): $(OBJS)
	$(GCC) $(CFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/%.o: %(SRC_DIR)/%.cpp
	$(GCC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXEC)
	
