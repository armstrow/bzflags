#!/bin/sh

~cs470s/bzflag/src/bzflag/bzflag -directory /users/ta/cs470s/bzflag/data -window 1024x768 &
gnome-terminal -x ~cs470s/bzflag/src/bzfs/bzfs -c -d -noTeamKills -set _inertiaLinear 1 -set _inertiaAngular 1 -set _tankAngVel 0.5 -set _explodeTime 300 -set _rejoinTime 300 -set _grabOwnFlag 0 -world four_ls.bzw -ms 3 -worldsize 400 &
read -p "Press any key to continue (when the bzfs is setup)"
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team green -solo 14 -p 6000 -posnoise 50 &


gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6008 -posnoise 3 badguys00@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6009 -posnoise 3 badguys01@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6010 -posnoise 3 badguys02@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6011 -posnoise 3 badguys03@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6012 -posnoise 3 badguys04@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6013 -posnoise 3 badguys05@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6014 -posnoise 3 badguys06@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6015 -posnoise 3 badguys07@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6016 -posnoise 3 badguys08@mek.com &
gnome-terminal -x ~cs470s/bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 6017 -posnoise 3 badguys09@mek.com &

