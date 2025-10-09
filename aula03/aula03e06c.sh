#!/bin/bash

#until
i=1
until [ $i -gt 5 ]
do
   echo $i
   i=$((i+1))
done


#for (seq)
for i in $(seq 1 5)
do
   echo $i
done

#for (C)

for (( i=1; i<=5; i++ ))
do
   echo $i
done