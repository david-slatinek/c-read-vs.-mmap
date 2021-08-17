all: build

build:
	gcc -Wall -O3 main.c -o main

clean:
	rm -f main

remove:
	rm -rf test_files/

permission:
	chmod u+x script.sh main.py
