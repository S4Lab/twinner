OPTIONS := -g
BINARIES := input0.out input0.vo.out input1.out input1.vo.out input2.out input2.vo.out input3.out input3.vo.out input4.out input4.vo.out input5.out input5.vo.out input6.out input6.vo.out input7.out input7.vo.out input8.out input8.vo.out input9.out input9.vo.out input10.out input10.vo.out input11.out input11.vo.out input12.out input12.vo.out input13.out input13.vo.out input14.out input14.vo.out input15.out input15.vo.out input16.out input16.vo.out input17.out input17.vo.out input18.out input18.vo.out input19.out input19.vo.out

INCLUDEPATH := -I ../src/res/

TARGETS := simple-condition-test \
	simple-condition-test.twin \
	assembly-mov-sizes \
	registers-state-manipulation \
	multi-path-with-printf-leafs-test \
	overhead-measurement-test \
	expression-addition-runtime-simplification \
	mpwpl-manual-virt-obf \
	bitwise-and-simplification \
	runtime-simplification
TWIN_TARGETS := simple-condition-test \
	multi-path-with-printf-leafs-test \
	mpwpl-manual-virt-obf \
	input0 input0.vo \
	input1 input1.vo \
	input2 input2.vo \
	input3 input3.vo \
	input4 input4.vo \
	#upx-test

all: $(BINARIES) $(TARGETS:=.out) $(TWIN_TARGETS:=.twin.out)

input%.vo.out: input%.vo.o vo-interpreter.o
	$(CXX) $(OPTIONS) $^ -o $@

%.o: %.c
	$(CXX) $(OPTIONS) $< -c -o $@

input%.twin.out: input%.twin.c
	g++ -std=c++11 -g $(INCLUDEPATH) $< -o $@

%.out: %.o
	g++ --std=c++11 $< -o $@

runtime-simplification.out: runtime-simplification.s
	gcc runtime-simplification.s -o runtime-simplification.out

mpwpl-manual-virt-obf.out: mpwpl-manual-virt-obf.c
	g++ -c -g -Wa,-a,-ad --std=c++11 mpwpl-manual-virt-obf.c > mpwpl-manual-virt-obf.lst
	g++ --std=c++11 mpwpl-manual-virt-obf.c -o mpwpl-manual-virt-obf.out

%.win32.twin.out: %.win32.twin.c
	g++ -std=c++11 -DARCHITECTURE_32 -g $(INCLUDEPATH) $< -o $@

%.twin.out: %.twin.c
	g++ -std=c++11 -g $(INCLUDEPATH) $< -o $@

%.s: %.c
	gcc -g $(INCLUDEPATH) -S $<

%.o: %.s
	gcc -g $< -c -o $@

upx-test.out: upx-test.c
	gcc -g -static -o upx-test.out upx-test.c

upx-test.upx-packed.out: upx-test.out
	rm upx-test.upx-packed.out && upx --strip-relocs=0 -o upx-test.upx-packed.out upx-test.out

expression-addition-runtime-simplification.out: expression-addition-runtime-simplification.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../pin-2.14-linux/source/include/pin -I../pin-2.14-linux/source/include/pin/gen -I../pin-2.14-linux/extras/components/include -I../pin-2.14-linux/extras/xed2-intel64/include -I../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o expression-addition-runtime-simplification.o expression-addition-runtime-simplification.cpp
	g++ -o expression-addition-runtime-simplification.out expression-addition-runtime-simplification.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/cv/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/exptoken/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/syscall/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/encoder/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o -lcvc4 -lgmp -lcln

