#!/usr/bin/env bash 

echo -e "\e[0;31m"
echo "Make sure to run as root, otherwise it won't gaurantee your saftey :)"
echo -e "\e[0m"
## Print header
echo -e "Size\tResid.\tShared\tData\t%"
while [ 1 ]; do
	## Get the PID of the process name given as argument 1
	if [ "a$1" = "a" ]; then
		pidnos=`pgrep -u sandbox`
	else
		pidnos=`pgrep $1`
	fi
	for pidno in $pidnos; do
		## If the process is running, print the memory usage
		if [ -e "/proc/$pidno/statm" ]; then
			# Get the memory info
			m=`awk '{OFS="\t";print $1,$2,$3,$6}' /proc/$pidno/statm`
			## Get the memory percentage
			perc=`top -bd .10 -p $pidno -n 1  | grep $pidno | gawk '{print \$8}'`
			## print the results
			echo -e "[$pidno] $m\t$perc";
			val=`echo $m | cut -d\  -f4`
			memlimit=401000
			if [ $val -gt $memlimit ]; then
				echo "Memory usage of $pidno is more than $memlimit. Killing..."
				kill -9 $pidno
				exit -1;
			fi
			## If the process is not running
		else
			echo "$1 is not running";
		fi
	done
	sleep 1;
done

