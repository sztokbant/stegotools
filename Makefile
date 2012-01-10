all:
	gcc -O4 stegread.c -o stegread
	gcc -O4 stegwrite.c -o stegwrite

clean:
	-rm stegread stegwrite
	-rm *~

test:
	sh test.sh
