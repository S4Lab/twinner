rm -f /tmp/twinner/*
./obj-ia32/Twinner --input ./obj-ia32/mpwpl-manual-virt-obf.out --args '475' --pin-launcher ../../../pin.exe --tool ./obj-ia32/TwinTool.dll -o /tmp/twinner/unpacked.twin.c --main --verbose $@
