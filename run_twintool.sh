cmd="../../../pin.sh -pin_memory_range 0x400000000:0x600000000 -t ./obj-intel64/TwinTool.so -symbols ./tmp/twinner/symbols.dat -trace ./tmp/twinner/trace.dat -memory ./tmp/twinner/memory.dat -verbose $@ -logfilename out-input1-vo-twintool -main -mar ./tmp/twinner/main-args-reporting.dat -safe-functions 'puts@0x400410#auto' -- ./obj-intel64/input1.vo.out 475"
echo "Running \"$cmd\""
sudo su sandbox -c "$cmd"
