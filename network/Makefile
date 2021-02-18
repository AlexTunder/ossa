default: napi
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
	mv = mv
	rm = rm
	cp = cp
endif
napi:
	@echo [GO] Building static lib
	@$(gocompl) build -buildmode=c-archive ./napi.go
	@echo [FS] Moving static lib to ossa/lib
	@$(mv) napi.a ../lib/net-ossa.lib
	@echo [FS] cleaning up
	@$(rm) napi.h
napi-cp:
	@$(gocompl) build -buildmode=c-archive napi.go
	@$(cp)  napi.a ../lib/net-ossa.a
	@$(rm) napi.h
api-test:
	@echo [CC] building test...
	@$(CC) api-test.c -o test ../lib/net-ossa.a $(CC_FLAG)