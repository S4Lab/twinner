#sudo su sandbox -c "rm -f ./tmp/twinner/*"
sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/input1.vo.out --args '475' --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o ./tmp/twinner/unpacked.twin.c --tmpfolder ./tmp --safe-functions 'puts@0x400410#auto,printf@0x4006a0#auto' --generate-etg ./tmp/twinner/etg.dot --main --verbose $@"
#sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/runtime-simplification.out --args '2' --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o /tmp/twinner/unpacked.twin.c --safe-functions 'printf@0x400460#auto,puts@0x400450#1{const char *}' --main --verbose $@"
#sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/runtime-simplification.out --args '2' --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o /tmp/twinner/unpacked.twin.c --safe-functions 'printf@0x400460#auto,puts@0x400450#auto' --main --verbose $@"
