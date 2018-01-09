cmd="../../../pin.sh -pin_memory_range 0x400000000:0x600000000 -t ./obj-intel64/TwinTool.so -symbols ./tmp/twinner/symbols.dat -trace ./tmp/twinner/trace.dat -memory ./tmp/twinner/memory.dat -verbose $@ -logfilename out-input3-twintool -main -mar ./tmp/twinner/main-args-reporting.dat -safe-functions 'puts@0x400450#auto,printf@0x4006e0#auto,strcmp@0x400740#2{const char *^s!const char *^s}' -- ./obj-intel64/input3.out --left 3 --right 5"
echo "Running \"$cmd\""
sudo su sandbox -c "$cmd"
