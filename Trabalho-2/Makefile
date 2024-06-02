# ----- Variaveis -----
	# Target
TARGET = simulador
	# Compilador
CC = g++
	# Flags
FLAGS = -Wall -g
	# Diretorio fonte
SRC = Src
	# Pasta dos arquivos objetos
OBJ_DIR = Obj
# ---------------------


# ----- Files -----
	# .cpp files
SRCS = $(wildcard $(SRC)/*.cpp)
	# .h files
HDRS = $(wildcard $(SRC)/*.h)
	# .o files
OBJS = $(patsubst $(SRC)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
# ----------------


# ----- Compilacao -----
# Linka arquivos objetos
$(TARGET): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^

# Compila
$(OBJ_DIR)/%.o: $(SRC)/%.cpp $(HDRS)
	$(CC) $(FLAGS) -c -o $@ $<
# ----------------------


clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Cria o diretorio dos .o files
$(shell mkdir -p $(OBJ_DIR))

.PHONY: clean
