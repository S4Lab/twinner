
INCLUDEPATH := -I ../src/res/

TARGETS := simple-condition-test \
	simple-condition-test.twin \
	assembly-mov-sizes \
	registers-state-manipulation \
	multi-path-with-printf-leafs-test
TWIN_TARGETS := multi-path-with-printf-leafs-test

all: $(TARGETS:=.out) $(TWIN_TARGETS:=.twin.out)

%.twin.out: %.twin.c
	g++ -g $(INCLUDEPATH) $< -o $@

%.s: %.c
	gcc -g $(INCLUDEPATH) -S $<

%.out: %.s
	gcc -g $< -o $@

clean:
	rm -f *.out *~

.PHONY: all clean
.PRECIOUS: %.s
