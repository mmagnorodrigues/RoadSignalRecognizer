#include "opencv2\opencv.hpp"
#include <string>

using namespace cv;

int main(int argv, char** argc) {

	std::cout << "Lenna 1" << std::endl;
	
	Mat test = imread("Lenna.png", CV_LOAD_IMAGE_UNCHANGED);

	Mat clone = test.clone();

	Point3_<uchar>* p;
	int i, j;
	
	for (i = 0; i < clone.cols; i++) {
		for (j = 0; j < clone.rows; j++) {
			p = clone.ptr<Point3_<uchar> >(i, j);
			ushort* r = (ushort*)&p->x;
			ushort* g = (ushort*)&p->y;
			ushort* b = (ushort*)&p->z;

			*r = 0;
			*g = 0;
			*b = 0;

		}
	}
	imshow("Lenna", clone);

	waitKey();
}