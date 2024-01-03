build: 
	gcc -I src/include -L src/lib -o main main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

run:
	./main

clean:
	rm main