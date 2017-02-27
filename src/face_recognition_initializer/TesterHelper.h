///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2015, University of Cambridge,
// all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "CLM_core.h"
#include <std_msgs/String.h>


#include <fstream>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Face_utils.h>
#include <FaceAnalyser.h>
#include <GazeEstimation.h>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <clm_ros_wrapper/ClmHeads.h>
#include <clm_ros_wrapper/ClmEyeGaze.h>
#include <clm_ros_wrapper/GazeDirection.h>
#include <clm_ros_wrapper/ClmFacialActionUnit.h>
#include <clm_ros_wrapper/VectorWithCertainty.h>

#include <clm_ros_wrapper/ClmEyeGazes.h>
#include <clm_ros_wrapper/GazeDirections.h>
#include <clm_ros_wrapper/VectorsWithCertainty.h>
#include <clm_ros_wrapper/ClmHeadVectors.h>

#include <filesystem.hpp>
#include <filesystem/fstream.hpp>

#include <math.h>

#include <tf/transform_datatypes.h>

class TesterHelper
{
private:
    std::string face_recognizer_file_location;
    std::string executable_location;

    ros::NodeHandle nodeHandle;

    image_transport::ImageTransport imageTransport;
    image_transport::Subscriber     imageSubscriber;

    // publisher for the detected face images
    // image_transport::Publisher imagePublisher;

    //the total number of frames displayed
    int frame_count;
    //the number of frames with a face detected
    int num_detected_frames;
    int total_frames;
    int reported_completion;

    cv::Mat captured_image;

    CLMTracker::CLM clm_model;
    vector<CLMTracker::CLMParameters> clm_parameters;

    // The modules that are being used for tracking
    vector<CLMTracker::CLM> clm_models;
    vector<bool> active_models;

    bool video_input;
    bool images_as_video;

    // parameters needed for face recognition
    vector<Mat> faces_train;
    vector<int> labels_train;
    Ptr<cv::face::FaceRecognizer> face_recognizer;
    bool is_train;
    bool is_model_trained;
    int train_stage;
    int num_train_samples;

    void publishImage(cv::Mat &mat);

    void NonOverlappingDetections(const vector<CLMTracker::CLM>& clm_models,
      vector<cv::Rect_<double> >& face_detections);

    // get face image
    void retrieveFaceImage(cv::Mat img, const CLMTracker::CLM& clm_model);

    /**
    * Callback on the subscriber's topic.
    * @param msgIn an RGB image
    */
    void callback(const sensor_msgs::ImageConstPtr& msgIn);

public:
    TesterHelper(std::string _name, std::string _loc);
    ~TesterHelper() {};
};