
all: simple-condition-test.out assembly-mov-sizes.out


simple-condition-test.s: simple-condition-test.c
	gcc -S simple-condition-test.c

assembly-mov-sizes.s: assembly-mov-sizes.c
	gcc -S assembly-mov-sizes.c

simple-condition-test.out: simple-condition-test.s
	gcc simple-condition-test.s -o simple-condition-test.out

assembly-mov-sizes.out: assembly-mov-sizes.s
	gcc assembly-mov-sizes.s -o assembly-mov-sizes.out

clean:
	rm *.out *~

.PHONY: all clean

