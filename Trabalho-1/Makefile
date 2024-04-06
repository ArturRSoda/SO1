# ----- Variaveis -----
	# Nome executavel
NAME = main.out
	# Compilador
CC = g++
	# Flags
FLAGS = -Wall -g
	# Diretorio fonte
SRC = Src
	# Pasta dos arquivos objetos
OBJ_DIR = Obj
	# Target
TARGET = $(NAME)
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

all: $(TARGET)

# Cria o diretorio dos .o files
$(shell mkdir -p $(OBJ_DIR))

.PHONY: all clean
