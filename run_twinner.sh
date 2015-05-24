sudo su sandbox -c "rm -f /tmp/twinner/*"
sudo su sandbox -c "./obj-intel64/Twinner --input ./obj-intel64/multi-path-with-printf-leafs-test.out --args '475' --pin-launcher ../../../pin.sh --tool ./obj-intel64/TwinTool.so -o /tmp/twinner/unpacked.twin.c --main --verbose $@"
