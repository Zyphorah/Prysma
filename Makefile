# Variables
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
SRC_DIR = src
OBJ_DIR = build/obj
BIN_DIR = build
TARGET = $(BIN_DIR)/NexusMathEngine

# Trouver tous les fichiers .cpp récursivement dans src/
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
# Créer la liste des objets correspondants
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Règle par défaut
all: $(TARGET)

# Édition de liens (Linker) : Inclure TOUS les objets $(OBJS)
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des fichiers sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean