args="-pin_memory_range 0x40000000:0x60000000 -t ./obj-ia32/TwinTool.dll -symbols tmp/twinner/symbols.dat -trace tmp/twinner/trace.dat -memory tmp/twinner/memory.dat -verbose $@ -logfilename out-twintool -endpoints 0x40124c,0x4012c2 -stack-offset 11 -mar tmp/twinner/main-args-reporting.dat -safe-functions printf@0x4015e4#auto -- ./obj-ia32/input1.vo.out 916"
echo "Running \"../../../pin.exe $args\""
../../../pin.exe $args
