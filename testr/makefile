# Compiler
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11

# Les fichiers sources
SOURCES = generate_hash_table.c search_rainbow.c sha3.c reduce_hash.c

# Les fichiers objets
OBJECTS = $(SOURCES:.c=.o)

# Exécutables
EXECUTABLES = generate_hash_table search_rainbow

# Cible par défaut
all: $(EXECUTABLES)

# Générer les exécutables
generate_hash_table: generate_hash_table.o sha3.o reduce_hash.o
	$(CC) $(CFLAGS) -o $@ $^

search_rainbow: search_rainbow.o sha3.o reduce_hash.o
	$(CC) $(CFLAGS) -o $@ $^

# Générer les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyer les fichiers objets et exécutables
clean:
	rm -f $(OBJECTS) $(EXECUTABLES)

# Recompiler tout
rebuild: clean all
