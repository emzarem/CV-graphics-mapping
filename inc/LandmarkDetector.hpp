/* LandmarkDetector.hpp
*		Interface for the Haar-cascade face and eye detector.
*
*      This class uses Haar cascades to detect faces and eyes as well as to extract the focal
*      point of the face.
*
*      emzarem / ConnerCurrie / itwasarainyday / mradica
*      2018
*/

#ifndef LANDMARKDETECTOR_HPP
#define LANDMARKDETECTOR_HPP

#include "opencv2/opencv.hpp"

// -------------------------------------- //
struct face_pos {
	float x;
	float y;
};

// -------------------------------------- //
class LandmarkDetector
{
public: 
	LandmarkDetector(int frameX, int frameY);
	~LandmarkDetector();

	cv::Mat      FindLandmarks(cv::Mat frame);
	face_pos     GetFaceCenter();

private:
	int m_frameX, m_frameY;
	cv::CascadeClassifier m_face_cascade;
	cv::CascadeClassifier m_smile_cascade;
	cv::CascadeClassifier m_eyes_cascade;
	face_pos              m_faceCoordinates;

};





#endif
