#!/bin/sh

python ./sync.py localhost 6000 &

gnome-terminal -x ./bin/robotter localhost 6008 0 "move_sniper" &
gnome-terminal -x ./bin/robotter localhost 6009 1 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6010 3 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6010 3 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6011 4 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6011 4 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6012 5 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6013 6 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6014 7 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6015 8 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6016 9 "move_sniper" &
 gnome-terminal -x ./bin/robotter localhost 6017 10 "move_sniper" 
