#!/bin/bash 
# Calculate the sum of a series of numbers. 
SCORE="0" 
SUM="0" 
COUNTER="0" 

while true 
do 
    echo -n "Enter your score [0-10] ('q' to quit): " 
    read SCORE; 
    if (("$SCORE" < "0"))  || (("$SCORE" > "10")); then 
        echo "Try again: " 
    elif [[ "$SCORE" == "q" ]] 
    then 
        echo "Sum: $SUM." 
        echo "Media : $((SUM / COUNTER))"
        break 
    else 
        
        COUNTER=$((COUNTER+1))
        SUM=$((SUM + SCORE)) 
    fi 
done 
echo "Exiting."