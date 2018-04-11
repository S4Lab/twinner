fullpreparation=$1

echo "Linking to source files from deploy folder..."
cp -f -l -a -u mem.sh run_*.sh gdb_for_pintool.sh src/* "deploy/"
cp -f -l -a -u "test/simple-condition-test.out" "test/multi-path-with-printf-leafs-test.out" "test/mpwpl-manual-virt-obf.out" "deploy/obj-intel64/"

if [ "a$fullpreparation" = "afull" ]; then
	echo "Full preparation..."
	ptracescope=$(cat /proc/sys/kernel/yama/ptrace_scope)
	aslrstate=$(cat /proc/sys/kernel/randomize_va_space)
	if [ ! "$ptracescope$aslrstate" = "00" ]; then
		if [ ! "$ptracescope" = "0" ]; then
			echo "Disabling yama ptrace protection for parent-child debugging..."
			echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope > /dev/null
		fi
		if [ ! "$aslrstate" = "0" ]; then
			echo "Disabling ASLR for multiple executions..."
			echo 0 | sudo tee /proc/sys/kernel/randomize_va_space > /dev/null
		fi
		echo "Disabling swap for memory loop safety..."
		sudo swapoff -a
		sudo -k
	fi
	sudo su sandbox -c 'mkdir -p /tmp/twinner'
	export LD_LIBRARY_PATH=/home/john/twinner/pin-2.14-linux/intel64/runtime
fi
echo "Done."
bash
