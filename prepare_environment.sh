cp -l -a -u src/* "../pin-2.13-61206-gcc.4.4.7-linux/source/tools/Twinner/"
ptracescope=$(cat /proc/sys/kernel/yama/ptrace_scope)
if [ ! "$ptracescope" = "0" ]; then
	echo 0 | sudo -k tee /proc/sys/kernel/yama/ptrace_scope > /dev/null
fi
