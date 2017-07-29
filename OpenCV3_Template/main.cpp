#include "opencv2\opencv.hpp"
#include "ParallelizeImage.hpp"
#include "ConvolutionMask.hpp"
#include <string>

using namespace cv;

int main(int argv, char** argc) {

	std::cout << "Lenna 0" << std::endl;
	
	Mat test = imread("man.jpg", CV_LOAD_IMAGE_UNCHANGED);

	Mat clone = test.clone();

	int side = 9;
	ConvolutionMask* mask = new ConvolutionMask(side, ConvolutionMask::AVERAGE);
	ParallelizeImage* parallel = new ParallelizeImage();

	parallel->convolve(clone, *mask);
	parallel->showImage(clone, 800, 800, "baita teste");

	waitKey();
}
