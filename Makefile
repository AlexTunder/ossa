default: lib osmic
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
CC_FLAG += -std=c99

CNeoBase:
	@echo [CC] core/newobase.c
	@$(CC) -c core/neobase.c -o lib/libossa.a -std=c99
CBase:
	@echo [CC] core/base.c
	@$(CC) -c core/base.c -o lib/libossa.a -std=c99
net:
	@echo [MF] network/osb
	@make -C ./network -s
osmic:
	@echo [CC] cli/osmic.c
	@$(CC) cli/osmic.c -o bin/osmic $(CC_FLAG) ./lib/libossac.a
osmic-gdb:
	@echo [CC] cli/osmic.c
	@$(CC) cli/osmic.c -o bin/osmic $(CC_FLAG) ./lib/libossac.a -g3
full: lib osmic
lib: CBase net
test.net:
	@make lib -s
	@make -C network api-test -s
bengo:
	@echo [CC] cli/bengo
	@$(CC) ./cli/bengo.c -o ./bin/bengo -g3 -Wall