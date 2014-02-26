
INCLUDEPATH := -I ../src/res/

TARGETS := simple-condition-test \
	simple-condition-test.twin \
	assembly-mov-sizes \
	registers-state-manipulation \
	multi-path-with-printf-leafs-test

all: $(TARGETS:=.out)


%.s: %.c
	gcc -g $(INCLUDEPATH) -S $<

%.out: %.s
	gcc -g $< -o $@

clean:
	rm -f *.out *~

.PHONY: all clean
.PRECIOUS: %.s
