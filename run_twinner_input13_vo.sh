sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/input13.vo.out --args 'RDF67 right extra' --number-of-traces 25 --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o ./tmp/twinner/input13.vo.twin.c --tmpfolder ./tmp --safe-functions 'printf@0x400820#auto,strcmp@0x400880#auto' --generate-etg ./tmp/twinner/input13.vo.twin.etg.dot --main --verbose $@"