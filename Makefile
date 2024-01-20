build: 

	gcc -I src/include -L src/lib -o wip main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lsqlite3 -lcurl -ljson-c

run:
	./wip

clean:
	rm wip
