/* LandmarkDetector.cpp
*		Implementation  of Haar-cascade face and eye detector.
*
*      This class uses Haar cascades to detect faces and eyes as well as to extract the focal
*      point of the face.
*
*      emzarem / ConnerCurrie / itwasarainyday / mradica
*      2018
*/


#include "../inc/LandmarkDetector.hpp"

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;
using namespace std;

LandmarkDetector::LandmarkDetector(int frameX, int frameY)
{
	cout << m_face_cascade.load("cascades/haarcascade_frontalface_alt2.xml") << endl;
	cout << m_smile_cascade.load("cascades/haarcascade_smile.xml") << endl;
	cout << m_eyes_cascade.load("cascades/haarcascade_eye_tree_eyeglasses.xml") << endl;

	m_frameX = frameX;
	m_frameY = frameY;

	cout << m_frameX << ", " << m_frameY << endl;
}

LandmarkDetector::~LandmarkDetector()
{
}

Mat LandmarkDetector::FindLandmarks(cv::Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	m_face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		m_eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++)
		{
			//cout << eyes.size() << endl;
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
		if (eyes.size() == 2) {
			float x1 = ((faces[i].x + eyes[1].x + eyes[1].width*0.5) + (faces[i].x + eyes[0].x + eyes[0].width*0.5)) / 2;
			float y1 = faces[i].y + ((eyes[0].y + eyes[1].y) / 2);
			Point pov(x1, y1);
			circle(frame, center, 2, Scalar(255, 0, 0), 4, 8, 0);
			m_faceCoordinates.x = (x1 / m_frameX) * 100;
			m_faceCoordinates.y = (y1 / m_frameY) * 100;
		}

	}

	return frame;
}


face_pos LandmarkDetector::GetFaceCenter()
{
	return m_faceCoordinates;
}