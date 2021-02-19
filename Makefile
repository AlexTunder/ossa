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

CBase:
	@echo [MF] core/base.c
	@gcc -c core/base.c -o lib/libossa.a
net:
	@echo [MF] network
	@make -C ./network -s
osmic:
	@echo [CC] cli/osmic.c
	@$(CC) cli/osmic.c -o bin/osmic $(CC_FLAG) ./lib/libossac.a
full: lib osmic
lib: CBase net
test.net:
	@make lib -s
	@make -C network api-test -s