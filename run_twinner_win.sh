rm -f /tmp/twinner/*
./obj-ia32/Twinner --input ./obj-ia32/input1.vo.out --args '916' --pin-launcher ../../../pin.exe --tool ./obj-ia32/TwinTool.dll -o tmp/twinner/unpacked.twin.c --generate-etg tmp/twinner/input1.etg.dot --record replay --safe-functions 'printf@0x4015e4#auto' --tmpfolder tmp --analysis-endpoints 0x40124c,0x4012c2 --stack-offset 11 --verbose $@
