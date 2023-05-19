
all: bin
	gcc -Isrc/include -o bin/cli src/cli/main.c src/cli/cli.c src/cli/initdb.c src/config/config.c src/storage/datapage.c

bin:
	mkdir bin
	mkdir bin/data

clean:
	rm -rf bin
	mkdir bin
	mkdir bin/data