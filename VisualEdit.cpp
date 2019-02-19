//
// Created by abdollah on 19/02/19.
//

#include "include/VisualEdit.h"

//--------------------------
// Gazebo related header files
#include <mutex>
#include <gazebo/common/Events.hh>
#include <gazebo/common/Time.hh>
#include <gazebo/rendering/Visual.hh>
#include <gazebo/transport/Node.hh>
#include <gazebo/rendering/rendering.hh>
#include <gazebo/msgs/color.pb.h>

//-------------------------
// ROS related header files
#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float32.h"


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

            public: transport::SubscriberPtr backgroundColorSub;

            public: transport::SubscriberPtr ambientColorSub ;

            /// \brief background color of the scene
            public: common::Color backgroundColor ;

            /// \brief ambient color of the scene
            public: common::Color ambientColor ;

            //---------------------------------------
            /// ROS related variables
            //---------------------------------------

            /// \brief A node used for transport (ROS)
            public: std::unique_ptr<ros::NodeHandle> rosNode ;

            /// \brief A subscriber(ROS).
            public: ros::Subscriber rosSub ;

            /// \brief A callback queue that helps process messages (ROS)
            public: ros::CallbackQueue rosQueue ;

            /// \brief A thread that keeps running the rosQueue
            public: std::thread rosQueueThread ;


    };

    using namespace gazebo  ;

    VisualEdit::VisualEdit() : dataPtr(new VisualEditPluginPrivate) {
    }

    VisualEdit::~VisualEdit() {
        this->dataPtr->backgroundColorSub.reset() ;
        this->dataPtr->ambientColorSub.reset() ;
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

        // Initialize the node
        this->dataPtr->node = transport::NodePtr(new transport::Node()) ;

        this->dataPtr->node->Init() ;


        // Create a topic name for changing background color of scene
        std::string sBackgroundColorTopicName = "~/" + this->topicName + "/backgroundColor" ;
        // Create a topic name for changing ambient color of scene
        std::string sAmbientTopicName = "~/" + this->topicName + "/ambientColor" ;

        ///setup the subscription and callback functions
        // Setting up for receiving background color changing commands
        this->dataPtr->backgroundColorSub = this->dataPtr->node->Subscribe(sBackgroundColorTopicName, &VisualEdit::OnBackgroundColorInfo, this);

        // Setting up for receiving  color changing commands
        this->dataPtr->ambientColorSub = this->dataPtr->node->Subscribe(sAmbientTopicName, &VisualEdit::OnAmbientColorInfo, this);

        //----------------------
        /// Ros communication setup section

        //Initialize ROS if it has not been already initialized
        if (!ros::isInitialized()) {
            int argc = 0 ;
            char **argv = nullptr ;

            ros::init(argc , argv , "gazebo_client" , ros::init_options::NoSigintHandler) ;

        }

        // Create the ROS node. This is similar to the Gazebo node
        this->dataPtr->rosNode.reset(new ros::NodeHandle("gazebo_client")) ;

        // Create a named topic and subscribe to it for background color changing of scene
        ros::SubscribeOptions so = ros::SubscribeOptions::create<std_msgs::Float32>("/" + this->topicName + "/backgroundColor" ,
                                                                                    1 , boost::bind(&VisualEdit::OnRosBMsg , this, _1) , ros::VoidPtr() , &this->dataPtr->rosQueue ) ;


        this->dataPtr->rosSub = this->dataPtr->rosNode->subscribe(so) ;

        //spin up the queue helper thread
        this->dataPtr->rosQueueThread = std::thread(std::bind(&VisualEdit::QueueThread,  this)) ;



    }


    void VisualEdit::Update() {

    }

    void VisualEdit::OnBackgroundColorInfo(ConstColorPtr &_msg) {

        std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
        this->dataPtr->backgroundColor.Set(_msg->r() , _msg->g() , _msg->b() , 1.0) ;
        this->dataPtr->scene->SetBackgroundColor(this->dataPtr->backgroundColor) ;

    }

    void VisualEdit::OnAmbientColorInfo(ConstColorPtr &_msg) {

        std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
        this->dataPtr->ambientColor.Set(_msg->r() , _msg->g() , _msg->b() , 1.0) ;
        this->dataPtr->scene->SetAmbientColor(this->dataPtr->ambientColor) ;

    }


    void VisualEdit::OnRosBMsg(const std_msgs::Float32ConstPtr &_msg) {

        //const float* data1 =  _msg->data.data() ;

        std::cout << "I got something" << std::endl ;

        std::lock_guard<std::mutex> lock(this->dataPtr->mutex);
        this->dataPtr->backgroundColor.Set(_msg->data , 0.0 , 0.0 , 1.0) ;
        this->dataPtr->scene->SetBackgroundColor(this->dataPtr->backgroundColor) ;


//
//        std::cout << "The data received in from ROS is: " << _msg->data[0] << std::endl ;
//        std::cout << "The data received in from ROS is: " << _msg->data[1] << std::endl ;
//        std::cout << "The data received in from ROS is: " << _msg->data[2] << std::endl ;
    }

    void VisualEdit::QueueThread() {
        static const double timeout = 0.01 ;
        while(this->dataPtr->rosNode->ok()) {
            this->dataPtr->rosQueue.callAvailable(ros::WallDuration(timeout)) ;
        }
    }

    GZ_REGISTER_VISUAL_PLUGIN(VisualEdit)
}