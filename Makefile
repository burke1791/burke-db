
all: bin
	gcc -Isrc/include -o bin/cli src/cli/main.c src/cli/cli.c src/config/config.c

bin:
	mkdir bin

clean:
	rm -rf bin
	mkdir bin