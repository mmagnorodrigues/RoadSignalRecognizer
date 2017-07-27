#pragma once
#ifndef	PARALLELIZEIMAGE
#define PARALLELIZEIMAGE

#include "opencv2\opencv.hpp"
#include "ConvolutionMask.hpp"
#include <tuple>
#include <string>

using namespace cv;
using namespace std;

class ParallelizeImage{
public:
	void setPixel(Mat inImg, uchar red, uchar green, uchar blue, int x, int y);
	void ParallelizeConvolve(Mat inImg, ConvolutionMask conv);
	void partialConvolve(Mat inImg, Mat outImg, int imgMinX, int imgMinY, int imgMaxX, int imgMaxY, ConvolutionMask conv);

	void print(string string);
};

#endif