#!/bin/sh

../bzflag/src/bzflag/bzflag -directory ../bzflag/data -window 1024x768 &
gnome-terminal -x ../bzflag/src/bzfs/bzfs -c -d -noTeamKills -set _inertiaLinear 1 -set _inertiaAngular 1 -set _tankAngVel 0.5 -set _explodeTime 300 -set _rejoinTime 300 -set _grabOwnFlag 0 -world four_ls.bzw -ms 3 -worldsize 400 &
read -p "Press any key to continue (when the bzfs is setup)"
gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team green -solo 7 -p 5100 -posnoise 50 &


gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 3 -p 5208 -posnoise 3 badguys00@mek.com &
gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 2 -p 5309 -posnoise 3 badguys01@mek.com &
#gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 3010 -posnoise 3 badguys02@mek.com &
#gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 3011 -posnoise 3 badguys03@mek.com &
#gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 3012 -posnoise 3 badguys04@mek.com &
#gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 1013 -posnoise 3 badguys05@mek.com &
#gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 1014 -posnoise 3 badguys06@mek.com &
#gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 1015 -posnoise 3 badguys07@mek.com &
#gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 1016 -posnoise 3 badguys08@mek.com &
#gnome-terminal -x ../bzflag/src/bzrobots/bzrobots -team red -solo 1 -p 1017 -posnoise 3 badguys09@mek.com &

