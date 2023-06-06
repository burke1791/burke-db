
all: bin
	gcc -Isrc/include -o bin/cli src/cli/main.c src/cli/cli.c src/cli/initdb.c src/config/config.c src/storage/datapage.c src/employee/emp_tuple.c src/cli/emp_list.c src/cli/resultset_output.c src/tuple/datum.c src/tuple/tuple.c

bin:
	mkdir bin
	mkdir bin/data

clean:
	rm -rf bin
	mkdir bin
	mkdir bin/data