.PHONY: compile clean

compile server.out client.out: client.o networking.o server.o history.o
	@gcc -o client client.o networking.o history.o
	@gcc -o server server.o networking.o history.o

client: client.o networking.o history.o
	@gcc -o client client.o networking.o history.o

server: server.o networking.o history.o
	@gcc -o server server.o networking.o history.o
	
client.o: client.c networking.h history.h
	@gcc -c client.c

server.o: server.c networking.h history.h
	@gcc -c server.c

networking.o: networking.c networking.h
	@gcc -c networking.c
	
history.o: history.c history.h
	@gcc -c history.c

clean:
	@rm -f *.o
	@rm -f client
	@rm -f server
	@rm -f *~
