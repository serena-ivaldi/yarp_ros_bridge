yarp_ros_bridge
===============

A bridge for exchanging data/images between YARP and ROS




A little bit of history for the YARP-ROS bridge.
------------

The first time we needed this bridge was around may 2012, when we decided to do experiments in HRI.
I was developing in YARP, others in ROS. 
We designed the module and formalized the idea of exchanging data, and we had the chance to find amazing students that could have fun with the code.

Version 0.1 was very specific to a particular data exchange case, and was line-command only. It was only tested with ROS electric.

Version 0.2 introduced a generic data exchange and a practical GUI for connecting yarp ports and ros topics. It was only tested with ROS groovy.

For version 0.3, our goal is to have something independent of MACSI's compilation chain, make code robust etc. 

