cmd="../../../pin.sh -t ./obj-intel64/TwinTool.so -symbols /tmp/twinner/symbols.dat -trace /tmp/twinner/trace.dat -memory /tmp/twinner/memory.dat -verbose $@ -main -mar /tmp/twinner/main-args-reporting.dat -- ./obj-intel64/mpwpl-manual-virt-obf.out 475"
echo "Running \"$cmd\""
sudo su sandbox -c "$cmd"
