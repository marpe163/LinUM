CFLAGS = -O2 -W -Wall -Wextra -pedantic -std=c11 -D_GNU_SOURCE
LIBS = -lm

BIN = test
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

.PHONY: all clean

all: $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

clean:
	rm -f $(OBJ) $(DEP) $(BIN)

-include $(DEP)
