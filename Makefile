TARGET_EXEC := game

BUILD_DIR := build
SRC_DIR := src
INC_DIR := include

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

INCS := $(shell find $(INC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INCS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP

LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -rdynamic

CXX := g++ 
CC  := gcc
CXXFLAGS := -std=c++17

build: $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: build bots
	./$(BUILD_DIR)/$(TARGET_EXEC)

clean:
	rm -rf $(BUILD_DIR)


BOTS_DIR := bots
BOTS_BUILD_DIR := $(BUILD_DIR)/$(BOTS_DIR)

BOTS_SRCS := $(shell find $(BOTS_DIR) -name '*.cpp')
BOTS_TARGETS := $(patsubst $(BOTS_DIR)/%.cpp,$(BOTS_BUILD_DIR)/%.so,$(BOTS_SRCS))

$(BOTS_BUILD_DIR)/%.so: $(BOTS_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -shared $< -o $@


bots: $(BOTS_TARGETS)

.PHONY: clean bots run

-include $(DEPS)