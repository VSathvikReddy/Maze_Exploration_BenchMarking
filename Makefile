TARGET_EXEC := game

BUILD_DIR := build
SRC_DIR := src
INC_DIR := include

CPP_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
C_SRCS   := $(shell find $(SRC_DIR) -name '*.c')
CPP_OBJS := $(CPP_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
C_OBJS   := $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
#SRCS := $(CPP_SRCS) $(C_SRCS)
OBJS := $(CPP_OBJS) $(C_OBJS)
DEPS := $(OBJS:.o=.d)

INCS := $(shell find $(INC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INCS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP

LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

CXX := g++
CC  := gcc

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

run: $(BUILD_DIR)/$(TARGET_EXEC)
	./$(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)