fullpreparation=$1
echo "AAA-$1-AAA"
echo "Linking to source files from deploy folder..."
cp -f -l -a -u mem.sh run_twinner_win.sh run_paused_twintool.sh gdb_for_pintool.sh src/* "deploy/"
cd deploy/
rm depends.sh
mv depends_win.sh depends.sh
cd ..
#cp -f -l -a -u "test/simple-condition-test.out" "test/multi-path-with-printf-leafs-test.out" "test/mpwpl-manual-virt-obf.out" "deploy/obj-ia32/"
cp -f -l -a -u "test/mpwpl-manual-virt-obf.out" "deploy/obj-ia32/"

if [ "a$fullpreparation" = "afull" ]; then
	echo "Full preparation..."
	mkdir -p /tmp/twinner
	#export LD_LIBRARY_PATH=/home/john/twinner/pin-2.14-linux/intel64/runtime
fi
echo "Done."
#bash
