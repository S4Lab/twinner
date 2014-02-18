fullpreparation=$1

echo "Linking to source files from deploy folder..."
cp -f -l -a -u mem.sh run_twinner.sh run_twintool.sh src/* "deploy/"
cp -f -l -a -u test/simple-condition-test.out deploy/obj-intel64/

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
		echo "Disabling swap for memory loop safty..."
		sudo swapoff -a
		sudo -k
	fi
	sudo su sandbox -c 'mkdir -p /tmp/twinner'
fi
echo "Done."

