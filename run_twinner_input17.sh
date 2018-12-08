sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/input17.out --args 'verbose UUUR2 L magicvalue SSS' --number-of-traces 25 --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o ./tmp/twinner/input17.twin.c --tmpfolder ./tmp --safe-functions 'puts@0x400450#auto,strcmp@0x400470#auto' --generate-etg ./tmp/twinner/input17.twin.etg.dot --main --verbose $@"