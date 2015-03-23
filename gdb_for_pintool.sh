pid=$1
if [ "a$pid" = "a" ]; then
  echo "Run ./run_paused_twintool.sh <logging-level> in another terminal"
  echo "and then use the printed PID as the only argument of this script"
  exit -1;
fi
sudo su sandbox -c "gdb /home/behnam/University/Phd/BMPhdThesis/tools/pin-2.14-linux/intel64/bin/pinbin $pid"
