
all: twintool twinner

twintool:
	make -f twintool.makefile twintool

twinner:
	make -f twinner.makefile twinner

clean:
	rm -rf {obj-ia32,obj-intel64}/{edu,Trace.*,Concolic.*,Twinner*,TwinTool.*}

.PHONY: all twintool twinner
