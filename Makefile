# Variables
CXX = g++

# 1. Récupération des flags LLVM
LLVM_CXXFLAGS := $(shell llvm-config --cxxflags)
LLVM_CXXFLAGS := $(subst -I/,-isystem /,$(LLVM_CXXFLAGS)) 
LLVM_LDFLAGS  := $(shell llvm-config --ldflags --system-libs --libs core support irreader native)

# 2. Configuration des flags
# -fexceptions : Force l'activation des exceptions (écrase le -fno-exceptions de LLVM)
# -Wno-unused-parameter : Cache les warnings inutiles dans les headers LLVM
PRYSMA_CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3 -Iinclude -Ibuild/generationCode/include $(LLVM_CXXFLAGS) -fexceptions -Wno-unused-parameter

SRC_DIR = src
GEN_DIR = build/generationCode
OBJ_DIR = build/obj
BIN_DIR = build
TARGET = $(BIN_DIR)/Prysma

# Trouver tous les fichiers .cpp
SRCS := $(shell find $(SRC_DIR) \
	-path "$(SRC_DIR)/Compilateur/AST/Noeuds/StrategieEquation" -prune -o \
	-path "$(SRC_DIR)/Compilateur/AnalyseSyntaxique/ParseurInstruction.cpp" -prune -o \
	-path "$(SRC_DIR)/Compilateur/AnalyseSyntaxique/ParseurEquation.cpp" -prune -o \
	-path "$(SRC_DIR)/Compilateur/AnalyseSyntaxique/Math/ParseurLitteral.cpp" -prune -o \
	-path "$(SRC_DIR)/Compilateur/AnalyseSyntaxique/Math/ParseurNegation.cpp" -prune -o \
	-path "$(SRC_DIR)/Compilateur/AnalyseSyntaxique/Tableau/ParseurTableauInitialisation.cpp" -prune -o \
	-name "*.cpp" -print)
GEN_SRCS := $(shell find $(GEN_DIR)/src -name "*.cpp" 2>/dev/null)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
GEN_OBJS := $(GEN_SRCS:$(GEN_DIR)/%.cpp=$(OBJ_DIR)/gen/%.o)

# Règle par défaut
all: $(TARGET)

# Édition de liens
$(TARGET): $(OBJS) $(GEN_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(PRYSMA_CXXFLAGS) $(CXXFLAGS) -o $@ $^ $(LLVM_LDFLAGS)

# Compilation des sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(PRYSMA_CXXFLAGS) $(CXXFLAGS) -c $< -o $@

# Compilation des fichiers générés
$(OBJ_DIR)/gen/%.o: $(GEN_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(PRYSMA_CXXFLAGS) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BIN_DIR)

# Générer compile_commands.json pour clangd/bear
compile_commands: clean
	@echo "Génération de compile_commands.json avec bear..."
	bear --output build/compile_commands.json -- make

.PHONY: all clean compile_commands