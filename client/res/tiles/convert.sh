#!/bin/bash

i=0
j=0
cmd=""
for file in $(ls *.png)
do 
	
	if (( i == 0 ))
       	then 
		cmd="convert +append"
       	fi 
	cmd="$cmd $file"


       	if (( i == 2 ))
	then 
		cmd="$cmd block/out_$j.png"
		$($cmd)
		j=$(( j + 1 ))
		i=0
       	else 
		i=$(( i + 1 ))
	fi
done

if [ "$cmd" != "" ]
then
	cmd="$cmd block/out_$j.png"
	$($cmd)
fi

cmd="convert -append"
for ((i=0;i<=$j;i++))
do
	cmd="$cmd block/out_$i.png"
done

if (( j > 0 ))
then
	cmd="$cmd block/out_full.png"
	$($cmd)
fi
