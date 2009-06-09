#!/bin/sh

gnome-terminal -x ~cs470s/bzflag/src/bzfs/bzfs -c -d -noTeamKills -set _inertiaLinear 1 -set _inertiaAngular 1 -set _tankAngVel 0.5 -set _explodeTime 0 -set _rejoinTime 0 -set _grabOwnFlag 0 -world empty.bzw -ms 2 -worldsize 200 &
read -p "Press any key to continue (when the bzfs is setup)"
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 6000 &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6005 -posnoise 0 badguys@mek.com &




