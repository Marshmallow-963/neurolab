CC = gcc

INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

TARGET = $(BIN_DIR)/neurolab

CFLAGS  = -Wall -Wextra -std=c99 -g -O2 -DRAYGUI_SUPPORT_ICONS
LDFLAGS = -L$(LIB_DIR) -lraylib -lm -lpthread -ldl -lrt -lX11

SOURCES = $(shell find $(SRC_DIR) -name "*.c")
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

CPPFLAGS = -I$(INC_DIR) -L$(LIB_DIR) -MMD -MP

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "==> Criando o executável: $@"
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "==> Copiando 'assets' para '$(BIN_DIR)'..."
	@cp -r assets $(@D)/
	@echo "=> Compilação concluída com sucesso! Executável em: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "==> Compilando: $<"
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@echo "==> Removendo diretórios $(BIN_DIR) e $(OBJ_DIR)..."
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	@echo "==> Limpeza Concluída"

-include $(OBJS:.o=.d)

.PHONY: all clean
