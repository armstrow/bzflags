#!/bin/sh

python ./sync.py localhost 6000 &

firstpart='\'cd; cd bzflags; ./bin/robotter'
host=nunchucks
secondpart='0 "move_sniper"\' '




first="$echo $firstpart $host 6008 $secondpart"
gnome-terminal -x ssh pike $first &

second="$echo $firstpart $host 6009 $secondpart"
gnome-terminal -x ssh pike $second &

third="$echo $firstpart $host 6010 $secondpart"
gnome-terminal -x ssh pike $third &

fourth="$echo $firstpart $host 6011 $secondpart"
gnome-terminal -x ssh pike $fourth &

fifth="$echo $firstpart $host 6012 $secondpart"
gnome-terminal -x ssh pike $fifth &

sixth="$echo $firstpart $host 6013 $secondpart"
gnome-terminal -x ssh pike $sixth &

seventh="$echo $firstpart $host 6014 $secondpart"
gnome-terminal -x ssh pike $seventh &

eigth="$echo $firstpart $host 6015 $secondpart"
gnome-terminal -x ssh pike $eigth &

ninth="$echo $firstpart $host 6016 $secondpart"
gnome-terminal -x ssh pike $ninth &

tenth="$echo $firstpart $host 6017 $secondpart"
gnome-terminal -x ssh pike $tenth &



