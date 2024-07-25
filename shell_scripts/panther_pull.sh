echo "Would you like to delete panther_ws and download the latest version?"
select yn in "Yes" "No"; do
	case $yn in
		Yes ) echo Deleting local panther_ws
cd
rm --r --f panther_ws
echo Downloading the latest version of panther_ws
git clone git@github.com:TBelteshazzarT/panther_ws.git
echo panther_ws is now up to date
echo Remember to run catkin build to compile packages before running any nodes and run rosdep.; break;;
		No ) echo exiting command...; exit;;
	esac
done

