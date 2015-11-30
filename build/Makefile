# MAKE FILE

RM:= rm
CC:= gcc
CFLAGS := -Wall

OBJETOS = main.o

all : Cliente_Con1

Cliente_Con1 : $(OBJETOS)
	$(CC) $(CFLAGS) $(OBJETOS) -o $@ -lm
	@echo 'Creación exitosa'


# Reglas Generales

%.o : ../src/%.c
	$(CC) -c $(DEFS) $(CFLAGS) $< -o $@

.PHONY: all clean

clean : 
	$(RM) Cliente_Con1 $(OBJETOS)

