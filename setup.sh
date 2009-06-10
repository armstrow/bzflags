#!/bin/sh

~cs470s/bzflag/src/bzflag/bzflag -directory /users/ta/cs470s/bzflag/data -window 1024x768 &
gnome-terminal -x ~cs470s/bzflag/src/bzfs/bzfs -c -d -noTeamKills -set _inertiaLinear 1 -set _inertiaAngular 1 -set _tankAngVel 0.5 -set _explodeTime 0 -set _rejoinTime 0 -set _grabOwnFlag 0 -world empty2.bzw -ms 2 -worldsize 400 &
read -p "Press any key to continue (when the bzfs is setup)"
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 6000 &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 10 -p 6005 -posnoise 3 badguys@mek.com &




