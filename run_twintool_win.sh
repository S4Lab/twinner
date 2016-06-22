cmd="../../../pin.exe -pin_memory_range 0x40000000:0x60000000 -t ./obj-ia32/TwinTool.dll -symbols /tmp/twinner/symbols.dat -trace /tmp/twinner/trace.dat -memory /tmp/twinner/memory.dat -verbose $@ -endpoints 0x4012e7,0x40135d -stack-offset 11 -mar /tmp/twinner/main-args-reporting.dat -safe-functions printf@0x401614#auto -- ./obj-ia32/mpwpl-manual-virt-obf.out 475"
echo "Running \"$cmd\""
$cmd
