/*	CS585_Lab2.cpp
*	CS585 Image and Video Computing Fall 2017
*	Lab 2
*	--------------
*	This program introduces the following concepts:
*		a) Reading a stream of images from a webcamera, and displaying the video
*		b) Skin color detection
*		c) Background differencing
*		d) Visualizing motion history
*	--------------
*/

//opencv libraries
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//C++ standard libraries
#include <iostream>
#include <vector>
#include <math.h>

using namespace cv;
using namespace std;

//function declarations

/**
Function that returns the maximum of 3 integers
@param a first integer
@param b second integer
@param c third integer
*/
int myMax(int a, int b, int c);

/**
Function that returns the minimum of 3 integers
@param a first integer
@param b second integer
@param c third integer
*/
int myMin(int a, int b, int c);

/**
Function that detects whether a pixel belongs to the skin based on RGB values
@param src The source color image
@param dst The destination grayscale image where skin pixels are colored white and the rest are colored black
*/
void mySkinDetect(Mat& src, Mat& dst);

/**
Function that does frame differencing between the current frame and the previous frame
@param src The current color image
@param prev The previous color image
@param dst The destination grayscale image where pixels are colored white if the corresponding pixel intensities in the current
and previous image are not the same
*/
void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst);

/**
Function that accumulates the frame differences for a certain number of pairs of frames
@param mh Vector of frame difference images
@param dst The destination grayscale image to store the accumulation of the frame difference images
*/
void myMotionEnergy(vector<Mat> mh, Mat& dst);

// Dakota Functions

/**
Function to determine whether an BGR color vector matches the Peer metric for
skin color.
*/
bool peer_metric(Vec3b& bgr);

int euclidean_color_difference(Vec3b& cur_color, Vec3b& past_color);

int main() {
    // read a new frame from video
    Mat frame;
    frame = imread("dakota_face.jpg", IMREAD_COLOR);
    // destination frame
    Mat frameDest;
    frameDest = Mat::zeros(frame.rows, frame.cols, CV_8UC1); //Returns a zero array of same size as src mat, and of type CV_8UC1
    namedWindow("Skin", WINDOW_AUTOSIZE);
    namedWindow("Original", WINDOW_AUTOSIZE);
    //----------------
    //	b) Skin color detection
    //----------------
    mySkinDetect(frame, frameDest);
    imshow("Original", frame);
    imshow("Skin", frameDest);


    //----------------
    //	c) Background differencing
    //----------------
    Mat dif_test;
    dif_test = frame -= 20;
    Mat dist_frame;
    dist_frame = Mat::zeros(frame.rows, frame.cols, CV_8UC1);
    myFrameDifferencing(frame, dif_test, dist_frame);
    namedWindow("Distance", WINDOW_AUTOSIZE);
    imshow("Distance", dist_frame);

    // //call myFrameDifferencing function
    // myFrameDifferencing(frame0, frame, frameDest);
    // imshow("MyVideo", frameDest);
    // myMotionHistory.erase(myMotionHistory.begin());
    // myMotionHistory.push_back(frameDest);
    // Mat myMH = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);

    // //----------------
    // //  d) Visualizing motion history
    // //----------------

    // //call myMotionEnergy function
    // myMotionEnergy(myMotionHistory, myMH);
    waitKey(0);
	return 0;
}

//Function that returns the maximum of 3 integers
int myMax(int a, int b, int c) {
	int m = a;
	(void)((m < b) && (m = b));
	(void)((m < c) && (m = c));
	return m;
}

//Function that returns the minimum of 3 integers
int myMin(int a, int b, int c) {
	int m = a;
	(void)((m > b) && (m = b));
	(void)((m > c) && (m = c));
	return m;
}

// Function that detects whether a pixel belongs to skin based on RGB values.
bool peer_metric(Vec3b& bgr) {
    int blue, green, red;
    blue = bgr[0];
    green = bgr[1];
    red = bgr[2];
	if (red > 95 && green > 40 && blue > 20) {
		if (myMax(bgr[0], bgr[1], bgr[2]) -  myMin(bgr[0], bgr[1], bgr[2]) > 15) {
			if (abs(bgr[2] - bgr[1]) > 15 && bgr[2] > bgr[1] && bgr[2] > bgr[0]) {
				return true;
			}
        }
    }
	return false;
}


// Function that creates a binary image based on skin detection via RBG values.
void mySkinDetect(Mat& src, Mat& dst) {
	//Surveys of skin color modeling and detection techniques:
	//Vezhnevets, Vladimir, Vassili Sazonov, and Alla Andreeva. "A survey on pixel-based skin color detection techniques." Proc. Graphicon. Vol. 3. 2003.
	//Kakumanu, Praveen, Sokratis Makrogiannis, and Nikolaos Bourbakis. "A survey of skin-color modeling and detection methods." Pattern recognition 40.3 (2007): 1106-1122.
	
	// Implement Peer method
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
            
            bool skin_color = peer_metric(src.at<Vec3b>(row, col));
            if (skin_color) {
                dst.at<uchar>(row, col) = 255;
            } else {
                dst.at<uchar>(row, col) = 0;
            }
		}
    }
}

// Function to estimate the euclidean distance between two color pixels
int euclidean_color_difference(Vec3b& curr_pixel, Vec3b& past_pixel) {
    int sum_of_square_dif = 0;
    for (int i = 0; i < 3; i ++) {
        sum_of_square_dif += pow((curr_pixel[0] - past_pixel[0]), 2);
    }
    return round(sqrt(sum_of_square_dif));
}

//Function that does frame differencing between the current frame and the previous frame
void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst) {
	for (int row = 0; row < curr.rows; row++) {
        for (int col = 0; col < curr.cols; col++) {
            Vec3b previous_pixel = prev.at<Vec3b>(row, col);
            Vec3b curr_pixel = curr.at<Vec3b>(row, col);
            dst.at<uchar>(row, col) = euclidean_color_difference(curr_pixel,
                                                                 previous_pixel);
        }
    }
}

//Function that accumulates the frame differences for a certain number of pairs of frames
void myMotionEnergy(vector<Mat> mh, Mat& dst) {
	
}