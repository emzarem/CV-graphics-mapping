//CV app © The boys

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;
using namespace std;

struct face_pos {
    float x;
    float y;
};

void detectAndDisplay(Mat frame);
void printStruct(struct face_pos);
CascadeClassifier face_cascade;
CascadeClassifier smile_cascade;
CascadeClassifier eyes_cascade;
face_pos faceCoordinates;
int frameX, frameY;

int main(int argc, char* argv[]) {
    
    //Open the default video camera
    VideoCapture cap(0);
    
    // if not success, exit program
    if (cap.isOpened() == false) {
        cout << "Cannot open the video camera" << endl;
        return -1;
    }
    
    Mat img;
    Mat frame;

    face_cascade.load( "haarcascade_frontalface_alt2.xml" );
    smile_cascade.load( "haarcascade_smile.xml" );
    eyes_cascade.load( "haarcascade_eye_tree_eyeglasses.xml" );
    
    frameX = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    frameY = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    
    cout<< frameX;
    cout<<frameY;
    
    string window_name = "Smile detector";
    namedWindow(window_name); //create a window called "Smile detector"
    
    while (true) {
        bool bSuccess = cap.read(frame); // read a new frame from video
        
        //Breaking the while loop if the frames cannot be captured
        if (bSuccess == false) {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }
        
        // Image from camera to Mat
        
        cap >> img;
        
        // obtain input image from source
        cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);
        
        // Just resize input image if you want
        resize(img, img, Size(1000,640));
        
        detectAndDisplay(frame);
        //wait for for 10 ms until any key is pressed.
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop
        //If any key is not pressed withing 10 ms, continue the loop 
        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            break;
        }
    }
    
    return 0;
    
}

void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;
    
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    
    for( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;
        
        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        
        for( size_t j = 0; j < eyes.size(); j++ )
        {
            cout<< eyes.size() << endl;
            Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
        if(eyes.size() == 2){
            float x1 = ((faces[i].x + eyes[2].x + eyes[2].width*0.5) + (faces[i].x + eyes[1].x + eyes[1].width*0.5))/2;
            float y1 = faces[i].y + ((eyes[1].y+eyes[2].y)/2);
            Point pov(x1,y1);
            circle( frame, center, 2, Scalar( 255, 0, 0 ), 4, 8, 0 );
            faceCoordinates.x = (x1/frameX)*100;
            faceCoordinates.y = (y1/frameY)*100;
            printStruct(faceCoordinates);
        }
        
    }
    //-- Show what you got
    imshow( "cap", frame );
}

void printStruct(face_pos f){
    cout << "x:" << f.x << "% | y:" << f.y << "%" << endl;
}
