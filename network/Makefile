napi:
	go build -buildmode=c-archive napi.go
	mv napi.a ../lib/net-ossa.a
	rm napi.h
napi-cp:
	go build -buildmode=c-archive napi.go
	cp  napi.a ../lib/net-ossa.a
	rm napi.h
api-test:
	gcc api-test.c -o test ../lib/net-ossa.a -lpthread
default: napi