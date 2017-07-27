#include "opencv2\opencv.hpp"
#include "ParallelizeImage.hpp"
#include "ConvolutionMask.hpp"
#include <string>

using namespace cv;

int main(int argv, char** argc) {

	std::cout << "Lenna 0" << std::endl;
	
	Mat test = imread("Lenna.png", CV_LOAD_IMAGE_UNCHANGED);

	Mat clone = test.clone();

	int side = 5;
	ConvolutionMask* mask = new ConvolutionMask(side, ConvolutionMask::AVERAGE);
	ParallelizeImage* parallel = new ParallelizeImage();

	imshow("Lenna", clone);

	parallel->partialConvolve(test, clone, 0, 0, test.cols, test.rows, *mask);

	/*
	Point3_<uchar>* p;
	int i, j;
	for (i = 0; i < clone.cols; i++) {
		for (j = 0; j < clone.rows; j++) {
			p = clone.ptr<Point3_<uchar> >(i, j);
			uchar* r = (uchar*)&p->x;
			uchar* g = (uchar*)&p->y;
			uchar* b = (uchar*)&p->z;

			*r = 255;
			*g = 255;
			*b = 255;

		}
	}
	*/
	imshow("Lenna", test);

	waitKey();
}