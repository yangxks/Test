
CFLAGS ?= -g -Wall -Werror

all: client server

client: client.o
	cc -o $@ $^ account_clnt.c account.h
	-@strip $@

server: server.o
	cc -o $@ $^ account_svc.c account.h
	-@strip $@

%.o: %.c
	cc $(CFLAGS) -o $@ -c $<

clean:
	rm -f client server *.o
