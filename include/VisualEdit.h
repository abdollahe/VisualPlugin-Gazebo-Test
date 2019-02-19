//
// Created by abdollah on 19/02/19.
//

#include <gazebo/msgs/MessageTypes.hh>
#include "gazebo/common/Plugin.hh"



#ifndef GAZEBO_PLUGIN_TUTORIAL_VISUALEDIT_H
#define GAZEBO_PLUGIN_TUTORIAL_VISUALEDIT_H

namespace gazebo {

class VisualEditPluginPrivate ;

class GAZEBO_VISIBLE VisualEdit : public VisualPlugin {

private :
     std::unique_ptr<VisualEditPluginPrivate> dataPtr;
public:
     VisualEdit() ;
     ~VisualEdit() override ;
     void Load(rendering::VisualPtr _visual, sdf::ElementPtr _sdf) override;

private :
    void Update() ;
    void OnInfo(ConstPosesStampedPtr &_msg);


};
}


#endif //GAZEBO_PLUGIN_TUTORIAL_VISUALEDIT_H
