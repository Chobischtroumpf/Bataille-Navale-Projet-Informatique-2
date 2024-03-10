#!/bin/bash

array=()
for i in {a..z} {A..Z} {0..9}; 
   do
   array[$RANDOM]=$i
done
printf %s ${array[@]::8} $'\n'