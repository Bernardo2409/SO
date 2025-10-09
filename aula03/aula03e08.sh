#!/bin/bash 
# select structure to create menus 

PS3="Choose the number you want: "
select arg in $@ 
do 
    if [[ -z "$arg" ]];
        then
            echo "Invalid Option"
            break
    else
        echo "You picked $arg ($REPLY)." 
    fi
done 