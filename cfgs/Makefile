
CLANG := clang++
CLANG_FLAGS := -S -std=c++11 -emit-llvm

INCLUDEPATH := -I ../src/res/

TARGETS := multi-path-with-printf-leafs-test \
	multi-path-with-printf-leafs-test.twin

all: $(TARGETS:=.cfg.png)

%.cfg.png: %.cfg.dot
	dot -o $@ -Tpng $<

%.cfg.dot: %.ll
	opt -dot-cfg $< >/dev/null
	mv cfg.main.dot  $@

%.ll: %.c
	$(CLANG) $(CLANG_FLAGS) $(INCLUDEPATH) $<

%.ll: ../test/%.c
	$(CLANG) $(CLANG_FLAGS) $(INCLUDEPATH) $<

.PHONEY: all
.PRECIOUS: %.cfg.dot
