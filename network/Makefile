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
	@echo [GO] Building OSB
	@$(gocompl) build -buildmode=c-archive ./osb.go
	@echo [FS] Moving protocol static lib to ossa/lib
	@$(mv) ./osb.a ../lib/libossac.a
	@echo [FS] cleaning up
	@$(rm) osb.h
napi-cp:
	@$(gocompl) build -buildmode=c-archive osb.go
	@$(cp)  osb.a ../lib/libossac.a
	@$(rm) osb.h
api-test:
	@echo [CC] building test...
	@$(CC) api-test.c -o test ../lib/libossac.a ../lib/libossa.a $(CC_FLAG)