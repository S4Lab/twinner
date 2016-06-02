rm -f /tmp/twinner/*
./obj-ia32/Twinner --input ./obj-ia32/mpwpl-manual-virt-obf.out --args '475' --pin-launcher ../../../pin.exe --tool ./obj-ia32/TwinTool.dll -o /tmp/twinner/unpacked.twin.c --safe-functions 'printf@0x401614#auto' --analysis-endpoints 0x4012d9,0x40135d --verbose $@
