#!/bin/sh
i=1
while [ $i -le 10 ]
do
    nc -l localhost -p 55555
    echo
    echo
done
