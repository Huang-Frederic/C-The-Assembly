build: 
	gcc -I src/include -L src/lib -o font font.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

run:
	./font

clean:
	rm main