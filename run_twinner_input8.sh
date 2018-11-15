sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/input8.out --args 'U R F verbose' --number-of-traces 20 --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o ./tmp/twinner/input8.twin.c --tmpfolder ./tmp --safe-functions 'puts@0x400450#auto,strcmp@0x400470#auto' --generate-etg ./tmp/twinner/input8.twin.etg.dot --main --verbose $@"
