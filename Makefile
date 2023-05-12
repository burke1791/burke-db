
all: bin
	gcc -o bin/cli src/cli/main.c src/cli/cli.c

bin:
	mkdir bin

clean:
	rm -rf bin
	mkdir bin