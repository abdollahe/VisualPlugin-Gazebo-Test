//
// Created by abdollah on 19/02/19.
//

#include "include/VisualEdit.h"

#include <mutex>
#include <gazebo/common/Events.hh>
#include <gazebo/common/Time.hh>
#include <gazebo/rendering/Visual.hh>
#include <gazebo/transport/Node.hh>
#include <gazebo/rendering/rendering.hh>

namespace gazebo {


    class VisualEditPluginPrivate {

            public: rendering::ScenePtr scene ;

            /// \brief Visual whose color will be changed.

            public: rendering::VisualPtr visual;

            /// \brief Connects to rendering update event.

            public: event::ConnectionPtr updateConnection;


            /// \brief Node used for communication.

            public: transport::NodePtr node;


            /// \brief Node used for communication.

            public: std::mutex mutex;


            /// \brief True to use wall time, false to use sim time.

            public: bool useWallTime;


            /// \brief Subscriber to world info.

            public: transport::SubscriberPtr infoSub;
    };

    using namespace gazebo  ;

    VisualEdit::VisualEdit() : dataPtr(new VisualEditPluginPrivate) {
    }

    VisualEdit::~VisualEdit() {
        this->dataPtr->infoSub.reset() ;
        if(this->dataPtr->node)
            this->dataPtr->node->Fini() ;
    }

    void VisualEdit::Load(gazebo::rendering::VisualPtr _visual, sdf::ElementPtr _sdf) {

        if (!_visual || !_sdf)
              {
                gzerr << "No visual or SDF element specified. Plugin won't load." << std::endl;

                return;
              }

        this->dataPtr->visual = _visual;
        gzmsg << "Able to find the plugin" << std::endl;

        this->dataPtr->scene = rendering::get_scene(); ;

        if(! this->dataPtr->scene) {
            gzerr << "Scene is empty mate!!!" << std::endl ;
        }

        else {
            gzmsg << "Able to acquire ref to scene mate!!! and its name is:" << this->dataPtr->scene->Name() << std::endl ;

            common::Color colorA = this->dataPtr->scene->BackgroundColor() ;

            gzmsg << "Background color of scene -> R :" << colorA.r << std::endl ;
            gzmsg << "Background color of scene -> G :" << colorA.g << std::endl ;
            gzmsg << "Background color of scene -> B :" << colorA.b << std::endl ;
            gzmsg << "Background color of scene -> A :" << colorA.a << std::endl ;

            common::Color colorB = this->dataPtr->scene->AmbientColor() ;

            gzmsg << "Ambient color of scene -> R :" << colorB.r << std::endl ;
            gzmsg << "Ambient color of scene -> G :" << colorB.g << std::endl ;
            gzmsg << "Ambient color of scene -> B :" << colorB.b << std::endl ;
            gzmsg << "Ambient color of scene -> A :" << colorB.a << std::endl ;

            const common::Color myBackground = *( new common::Color(1.0 , 0.0 , 0.0 , 1.0) ) ;

            gzmsg << "const color for background -> R :" << myBackground.r << std::endl ;
            gzmsg << "const color for background -> G :" << myBackground.g << std::endl ;
            gzmsg << "const color for background -> B :" << myBackground.b << std::endl ;
            gzmsg << "const color for background -> A :" << myBackground.a << std::endl ;

            //this->dataPtr->scene->SetBackgroundColor(myBackground) ;


            const common::Color myAmbient = *( new common::Color(1.0 , 0.0 , 0.0 , 1.0) ) ;

            gzmsg << "const color for ambient -> R :" << myAmbient.r << std::endl ;
            gzmsg << "const color for ambient -> G :" << myAmbient.g << std::endl ;
            gzmsg << "const color for ambient -> B :" << myAmbient.b << std::endl ;
            gzmsg << "const color for ambient -> A :" << myAmbient.a << std::endl ;


            //this->dataPtr->scene->SetAmbientColor(myAmbient) ;

            gzmsg << "Colors Updated!!" << std::endl ;

        }


    }


    void VisualEdit::Update() {

    }

    void VisualEdit::OnInfo(ConstPosesStampedPtr &_msg) {


    }

    GZ_REGISTER_VISUAL_PLUGIN(VisualEdit)
}