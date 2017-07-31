#include "opencv2\opencv.hpp"
#include "ParallelizeImage.hpp"
#include "ConvolutionMask.hpp"
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

void writeOnFile(string fileName, double time, int minStep);

int main(int argv, char** argc) {
	int imgSize = 800;
	std::string imageName = "man.jpg";

	bool par = true;
	
	Mat test = imread(imageName, CV_LOAD_IMAGE_UNCHANGED);

	Mat clone = test.clone();

	int side = 5;
	ConvolutionMask* mask = new ConvolutionMask(side, ConvolutionMask::AVERAGE);
	ParallelizeImage* parallel = new ParallelizeImage();

	/*Mat splited[3];
	split(test, splited);*/
	
	//parallel->parallelizeConvolve(clone, *mask);
	//parallel->partialConvolve(splited[1], splited[2],0, 0, 800, 500, *mask);
	//parallel->sobelFilter(test, clone, false, 30);
	//parallel->showImage(clone, imgSize, imgSize, "baita teste");

	double totalTime = 0;
	int numtests = 3;
	int minStep = 100;
	int start = 0;
	for (int i = start; i < 20; i++) {
		for (int j = 0; j < numtests; j++) {
			clone.release();
			clone = test.clone();

			cout << j << endl;
			clock_t begin = clock();
			parallel->convolve(clone, *mask, par, minStep*(i + 1));
			clock_t end = clock();
			double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			totalTime += elapsed_secs;
		}
		if(par)
			writeOnFile((imageName + ".txt"), (totalTime / numtests), minStep*(i + 1));

		cout << imageName <<"-> time " << (totalTime/ numtests) <<" side: "<<side <<" minstep: "<< minStep*(i+1)<< endl;
		totalTime = 0;
	}

	imshow("teste", clone);

	waitKey();
}

void writeOnFile(string fileName, double time, int minStep) {
	std::ofstream out;

	// std::ios::app is the open mode "append" meaning
	// new data will be written to the end of the file.
	out.open(fileName, std::ios::app);
	out << "time: " << time << " minStep: "<< minStep << endl;
}

