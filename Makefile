build: 
	gcc -I src/include -L src/lib -o menu menu.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

run:
	./menu

clean:
	rm main