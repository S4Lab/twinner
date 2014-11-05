
INCLUDEPATH := -I ../src/res/

TARGETS := simple-condition-test \
	simple-condition-test.twin \
	assembly-mov-sizes \
	registers-state-manipulation \
	multi-path-with-printf-leafs-test
TWIN_TARGETS := simple-condition-test \
	multi-path-with-printf-leafs-test

all: $(TARGETS:=.out) $(TWIN_TARGETS:=.twin.out)

%.twin.out: %.twin.c
	g++ -std=c++11 -g $(INCLUDEPATH) $< -o $@

%.s: %.c
	gcc -g $(INCLUDEPATH) -S $<

%.out: %.s
	gcc -g $< -o $@

expression-operations-concrete-value-test.out: expression-operations-concrete-value-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o expression-operations-concrete-value-test.o expression-operations-concrete-value-test.cpp
	g++ -o expression-operations-concrete-value-test.out expression-operations-concrete-value-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/search/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o ../deploy/obj-intel64/edu/sharif/twinner/exception/AbstractException.o -lcvc4 -lgmp

constraint-simplification-with-cvc4-test.out: constraint-simplification-with-cvc4-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o constraint-simplification-with-cvc4-test.o constraint-simplification-with-cvc4-test.cpp
	g++ -o constraint-simplification-with-cvc4-test.out constraint-simplification-with-cvc4-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/search/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o ../deploy/obj-intel64/edu/sharif/twinner/exception/AbstractException.o -lcvc4 -lgmp

remainder-having-constraint-test.out: remainder-having-constraint-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o remainder-having-constraint-test.o remainder-having-constraint-test.cpp
	g++ -o remainder-having-constraint-test.out remainder-having-constraint-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/search/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o ../deploy/obj-intel64/edu/sharif/twinner/exception/AbstractException.o -lcvc4 -lgmp

clean:
	rm -f *.out *~

.PHONY: all clean
.PRECIOUS: %.s
