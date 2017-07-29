#include "opencv2\opencv.hpp"
#include "ParallelizeImage.hpp"
#include "ConvolutionMask.hpp"
#include <string>

using namespace cv;

int main(int argv, char** argc) {

	std::cout << "Lenna 0" << std::endl;
	
	Mat test = imread("man.jpg", CV_LOAD_IMAGE_UNCHANGED);

	Mat clone = test.clone();

	int side = 5;
	ConvolutionMask* mask = new ConvolutionMask(side, ConvolutionMask::AVERAGE);
	ParallelizeImage* parallel = new ParallelizeImage();
	//parallel->partialConvolve(test, clone, 0, 0, clone.cols, clone.rows, *mask);
	
	parallel->sobelFilter(test, clone);
	imshow("LennaC", clone);
	imshow("Lenna", test);

	waitKey();
}