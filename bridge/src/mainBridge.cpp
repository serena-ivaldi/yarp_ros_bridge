/*
 * Copyright (C) 2013 MACSi Project
 * Author: Woody Rousseau
 * email:  woody.rousseau@ensta-paristech.fr
 * website: www.macsi.isir.upmc.fr
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/


// Yarp Librairies
 /**
@ingroup macsi_tools

\defgroup yarp_ros_bridge_generator

Bridge for sending data between YARP and ROS

\section intro_sec Description

Bridge for sending data between YARP and ROS

\section lib_sec Libraries
- YARP libraries.
- ROS (groovy) libraries.
- python-dev libraries
- OpenCv libraries

\section parameters_sec Parameters
none

\section portsa_sec Ports Accessed
It connects a YARP port with a ROS topic defined by the user in the .ini file.


\section tested_os_sec Tested OS
GNU/Linux Ubuntu 12.04

\author Woody Rousseau
*
/**
 * \file mainbridge.cpp
 * \brief Main entry to the program
 * \author Woody Rousseau
 * \version 0.1
 * \date 23/05/13
 *
 * We here launch the ros programs by overlapping children processes created using fork()
 */

#include "Python.h"

// Yarp Librairies
#include <yarp/os/Network.h>
#include <yarp/os/Property.h>

#include <algorithm>

#include "bridge.h"
#include "convertModule.h"

using namespace std;
using namespace yarp::os;

/**
* \fn int main (int argc, char **argv)
* \brief Main entry for the program, creates the pipe.
*
* \return EXIT_SUCCESS - The program has successfully ran
*/
int main(int argc, char **argv)
{
    cout << argv[0] << endl;
    bool gui = false;
    vector<string> argsVector(argv+1,argv + argc);
    gui = find(argsVector.cbegin(),argsVector.cend(), "--gui") != argsVector.cend();
    
    // Setting up YARP
    Network yarp;
    if (!yarp.checkNetwork())
    {
        cerr << "[yarp]::The YARP server is not available!" << endl;
        return -1;
    }

    int fd[2];

    if ( pipe(fd) == -1 )
    {
        cerr << "[yarp]::Pipe Error : " << strerror(errno) << endl;
        std::exit(EXIT_FAILURE);
    }
    
    // Let's load the parameters (.ini file)
    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("ros/conf");
    rf.setDefaultConfigFile("bridge.ini");
    rf.configure("MACSI_ROOT",argc,argv);

    if (rf.check("help"))
    {
        cout << "Options:" << endl << endl;
        cout << "\t--from fileName: input configuration file" << endl;
        cout << "\t--context dirName: resource finder context" << endl;

        return 0;
    }

    // We create the module which will be handling the conversion
    ConvertModule module(fd,gui);
    module.configure(rf); // We pass the ResourceFinder object
    module.runModule();

    return 0;
}




