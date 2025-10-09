#!/bin/bash 
#This script does a very simple test for checking disk space. 

# Obter informação da partição com maior ocupação
partition_info=$(df -h | grep -v "Use%" | awk '{print $5 " " $1}' | grep % | sort -nr | head -1)
space=$(echo $partition_info | cut -d% -f1)
partition=$(echo $partition_info | awk '{print $2}')

echo "largest occupied space = $space% on partition $partition" 
case $space in 
    [0-6][0-9] )  # espaço < 70% 
      Message="All OK. Partition $partition is at $space%." 
      ;; 
    [7-8][0-9] )  # 70% <= espaço < 90% 
      Message="Cleaning out. Partition $partition is $space% full." 
      ;; 
    9[0-8] )  # 90% <= espaço < 99% 
      Message="Better buy a new disk. Partition $partition is $space% full." 
      ;; 
    99 )  # espaço = 99% 
      Message="I'm drowning here! Partition $partition is at $space%!" 
      ;; 
    * ) 
      Message="I seem to be running with a non-existent disk..." 
      ;; 
esac 
echo $Message 