bitwise-and-simplification.out: bitwise-and-simplification.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../pin-2.14-linux/source/include/pin -I../pin-2.14-linux/source/include/pin/gen -I../pin-2.14-linux/extras/components/include -I../pin-2.14-linux/extras/xed2-intel64/include -I../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o bitwise-and-simplification.o bitwise-and-simplification.cpp
	g++ -o bitwise-and-simplification.out bitwise-and-simplification.o $(shell find ../deploy/obj-intel64/edu/sharif/twinner/engine/ ../deploy/obj-intel64/edu/sharif/twinner/trace/ ../deploy/obj-intel64/edu/sharif/twinner/util/ -name \*.o) -lcvc4 -lgmp -lcln

expression-operations-concrete-value-test.out: expression-operations-concrete-value-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o expression-operations-concrete-value-test.o expression-operations-concrete-value-test.cpp
	g++ -o expression-operations-concrete-value-test.out expression-operations-concrete-value-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/encoder/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o -lcvc4 -lgmp -lcln

constraint-simplification-with-cvc4-test.out: constraint-simplification-with-cvc4-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o constraint-simplification-with-cvc4-test.o constraint-simplification-with-cvc4-test.cpp
	g++ -o constraint-simplification-with-cvc4-test.out constraint-simplification-with-cvc4-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/encoder/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o -lcvc4 -lgmp -lcln

constraint-validation-with-cvc4-test.out: constraint-validation-with-cvc4-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o constraint-validation-with-cvc4-test.o constraint-validation-with-cvc4-test.cpp
	g++ -o constraint-validation-with-cvc4-test.out constraint-validation-with-cvc4-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/cv/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/exptoken/*.o ../deploy/obj-intel64/edu/sharif/twinner/trace/syscall/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/encoder/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o -lcvc4 -lgmp -lcln

remainder-having-constraint-test.out: remainder-having-constraint-test.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o remainder-having-constraint-test.o remainder-having-constraint-test.cpp
	g++ -o remainder-having-constraint-test.out remainder-having-constraint-test.o ../deploy/obj-intel64/edu/sharif/twinner/trace/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/smt/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/*.o ../deploy/obj-intel64/edu/sharif/twinner/engine/etg/encoder/*.o ../deploy/obj-intel64/edu/sharif/twinner/util/*.o -lcvc4 -lgmp -lcln

shift-left-and-bitwise-and-or-runtime-simplification.out: shift-left-and-bitwise-and-or-runtime-simplification.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o shift-left-and-bitwise-and-or-runtime-simplification.o shift-left-and-bitwise-and-or-runtime-simplification.cpp
	g++ -o shift-left-and-bitwise-and-or-runtime-simplification.out shift-left-and-bitwise-and-or-runtime-simplification.o $(shell find ../deploy/obj-intel64/edu/sharif/twinner/engine/ ../deploy/obj-intel64/edu/sharif/twinner/trace/ ../deploy/obj-intel64/edu/sharif/twinner/util/ -name \*.o) -lcvc4 -lgmp -lcln

shift-right-runtime-simplification.out: shift-right-runtime-simplification.cpp
	g++ -DBIGARRAY_MULTIPLIER=1 -Wall -Werror -Wno-unknown-pragmas -fno-stack-protector -DTARGET_IA32E -DHOST_IA32E -fPIC -DTARGET_LINUX -I../../pin-2.14-linux/source/include/pin -I../../pin-2.14-linux/source/include/pin/gen -I../../pin-2.14-linux/extras/components/include -I../../pin-2.14-linux/extras/xed2-intel64/include -I../../pin-2.14-linux/source/tools/InstLib -O3 -fomit-frame-pointer -fno-strict-aliasing -I../deploy -c -o shift-right-runtime-simplification.o shift-right-runtime-simplification.cpp
	g++ -o shift-right-runtime-simplification.out shift-right-runtime-simplification.o $(shell find ../deploy/obj-intel64/edu/sharif/twinner/engine/ ../deploy/obj-intel64/edu/sharif/twinner/trace/ ../deploy/obj-intel64/edu/sharif/twinner/util/ -name \*.o) -lcvc4 -lgmp -lcln

clean:
	rm -f *.out *~

.PHONY: all clean
.PRECIOUS: %.s
