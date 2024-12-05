CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lm -lpthread -ldl -lstdc++

TARGET = game
SRC = basico.c geral.c jogador.c viloes.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
