sudo su sandbox -c "../../../pin.sh -t ./obj-intel64/TwinTool.so -symbols /tmp/twinner/symbols.dat -trace /tmp/twinner/trace.dat -verbose $1 -main -- ./obj-intel64/simple-condition-test.out abc def"
