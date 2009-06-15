#!/bin/sh

~cs470s/bzflag/src/bzflag/bzflag -directory /users/ta/cs470s/bzflag/data -window 1024x768 &
gnome-terminal -x ~cs470s/bzflag/src/bzfs/bzfs -c -d -noTeamKills -set _inertiaLinear 1 -set _inertiaAngular 1 -set _tankAngVel 0.5 -set _explodeTime 300 -set _rejoinTime 300 -set _grabOwnFlag 0 -ms 3 -worldsize 400 &
read -p "Press any key to continue (when the bzfs is setup)"


gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 2 -p 3000 -posnoise 3 Doormats_imdumb@saber &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 2 -p 3001 -posnoise 3 Doormats_imstupdi@saber  &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 3 -p 3002 -posnoise 3 Doormats_yourstupid@saber  &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 3 -p 3003 -posnoise 3 Doormats_ieatcookiesallday@saber  &


gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9008 -posnoise 3 badguys00@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9009 -posnoise 3 badguys01@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9010 -posnoise 3 badguys02@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9011 -posnoise 3 badguys03@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9012 -posnoise 3 badguys04@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9013 -posnoise 3 badguys05@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9014 -posnoise 3 badguys06@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9015 -posnoise 3 badguys07@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9016 -posnoise 3 badguys08@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 1 -p 9017 -posnoise 3 badguys09@mek.com &

