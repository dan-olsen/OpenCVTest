/*
 * main.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: daniel
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <chrono>
#include <ctime>

using namespace std;
using namespace cv;

/// Function Headers
void MatchingMethod(int argc, char** argv);

/** @function main */
int main( int argc, char** argv )
{
	/// Create windows
	cv::namedWindow("Result Window", CV_WINDOW_NORMAL );

	MatchingMethod(argc, argv);

	return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod(int argc, char** argv)
{
	cv::Mat image;
	std::vector<cv::Mat> templates;
	std::vector<cv::Point> matchLocs;
	std::vector<cv::Mat> results;
	std::chrono::time_point<std::chrono::steady_clock> load_start, load_end, upload_start, upload_end, exec_start, exec_end;

	load_start = std::chrono::steady_clock::now();

	image = cv::imread(argv[1]);

	if (image.empty())
	{
		std::cout << "Could not load image" << std::endl;
	}

	//Load Template Loop
	for(int i = 2; i < argc; i++)
	{
		cv::Mat temp = cv::imread(argv[i]);

		if (temp.empty())
		{
			std::cout << "Could not load template" << std::endl;
		}

		templates.push_back(temp);

	}

	load_end = std::chrono::steady_clock::now();

	exec_start = std::chrono::steady_clock::now();

	/// Do the Matching
	for(int i = 2, j = 0; i < argc; i++, j++)
	{
		cv::Mat result;

		cv::matchTemplate(image, templates[j], result, CV_TM_CCORR_NORMED);

		results.push_back(result);

		/// Localizing the best match with minMaxLoc
		double maxVal;
		cv::Point maxLoc;
		cv::Point matchLoc;

		cv::minMaxLoc( result, 0, &maxVal, 0, &maxLoc);

		matchLoc = maxLoc;

		matchLocs.push_back(matchLoc);
	}

	exec_end = std::chrono::steady_clock::now();

	std::chrono::duration<double> elapsed_seconds_load = load_end - load_start;
	std::chrono::duration<double> elapsed_seconds_upload = upload_end - upload_start;
	std::chrono::duration<double> elapsed_seconds_exec = exec_end - exec_start;

	std::cout << "Load time: " << elapsed_seconds_load.count() << " s" << std::endl;
	std::cout << "Upload time: " << elapsed_seconds_upload.count() << " s" << std::endl;
	std::cout << "Exec time: " << elapsed_seconds_exec.count() << " s" << std::endl;

	std::chrono::duration<double> elapsed_seconds = elapsed_seconds_load + elapsed_seconds_upload + elapsed_seconds_exec;

	std::cout << "Total Exec time: " << elapsed_seconds.count() << " s" << std::endl;


	for(int i = 2, j = 0; i < argc; i++, j++)
	{
		/// Source image to display
		cv::Mat img_display;
		image.copyTo(img_display);

		/// Show me what you got
		cv::rectangle(img_display, matchLocs[j], cv::Point( matchLocs[j].x + templates[j].cols , matchLocs[j].y + templates[j].rows ), cv::Scalar(0, 0, 204), 4, 8, 0 );

		imshow("Result Window", img_display );

		cv::waitKey(0);
	}
}
