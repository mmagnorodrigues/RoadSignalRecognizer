#pragma once
#ifndef	PARALLELIZEIMAGE
#define PARALLELIZEIMAGE

#include "opencv2\opencv.hpp"
#include "ConvolutionMask.hpp"
#include <tuple>
#include <string>
#include "Barrier.hpp"

using namespace cv;
using namespace std;

class ParallelizeImage{
public:
	void setPixelrgb(Mat inImg, uchar red, uchar green, uchar blue, int x, int y);
	void setPixelGray(Mat inImg, uchar value, int x, int y);
	void parallelizeConvolve(Mat inImg, ConvolutionMask conv, bool par, int minStep);
	void partialConvolve(Mat inImg, Mat outImg, int imgMinX, int imgMinY, int imgMaxX, int imgMaxY, ConvolutionMask conv, Barrier* bar);
	void sobelFilter(Mat inImg, Mat outImg, bool par, int minStep);
	void addAndBinarizeImgs(Mat img1, Mat img2, Mat outImg);
	void print(string string);
	void showImage(Mat img, int windowSizeX, int windowSizeY, string title);
	void convolve(Mat inImg, ConvolutionMask conv, bool parallel, int minStep);

	bool isGrayImage(Mat img);
};

#endif