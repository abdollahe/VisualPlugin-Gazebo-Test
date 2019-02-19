//
// Created by abdollah on 19/02/19.
//

#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

#if GAZEBO_MAJOR_VERSION < 6
#include <gazebo/gazebo.hh>
#else
#include <gazebo/gazebo_client.hh>
#endif


int main(int _argc , char ** _argv) {


#if GAZEBO_MAJOR_VERSION < 6
    gazebo::setupClient(_argc, _argv);
#else
    gazebo::client::setup(_argc, _argv);
#endif


    // Create node for communication
    gazebo::transport::NodePtr node(new gazebo::transport::Node()) ;
    node->Init() ;


    //Create publisher to sent messages to the verlodyne topic
    gazebo::transport::PublisherPtr pub = node->Advertise<gazebo::msgs::Color>("~/visual_edit/ambientColor") ;


    //Wait for a subscriber to connect to this publisher
    pub->WaitForConnection() ;

    // Create a color message
    gazebo::msgs::Color msg ;


    // Set the velocity in the x-component
#if GAZEBO_MAJOR_VERSION < 6
    gazebo::msgs::Set(&msg, gazebo::math::Vector3(std::atof(_argv[1]), 0, 0));
#else
    gazebo::msgs::Set(&msg, gazebo::common::Color(static_cast<float>(std::atof(_argv[1])), static_cast<float>(std::atof(_argv[2])) , static_cast<float>(std::atof(_argv[3]))));
#endif

    // Send the message
    pub->Publish(msg);


    // Make sure to shut everything down.
#if GAZEBO_MAJOR_VERSION < 6
    gazebo::shutdown();
#else
    gazebo::client::shutdown();
#endif


    return 0;
}