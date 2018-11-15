sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/input5.vo.out --args '--opt val 12' --number-of-traces 15 --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o ./tmp/twinner/input5.vo.twin.c --tmpfolder ./tmp --safe-functions 'printf@0x400820#auto,strcmp@0x400880#auto' --generate-etg ./tmp/twinner/input5.vo.twin.etg.dot --main --verbose $@"
