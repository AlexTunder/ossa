napi:
	go build -buildmode=c-archive napi.go
clean-src:
	rm napi.h
api-test:
	gcc api-test.c -o test ./napi.a -lpthread
default: napi clean-src