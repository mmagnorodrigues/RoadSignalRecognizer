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
	//parallel->sobelFilter(test, clone);
	Mat channels[3];
	split(clone, channels);
	/*parallel->showImage(channels[0], 800, 800, "testao");
	for (int i = 0; i < clone.cols; i++) {
		for (int j = 0; j < clone.rows; j++) {
			parallel->setPixelGray(channels[0], 0, i, j);
			parallel->setPixelGray(channels[1], 0, i, j);
		}
	}
	merge(channels, 3, clone);*/
	parallel->print("a");
	parallel->partialConvolve(channels[0], channels[1], 0, 0, channels[0].cols, channels[1].rows, *mask);
	parallel->showImage(channels[1], 800, 800, "baita teste");

	waitKey();
}
