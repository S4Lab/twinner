fullpreparation=$1

cp -l -a -u src/* "deploy/"

if [ "a$fullpreparation" = "afull" ]; then
	ptracescope=$(cat /proc/sys/kernel/yama/ptrace_scope)
	aslrstate=$(cat /proc/sys/kernel/randomize_va_space)
	if [ ! "$ptracescope$aslrstate" = "00" ]; then
		if [ ! "$ptracescope" = "0" ]; then
			echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope > /dev/null
		fi
		if [ ! "$aslrstate" = "0" ]; then
			echo 0 | sudo tee /proc/sys/kernel/randomize_va_space > /dev/null
		fi
		sudo -k
	fi
	mkdir -p /tmp/twinner
fi

