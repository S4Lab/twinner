sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/input13.out --args 'RDF67 right extra' --number-of-traces 25 --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o ./tmp/twinner/input13.twin.c --tmpfolder ./tmp --safe-functions 'puts@0x400450#auto,strcmp@0x400470#auto' --generate-etg ./tmp/twinner/input13.twin.etg.dot --main --verbose $@"
