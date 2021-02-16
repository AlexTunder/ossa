default: full
bin = ../bin
core = ../core
languages = ../languages
network = ../network
CC = gcc
ifeq ($(OS),Windows_NT)
	# If it's Windows
	gocompl = C:\Program Files\Go\bin\go
	mv = move
	rm = del
	cp = copy
	# CC_FLAG += -lpthread
else
	gocompl = 'go'
	CC_FLAG += -lpthread
endif

network:
	@echo [MF] network
	@make -C $(network) -s
osmic:
	@echo [CC] osmic.c
	@$(CC) osmic.c -o $(bin)/osmic $(CC_FLAG)
full: network osmic