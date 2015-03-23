
INCLUDEPATH := -I ../src/res/

TARGETS := simple-condition-test \
	simple-condition-test.twin \
	assembly-mov-sizes \
	registers-state-manipulation \
	multi-path-with-printf-leafs-test \
	upx-test upx-test.upx-packed \
	overhead-measurement-test
TWIN_TARGETS := simple-condition-test \
	multi-path-with-printf-leafs-test \
	#upx-test

all: $(TARGETS:=.out) $(TWIN_TARGETS:=.twin.out)

%.twin.out: %.twin.c
	g++ -std=c++11 -g $(INCLUDEPATH) $< -o $@

%.s: %.c
	gcc -g $(INCLUDEPATH) -S $<

%.out: %.s
	gcc -g $< -o $@

upx-test.out: upx-test.c
	gcc -g -static -o upx-test.out upx-test.c

upx-test.upx-packed.out: upx-test.out
	rm upx-test.upx-packed.out && upx --strip-relocs=0 -o upx-test.upx-packed.out upx-test.out

expression-operations-concrete-value-test.out: expression-operations-concrete-value-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o expression-operations-concrete-value-test.o expression-operations-concrete-value-test.cpp
	g++ -o expression-operations-concrete-value-test.out expression-operations-concrete-value-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/search/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o ../deploy/obj-intel64/edu/sharif/twinner/exception/AbstractException.o -lcvc4 -lgmp

constraint-simplification-with-cvc4-test.out: constraint-simplification-with-cvc4-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o constraint-simplification-with-cvc4-test.o constraint-simplification-with-cvc4-test.cpp
	g++ -o constraint-simplification-with-cvc4-test.out constraint-simplification-with-cvc4-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/search/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o ../deploy/obj-intel64/edu/sharif/twinner/exception/AbstractException.o -lcvc4 -lgmp

remainder-having-constraint-test.out: remainder-having-constraint-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o remainder-having-constraint-test.o remainder-having-constraint-test.cpp
	g++ -o remainder-having-constraint-test.out remainder-having-constraint-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/search/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o ../deploy/obj-intel64/edu/sharif/twinner/exception/AbstractException.o -lcvc4 -lgmp

shift-left-and-bitwise-and-or-runtime-simplification.out: shift-left-and-bitwise-and-or-runtime-simplification.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o shift-left-and-bitwise-and-or-runtime-simplification.o shift-left-and-bitwise-and-or-runtime-simplification.cpp
	g++ -o shift-left-and-bitwise-and-or-runtime-simplification.out shift-left-and-bitwise-and-or-runtime-simplification.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/cv/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/exptoken/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/search/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o ../deploy/obj-intel64/edu/sharif/twinner/exception/AbstractException.o -lcvc4 -lgmp

shift-right-runtime-simplification.out: shift-right-runtime-simplification.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o shift-right-runtime-simplification.o shift-right-runtime-simplification.cpp
	g++ -o shift-right-runtime-simplification.out shift-right-runtime-simplification.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/cv/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/exptoken/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/search/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o ../deploy/obj-intel64/edu/sharif/twinner/exception/AbstractException.o -lcvc4 -lgmp

clean:
	rm -f *.out *~

.PHONY: all clean
.PRECIOUS: %.s
