
CLANG := clang++
CLANG_FLAGS := -S -std=c++11 -emit-llvm

INCLUDEPATH := -I ../src/res/

TARGETS := multi-path-with-printf-leafs-test \
	multi-path-with-printf-leafs-test.twin \
	mpwpl-manual-virt-obf \
	mpwpl-manual-virt-obf.twin

all: $(TARGETS:=.cfg.png)

%.cfg.png: %.cfg.dot
	nice dot -o $@ -Tpng $<

%.etg.png: %.etg.dot
	nice dot -o $@ -Tpng $<

%.etg.svg: %.etg.dot
	nice dot -o $@ -Tsvg $<

%.cfg.dot: %.ll
	opt -dot-cfg $< >/dev/null
	# use other opt passes to optimise the graph
	sed -i 's/,label=".*"\]/,label="{l1|{<s0>T|<s1>F}}"\]/' cfg.main.dot
	mv cfg.main.dot $@

%.ll: %.c
	$(CLANG) $(CLANG_FLAGS) $(INCLUDEPATH) $<

%.ll: ../test/%.c
	$(CLANG) $(CLANG_FLAGS) $(INCLUDEPATH) $<

.PHONEY: all
.PRECIOUS: %.cfg.dot
