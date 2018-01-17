/* Perspective_Main.cpp
 *		This is the main program for our OpenCV/DLib and OpenGL perspective visual.
 *
 *      The purpose of this is to adjust the virtual perspective of an OpenGL scene in
 *      on the screen in real time based on the location of the user's face in order to
 *      give the illusion of depth.
 *
 *      emzarem / ConnerCurrie / itwasarainyday / mradica
 *      2018
*/

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include "../inc/LandmarkDetector.hpp"
//#include "GraphicsGenerator.hpp"

#include <iostream>
#include <conio.h>

using namespace cv;

//-----------------------------------------------------------------------------------//
int main()
{
	VideoCapture webcam(0);			// Acquire webcam 0

	if (!webcam.isOpened())			// Confirm it was acquired
	{
		std::cout << "Error in acquiring webcam 0\n";
		_getch();
		return(0);
	}

	Mat  imgFrame;					// Frame from webcam
	char chCheckKey = 0;			// Keystroke detected
	int frameX = webcam.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	int frameY = webcam.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
	LandmarkDetector  detector(frameX, frameY);     // Haar-cascade-based facial feature detector
	//GraphicsGenerator graphics;     // OpenGL-based graphics

	//graphics.StartDisplay();

	while (chCheckKey != 27 && webcam.isOpened())	// Loop until user escapes
	{
		bool bReadSuccess = webcam.read(imgFrame);

		if (!bReadSuccess || imgFrame.empty())		// Error checking
		{
			std::cout << "Error in reading frame from webcam 0\n";
			break;
		}

		resize(imgFrame, imgFrame, Size(1000, 640));			// Resize frame
		imgFrame = detector.FindLandmarks(imgFrame);			// Find landmarks
		face_pos pt = detector.GetFaceCenter();					// Get Center coord's
		imshow("cap", imgFrame);								// Display
		std::cout << pt.x << ", " << pt.y << std::endl;

		//graphics.UpdatePerspective(pt.x, pt.y);		// Update Graphics

		waitKey(1);
	}

	//graphics.EndDisplay();

	return(0);
}

