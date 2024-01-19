build: 
	gcc -I src/include -L src/lib -o main main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lsqlite3 

run:
	./main

clean:
	rm main