#include "ParallelizeImage.hpp"
#include <thread>
#include "Barrier.hpp"

void ParallelizeImage::setPixelrgb(Mat inImg, uchar red, uchar green, uchar blue, int x, int y)
{
	Point3_<uchar>* pixel = inImg.ptr<Point3_<uchar>>(x, y);
	uchar* r = (uchar*)&pixel->x;
	uchar* g = (uchar*)&pixel->y;
	uchar* b = (uchar*)&pixel->z;

	*r = red;
	*g = green;
	*b = blue;
}

void ParallelizeImage::setPixelGray(Mat inImg, uchar value, int x, int y)
{
	*inImg.ptr(x, y) = value;
}

void ParallelizeImage::parallelizeConvolve(Mat inImg, ConvolutionMask conv, bool par,int minStep)
{	
	if (par) {
		float parallelizingRatio = 0.001;
		int stepX = (int)(inImg.cols * parallelizingRatio);
		int stepY = (int)(inImg.rows * parallelizingRatio);

		if (stepX < minStep) {
			stepX = minStep;
		}
		if (stepY < minStep) {
			stepY = minStep;
		}

		std::vector<thread*> threadsVector;
		Barrier* bar = new Barrier();

		Mat clone = inImg.clone();
		thread* convolutor;
		int maxX, maxY, i;
		for (i = 0; i < (inImg.cols + stepX); i += stepX) {
			if (i + stepX > inImg.cols) {
				maxX = inImg.cols;
			}
			else {
				maxX = i + stepX;
			}
			for (int j = 0; j < (inImg.rows + stepY); j += stepY) {

				if (j + stepY > inImg.rows) {
					maxY = inImg.rows;
				}
				else {
					maxY = j + stepY;
				}
				threadsVector.push_back(new thread([&](ParallelizeImage* parallel) {parallel->partialConvolve(clone, inImg, i, j, maxX, maxY, conv, bar); }, this));
			}
		}

		bar->wait();

		for (int i = 0; i < threadsVector.size(); i++) {
			threadsVector.at(i)->join();
		}
	}
	else{
		Barrier* bar = new Barrier();
		Mat clone = inImg.clone();
		partialConvolve(clone, inImg, 0, 0, clone.cols, clone.rows, conv, bar);
	}
}

void ParallelizeImage::partialConvolve(Mat inImg, Mat outImg, int imgMinX, int imgMinY, int imgMaxX, int imgMaxY, ConvolutionMask conv, Barrier* bar)
{
	try {
		int barrierIndex = bar->assign();

		int maskEdge = (int)((conv.side - 1) / 2); //Distancia entre a borda da mascara e o centro
		int rightBottonEdge = 0;
		if (conv.side % 2 == 0) { //Se o lado da mascara tem tamanho par, o centro fica delocado para cima e para a esquerda
			rightBottonEdge = 1;
		}

		int wholeImgLimitX = inImg.cols - 1;
		int wholeImgLimitY = inImg.cols - 1;

		int minMaskX, maxMaskX, minMaskY, maxMaskY;
		float totalValue;
		for (int imgI = imgMinX; imgI < imgMaxX; imgI++) {
			for (int imgJ = imgMinY; imgJ < imgMaxY; imgJ++) {
				totalValue = 0;
				minMaskX = imgI - maskEdge;
				minMaskY = imgJ - maskEdge;
				maxMaskX = imgI + maskEdge + rightBottonEdge;
				maxMaskY = imgJ + maskEdge + rightBottonEdge;
				for (int maskI = minMaskX; maskI <= maxMaskX; maskI++) {
					for (int maskJ = minMaskY; maskJ <= maxMaskY; maskJ++) {
						int maskX = maskI - imgI + maskEdge;
						int maskY = -(maskJ - imgJ - maskEdge);
						if (maskI < wholeImgLimitX && maskJ < wholeImgLimitY && maskI >= 0 && maskJ >= 0) {
							float pixelvalue = (float)inImg.at<uchar>(maskI, maskJ);
							if (maskX <= conv.side && maskY <= conv.side && maskX >= 0 && maskY >= 0) {
								float maskValue = conv.mask[maskX][maskY];
								float convValue = pixelvalue * maskValue;
								totalValue += convValue;
							}
							else {
							}
						}
						else {
						}
					}
				}
				if (totalValue < 0)
					totalValue *= -1;

				if (totalValue > 255)
					totalValue = 255;

				ParallelizeImage::setPixelGray(outImg, (uchar)totalValue, imgI, imgJ);
			}
		}

		bar->checkout(barrierIndex);
	}
	catch (int e) {
		std::cout << "erro: "<< e << std::endl;
	}
}

