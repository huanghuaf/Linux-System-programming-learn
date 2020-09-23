#!/bin/sh

MAX_SIZE=20000000
MON_FILE=./$(uname -n).txt
kmemleak_enable=false
i=0

while true
do
	i=`expr $i + 1`
	echo $i

	# memory information
	date >> $MON_FILE
	cat /proc/meminfo >> $MON_FILE

	echo "----------------------" >> $MON_FILE
	cat /proc/slabinfo >> $MON_FILE

	echo "----------------------" >> $MON_FILE
	 ps -eo pid,comm,stat,time,rss,vsz >> $MON_FILE

	echo "++++++++++++++++++++++" >> $MON_FILE
	fsize=`ls -l $MON_FILE | awk '{print $5}'`
	if [ $fsize -gt $MAX_SIZE ]; then
		# upload file to TFTP server
		suffix=`cat /proc/uptime | cut -d" " -f1`
		mv $MON_FILE $MON_FILE.$suffix
		# upload to cloud
	fi

	 # kmemleak
	if [ "$kmemleak_enable" = true ]; then
		echo "kmemleak"
		kmemleak_path=/sys/kernel/debug/kmemleak
		if [ -r $kmemleak_path ]; then
			cat $kmemleak_path > ./kmemleak_$(uname -n).out
		else
			echo "No $kmemleak_path"
		fi
	fi

	sleep 3
done
