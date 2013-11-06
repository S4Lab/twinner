#!/usr/bin/env bash 

echo -e "\e[0;31m"
echo "Make sure to run as root, otherwise it won't gaurantee your saftey :)"
echo -e "\e[0m"
## Print header
echo -e "Size\tResid.\tShared\tData\t%"
while [ 1 ]; do
	## Get the PID of the process name given as argument 1
	if [ "a$1" = "a" ]; then
		pidno=`pgrep -u sandbox`
	else
		pidno=`pgrep $1`
	fi
	## If the process is running, print the memory usage
	if [ -e "/proc/$pidno/statm" ]; then
		## Get the memory info
		m=`awk '{OFS="\t";print $1,$2,$3,$6}' /proc/$pidno/statm`
		## Get the memory percentage
		perc=`top -bd .10 -p $pidno -n 1  | grep $pidno | gawk '{print \$10}'`
		## print the results
		echo -e "$m\t$perc";
		val=`echo $m | cut -d\  -f4`
		if [ $val -gt 450000 ]; then
			echo "Memory usage is more than 450,000. Killing..."
			kill -9 $pidno
			exit -1;
		fi
		## If the process is not running
	else
		echo "$1 is not running";
		sleep 1;
	fi
done

