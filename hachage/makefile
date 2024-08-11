# Nom de l'exécutable
EXEC = calculate_hash

# Fichiers source
SRCS = calculate_hash.c sha3.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Compilateur et options
CC = gcc
CFLAGS = -I. -Wall -Wextra -std=c11

# Règle par défaut
all: $(EXEC)

# Règle pour créer l'exécutable
$(EXEC): $(OBJS)
	$(CC) -o $@ $^

# Règle pour compiler les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJS) $(EXEC)

# Règle pour exécuter le programme
run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
