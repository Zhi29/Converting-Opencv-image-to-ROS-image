#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>  
        
    class SubscribeAndPublish    
    {    
    public:    
      SubscribeAndPublish()    
      {    
        image_transport::ImageTransport it(nh); 
        //Topic you want to publish    
        pub_ = it.advertise("usb_cam/image", 1);    
        
        //Topic you want to subscribe    
        sub_ = it.subscribe("usb_cam/image_raw", 1, &SubscribeAndPublish::callback, this);   
      }    
        
      void callback (const sensor_msgs::ImageConstPtr& msg)   
      {     
        //.... do something with the input and generate the output... 
        try
        {
          cv::namedWindow("view");
          cv::Mat img =  cv_bridge::toCvShare(msg, "bgr8")->image;
          cv::imshow("view", img);
          ROS_INFO("get image");
          cv::waitKey(300);
          //destroyWindow("view");
          sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg();
          pub_.publish(msg); 
        }      
        catch (cv_bridge::Exception& e)
        {
          ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
        }
      }    
        
    private:    
      ros::NodeHandle nh;    
      image_transport::Publisher pub_;    
      image_transport::Subscriber sub_;    
        
    };//End of class SubscribeAndPublish    
        
    int main(int argc, char **argv)    
    {    
      //Initiate ROS    
      ros::init(argc, argv, "subscribe_and_publish");    

      cv::startWindowThread();
        
      //Create an object of class SubscribeAndPublish that will take care of everything    
      SubscribeAndPublish SAPObject;    
        
      ros::spin();    
        
      return 0;    
    }