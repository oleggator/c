BIN=rest

rest: rest.o http.o api.o
	cc -std=c11 -g rest.o api.o http.o -o $(BIN)
rest.o: rest.c
	cc -std=c11 -g -c rest.c
http.o: http.c
	cc -std=c11 -g -c http.c
api.o: api.c
	cc -std=c11 -g -c api.c
clean:
	rm *.o $(BIN)
