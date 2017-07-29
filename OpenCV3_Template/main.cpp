#include "opencv2\opencv.hpp"
#include "ParallelizeImage.hpp"
#include "ConvolutionMask.hpp"
#include <string>

using namespace cv;
using namespace std;

int main(int argv, char** argc) {
	clock_t begin = clock();
	std::cout << "Lenna 0" << std::endl;
	
	Mat test = imread("man.jpg", CV_LOAD_IMAGE_UNCHANGED);

	Mat clone = test.clone();

	int side = 5;
	ConvolutionMask* mask = new ConvolutionMask(side, ConvolutionMask::AVERAGE);
	ParallelizeImage* parallel = new ParallelizeImage();

	parallel->convolve(clone, *mask, true);
	parallel->showImage(clone, 800, 800, "baita teste");

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout << "time: " << elapsed_secs << endl;

	waitKey();
}


