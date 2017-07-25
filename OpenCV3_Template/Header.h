#pragma once
#ifndef	PARALLELIZEIMAGE
#define PARALLELIZEIMAGE

#include "opencv2\opencv.hpp"
#include "ConvolutionMask.h"

using namespace cv;

class ParallelizeImage : public cv::Mat {
public:
	void ParallelizeConvolve(ConvolutionMask);
};

#endif