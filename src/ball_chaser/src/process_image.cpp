#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO("DriveToTarget Request received - Linear x: %f , angular z: %f",lin_x,ang_z);
    
    
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    uint32_t width  = img.width;
    uint32_t height = img.height;
    
    for (uint32_t y = 0; y < height;y++){
        for (uint32_t x = 0; x < width;x++){
        
    	    int index = y * img.step + x * 3;

            uint8_t c1 = img.data[index + 0];
            uint8_t c2 = img.data[index + 1];
            uint8_t c3 = img.data[index + 2];
            
            if (c1 == white_pixel && c2 == white_pixel && c3 == white_pixel) {
                if (x < width/3){
                	ROS_INFO("Driving Left");
                }else if (x < (2 * width) / 3){
                	ROS_INFO("Driving Forward");
                }else {
                	ROS_INFO("Driving Right");
                }
                
                
            }
    	}
    }
    
    
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
