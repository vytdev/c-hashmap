test: hashmap.o test.o
	clang -o test hashmap.o test.o

hashmap.o: hashmap.c hashmap.h
	clang -c -o hashmap.o hashmap.c

test.o: test.c hashmap.h
	clang -c -o test.o test.c

.PHONY: clean

clean:
	rm -rf test
	rm -rf hashmap.o
	rm -rf test.o
