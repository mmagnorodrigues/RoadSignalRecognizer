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
	void setPixelrgb(Mat inImg, uchar red, uchar green, uchar blue, int x, int y);
	void setPixelGray(Mat inImg, uchar value, int x, int y);
	void ParallelizeConvolve(Mat inImg, ConvolutionMask conv);
	void partialConvolve(Mat inImg, Mat outImg, int imgMinX, int imgMinY, int imgMaxX, int imgMaxY, ConvolutionMask conv);
	void sobelFilter(Mat inImg, Mat outImg);
	void addAndBinarizeImgs(Mat img1, Mat img2, Mat outImg);
	void print(string string);
	void showImage(Mat img, int windowSizeX, int windowSizeY, string title);
	void convolve(Mat inImg, ConvolutionMask conv, bool parallel);

	bool isGrayImage(Mat img);
};

#endif