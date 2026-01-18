# Variables
CXX = g++

# 1. Récupération des flags LLVM
LLVM_CXXFLAGS := $(shell llvm-config --cxxflags)
LLVM_LDFLAGS  := $(shell llvm-config --ldflags --system-libs --libs core support irreader native)

# 2. Configuration des flags
# -fexceptions : Force l'activation des exceptions (écrase le -fno-exceptions de LLVM)
# -Wno-unused-parameter : Cache les warnings inutiles dans les headers LLVM
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude $(LLVM_CXXFLAGS) -fexceptions -Wno-unused-parameter

SRC_DIR = src
OBJ_DIR = build/obj
BIN_DIR = build
TARGET = $(BIN_DIR)/NexusMathEngine

# Trouver tous les fichiers .cpp
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Règle par défaut
all: $(TARGET)

# Édition de liens
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LLVM_LDFLAGS)

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean