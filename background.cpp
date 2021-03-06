
//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
// Global variables
Mat frame; //current frame
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Mat back=Mat::zeros(FRAME_HEIGHT,FRAME_WIDTH,CV_8UC1);
Ptr<BackgroundSubtractorMOG2> pMOG2; //MOG2 Background subtractor

/** Function Headers */

void processVideo();




/**
 * @function main
 */
int main()
{   

    //create GUI windows
    namedWindow("Frame");
    namedWindow("FG Mask MOG 2");

    //create Background Subtractor objects
    pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach

    processVideo();
    destroyAllWindows();
  return EXIT_SUCCESS;
}

/**
 * @function processVideo
 */
void processVideo() {
    //create the capture object
    VideoCapture capture;
    //open capture object at location zero (default location for webcam)
    capture.open(0);
    Mat firstFrame=Mat(FRAME_HEIGHT,FRAME_WIDTH,CV_8UC1);
    //set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
    
    capture.read(firstFrame);
    flip(firstFrame, firstFrame, 1);
    cvtColor(firstFrame,firstFrame,COLOR_BGR2GRAY);
    while(1){
        //store image to matrix
        capture.read(frame);
        flip(frame, frame, 1);
        cvtColor(frame,frame,COLOR_BGR2GRAY);
        GaussianBlur(frame, frame, Size(21,21), 0);

        absdiff(firstFrame,frame,back);
        threshold(back, back, 25, 255, THRESH_BINARY);
        firstFrame=frame;



        // int backgroundFrame=100;
        // pMOG2->setNMixtures(3);
        // pMOG2->setDetectShadows(false);
        // //pMOG2->setHistory(1);

        // if(backgroundFrame>0){
        // //update the background model
        //     pMOG2->apply(frame, fgMaskMOG2);
        //     backgroundFrame--;
        // }
        // else{
        //     pMOG2->apply(frame, fgMaskMOG2,0);
        // }
        
        // pMOG2->getBackgroundImage(back);
        // back=back-frame;
         int iterations=2;
        erode(back, back, iterations);
       
        dilate(back, back, iterations);

        	

        //show the current frame and the fg masks

        imshow("Frame", back);
        imshow("FG Mask MOG 2", frame);
        //get the input from the keyboard
        waitKey( 15 );
    }
   capture.release();
}
