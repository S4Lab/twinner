cp -l -a -u src/* "deploy/"
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
