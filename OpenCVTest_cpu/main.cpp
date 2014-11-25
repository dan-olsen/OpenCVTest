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

/// Global Variables
Mat img; Mat templ; Mat result;
string image_window = "Source Image";
string result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load image and template
  img = imread( argv[1], 1 );
  templ = imread( argv[2], 1 );

  /// Create windows
  namedWindow( image_window, CV_WINDOW_NORMAL );
  namedWindow( result_window, CV_WINDOW_NORMAL );

  MatchingMethod( 0, 0 );

  waitKey(0);
  return 0;
}

/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  //int result_cols =  img.cols - templ.cols + 1;
  //int result_rows = img.rows - templ.rows + 1;

  //result.create( result_cols, result_rows, CV_32FC1 );
  //result.create( img.cols, img.rows, CV_32FC1 );

  std::chrono::time_point<std::chrono::steady_clock> start, end;

  start = std::chrono::steady_clock::now();

  /// Do the Matching and Normalize
  matchTemplate( img, templ, result, CV_TM_CCORR_NORMED);
  //normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  end = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed_seconds = end - start;


  std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

  /// Localizing the best match with minMaxLoc
  double maxVal;
  Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, 0, &maxVal, 0, &maxLoc, Mat() );

  matchLoc = maxLoc;

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0, 0, 204), 4, 8, 0 );
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0, 0, 204), 2, 8, 0 );

  imshow( image_window, img_display );
  imshow( result_window, result );

  return;
}
