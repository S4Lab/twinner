args="-pin_memory_range 0x40000000:0x60000000 -t ./obj-ia32/TwinTool.dll -symbols tmp/twinner/symbols.dat -trace tmp/twinner/trace.dat -memory tmp/twinner/memory.dat -verbose $@ -logfilename out-twintool -endpoints 0x4012d9,0x4012ef -stack-offset 11 -mar tmp/twinner/main-args-reporting.dat -safe-functions printf@0x401308#auto,puts@0x401310#auto -- ./obj-ia32/multi-path-with-printf-leafs-test-annotated.vmp.exe 123"
echo "Running \"../../../pin.exe $args\""
../../../pin.exe $args
