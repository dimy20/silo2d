EXEC = main
GCC = clang++

SRC_DIR = ./src
BUILD_DIR = ./build
INCLUDE_DIR = include

SRCS = $(wildcard $(SRC_DIR)/*.cpp $(wildcard) $(SRC_DIR)/Physics2D/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

#Imgui source files and object files definitions
IMGUI_SRCDIR = ./Imgui
IMGUI_SRCS = $(wildcard $(IMGUI_SRCDIR)/*.cpp)
IMGUI_OBJS = $(patsubst $(IMGUI_SRCDIR)/%.cpp, $(BUILD_DIR)/Imgui/%.o, $(IMGUI_SRCS))

LIBRARIES = -ldl -lglfw
CFLAGS = -Wall -Werror -g -std=c++17 -I$(INCLUDE_DIR) -I$(IMGUI_SRCDIR)

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS) $(IMGUI_OBJS)
	$(GCC) $(CFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(GCC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/Imgui/%.o: $(IMGUI_SRCDIR)/%.cpp | $(BUILD_DIR)/Imgui
	$(GCC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/Physics2D

$(BUILD_DIR)/Imgui:
	mkdir -p $(BUILD_DIR)/Imgui

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/Imgui/*.o $(EXEC)
