# Do not call this manually !
# This script will be called by the bridge at run-time
# This script will be executed from yarp_bridge/build

if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
        . "$HOME/.bashrc"
    fi
fi
if [ -n "$ZSH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.zshrc" ]; then
        . "$HOME/.zshrc"
    fi
fi

rm -f rosreception

# Cmake
echo "Generating the Headers (Cmake) ..."
if [ $# -eq 1 ]; then
	roscd yarp_ros_bridge_generator/src/to_ros_image_handler
	rm -rf build
	roscd yarp_ros_bridge_generator/src/to_ros_data_handler
else
	roscd yarp_ros_bridge_generator/src/to_ros_data_handler
	rm -rf build
	roscd yarp_ros_bridge_generator/src/to_ros_image_handler
fi

rosws set -y $PWD > /dev/null
source $ROS_WORKSPACE/setup.sh > /dev/null
mkdir -p build
cd build
cmake .. > /dev/null
cd ..
rosws remove $PWD > /dev/null
source $ROS_WORKSPACE/setup.sh > /dev/null
cd ..
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:$PWD
cd -
cd build

# Make 
echo "Compiling the ROS module ..."
pwd
make > /dev/null
if [ $? -eq 0 ] ; then # Everything went well
	cd ../..
	mv rosreception $(dirname $(which yarprosbridge))
	cd $(dirname $(which yarprosbridge))
	rm -f Log/bridgelastedit.txt
	echo "$1" > Log/bridgelastedit.txt # Refreshing the last edit since everything went well
fi
