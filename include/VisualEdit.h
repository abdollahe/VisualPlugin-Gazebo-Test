//
// Created by abdollah on 19/02/19.
//

#include <gazebo/msgs/MessageTypes.hh>
#include "gazebo/common/Plugin.hh"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Float32.h"



#ifndef GAZEBO_PLUGIN_TUTORIAL_VISUALEDIT_H
#define GAZEBO_PLUGIN_TUTORIAL_VISUALEDIT_H

namespace gazebo {

class VisualEditPluginPrivate ;

class GAZEBO_VISIBLE VisualEdit : public VisualPlugin {

private :
     std::unique_ptr<VisualEditPluginPrivate> dataPtr;
     std::string topicName = "visual_edit" ;

public:
     VisualEdit() ;
     ~VisualEdit() override ;

     /// Gazebo related methods
     void Load(rendering::VisualPtr _visual, sdf::ElementPtr _sdf) override;

     /// ROS related methods
     void OnRosBMsg(const std_msgs::Float32ConstPtr &_msg) ;

private :
    /// Gazebo related methods
    void Update() ;
    void OnBackgroundColorInfo(ConstColorPtr &_msg);
    void OnAmbientColorInfo(ConstColorPtr &_msg) ;

    /// ROS related methods
    void QueueThread() ;

};
}


#endif //GAZEBO_PLUGIN_TUTORIAL_VISUALEDIT_H
