echo "Would you like to upload panther_ws as the latest version?"
select yn in "Yes" "No"; do
	case $yn in
		Yes ) echo Configuring upload...
cd
cd panther_ws
echo cleaning workspace
catkin clean
git init
PS3= "Select user:"
options=("Daniel" "Sebastian")
select opt in "${options[@]}"
do
    case $opt in
        "Daniel")
            echo "Configuring upload under Daniel.."
            git config --global user.email "belteshazzarboyd@gmail.com"
            break
            ;;
        "Sebastian")
            echo "Configuring upload under Sebastian..."
            git config --global user.email "matiaudasa@ornl.gov"
            break
            ;;
    esac
done

git add .
echo Type your commit message and press enter:
read comment
git commit -m "$comment"
git push origin main
echo Upload complete; break;;

		No ) echo exiting command...; exit;;
	esac
done

