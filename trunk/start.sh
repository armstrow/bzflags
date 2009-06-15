#!/bin/sh

bin=./bin/robotter
host=localhost
num=0
type=move_sniper

port=6008
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &


port=6009
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &


'port=6010
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &


port=6011
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &


port=6012
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &


port=6013
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &

port=6014
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &


port=6015
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &


port=6016
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &


port=6017
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &
'

python ./sync.py localhost 6000


