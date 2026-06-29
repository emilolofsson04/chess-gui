CC = gcc
CFLAGS = -Wall -Wextra -g

SOURCES = main.c board.c initialize.c kingsafety.c movegen.c playmove.c printAllMoves.c verifylegality.c unplaymove.c Gui.c logs.c firstframeperturn.c
TARGET = chess

TARGET = chess
OBJ_DIR = obj

ENGINE_SRC = ../Computer/Engine.c
# Check if Engine installed
ifneq ("$(wildcard $(ENGINE_SRC))","")
    SOURCES += $(ENGINE_SRC)
    CFLAGS += -DHAS_ENGINE
endif

OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SOURCES)))


all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ -lraylib -lm

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJ_DIR)/%.o: ../Computer/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
