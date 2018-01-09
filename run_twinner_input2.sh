sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/input2.out --args '--wrongopt wrongvalue' --number-of-traces 6 --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o ./tmp/twinner/unpacked.twin.c --tmpfolder ./tmp --safe-functions 'puts@0x400450#auto,strcmp@0x400470#2{const char *^s!const char *^s}' --generate-etg ./tmp/twinner/etg.dot --main --verbose $@"