void ParallelizeImage::sobelFilter(Mat inImg, Mat outImg, bool par, int minStep)
{
	ConvolutionMask* averageFilter = new ConvolutionMask(3, ConvolutionMask::AVERAGE);
	ConvolutionMask* horFilter = new ConvolutionMask(3, ConvolutionMask::SOBEL_HOR);
	ConvolutionMask* verFilter = new ConvolutionMask(3, ConvolutionMask::SOBEL_VER);

	Mat averageImg = inImg.clone();
	this->convolve(averageImg, *averageFilter, par, minStep);

	Mat horImg = averageImg.clone();
	thread* horThread = new thread([&](ParallelizeImage* parallel) {parallel->convolve(horImg, *horFilter, par, minStep); }, this);
	 
	Mat verImg = averageImg.clone();
	thread* verThread = new thread([&](ParallelizeImage* parallel) {parallel->convolve(verImg, *verFilter, par, minStep); }, this);

	(*horThread).join();
	(*verThread).join();

	this->addAndBinarizeImgs(horImg, verImg, outImg);
}

void ParallelizeImage::addAndBinarizeImgs(Mat img1, Mat img2, Mat outImg)
{
	int limiar = 300;
	float red, green, blue;
	for (int x = 0; x < img1.cols; x++) {
		for (int y = 0; y < img1.rows; y++) {
			Vec3b color1 = img1.at<Vec3b>(x, y);
			Vec3b color2 = img1.at<Vec3b>(x, y);
			red = (float)color1.val[0] + (float)color2.val[0];
			green = (float)color1.val[1] + (float)color2.val[1];
			blue = (float)color1.val[2] + (float)color2.val[2];

			if ((red + blue + green) >= 255) {
				red = 255;
				green = 255;
				blue = 255;
			}
			else {
				red = 0;
				green = 0;
				blue = 0;
			}
			setPixelrgb(outImg, (uchar)red, (uchar)green, (uchar)blue, x, y);
		}
	}
}

void ParallelizeImage::print(string string)
{
	std::cout << string << std::endl;
}

void ParallelizeImage::showImage(Mat img, int windowSizeX, int windowSizeY, string title)
{
	namedWindow(title, WINDOW_NORMAL);
	resizeWindow(title, windowSizeX, windowSizeY);
	imshow(title, img);
}

void ParallelizeImage::convolve(Mat inImg, ConvolutionMask conv, bool parallel, int minStep)
{
	thread threadsArray[3];

	if (isGrayImage(inImg)) {
		Mat clone = inImg.clone();
		parallelizeConvolve(inImg, conv, parallel, minStep);
	}
	else{
		Mat channels[3];
		split(inImg, channels);

		Mat clone = inImg.clone();
		Mat cloneChannels[3];
		split(clone, cloneChannels);

		if (parallel) {
			for (int i = 0; i < 3; i++)
				threadsArray[i] = thread([&](ParallelizeImage* parallel) {parallel->parallelizeConvolve(channels[i], conv, parallel, minStep); }, this);
			for (int i = 0; i < 3; i++)
				threadsArray[i].join();
		}
		else 
			for (int i = 0; i < 3; i++)
				parallelizeConvolve(channels[i], conv, parallel, minStep);
		merge(channels, 3, inImg);
	}
}


bool ParallelizeImage::isGrayImage(Mat img)
{
	if (img.channels() > 1)
		return false;
	else {
		return true;
	}
}
	
