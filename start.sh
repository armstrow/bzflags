#!/bin/sh

bin=./bin/robotter
host=localhost
num=0
type=move_sniper


port=7008
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &

sleep 1

port=7009
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &

sleep 1

port=7010
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &

sleep 1

port=7011
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &

sleep 1

port=7012
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &

sleep 2

port=7013
type=sniper
gnome-terminal -x $bin $host $port $num $type &

sleep 2

port=7014
type=sniper
gnome-terminal -x $bin $host $port $num $type &

sleep 1

port=7015
type=move_sniper
gnome-terminal -x $bin $host $port $num $type &

sleep 1

port=7016
type=travel
gnome-terminal -x $bin $host $port $num $type &

sleep 1

port=7017
type=travel
gnome-terminal -x $bin $host $port $num $type &



