#!/bin/sh

python ./sync.py localhost 3000 &

firstpart='\'./bin/robotter'
host=nunchucks
secondpart='0 "move_sniper"'\'


echo $firstpart $host 3008 $secondpart

first="$echo $firstpart $host 3008 $secondpart"#!/bin/sh



gnome-terminal -x ./bin/robotter localhost 6208 0 "travel" &
gnome-terminal -x ./bin/robotter localhost 6309 1 "travel" &
#gnome-terminal -x ./bin/robotter localhost 3010 2 "decoy" &
#gnome-terminal -x ./bin/robotter localhost 3011 3 "decoy" &
#gnome-terminal -x ./bin/robotter localhost 3012 4 "decoy" &
#gnome-terminal -x ./bin/robotter localhost 1013 5 "decoy" &
#gnome-terminal -x ./bin/robotter localhost 1014 6 "travel" &
#gnome-terminal -x ./bin/robotter localhost 1015 7 "travel" &
#gnome-terminal -x ./bin/robotter localhost 1016 8 "decoy" &
#gnome-terminal -x ./bin/robotter localhost 1017 9 "decoy" &



python ./sync.py localhost 6100


