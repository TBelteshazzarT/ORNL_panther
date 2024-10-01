#!/bin/bash
# Terminal 1
gnome-terminal -- bash -c "
	sshpass -p 'ORNLROV!' ssh ornl-rov@192.168.5.235 'panther_network_launch'
	exec bash" &
	
#Terminal 2
gnome-terminal -- bash -c "panther_rviz exec bash"

