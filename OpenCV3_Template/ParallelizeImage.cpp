#include "ParallelizeImage.hpp"
#include <thread>

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

void ParallelizeImage::ParallelizeConvolve(Mat inImg, ConvolutionMask conv)
{

}

void ParallelizeImage::partialConvolve(Mat inImg, Mat outImg, int imgMinX, int imgMinY, int imgMaxX, int imgMaxY, ConvolutionMask conv)
{	
	int maskEdge = (int)((conv.side - 1) / 2); //Distancia entre a borda da mascara e o centro
	int rightBottonEdge = 0;
	if (conv.side % 2 == 0) { //Se o lado da mascara tem tamanho par, o centro fica delocado para cima e para a esquerda
		rightBottonEdge = 1;
	}

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
					if (maskI < imgMaxX && maskJ < imgMaxY && maskI >= 0 && maskJ >=0 ) {
						float pixelvalue = (float)inImg.at<uchar>(maskI, maskJ);
						if (maskX < conv.side && maskY < conv.side && maskX >= 0 && maskY >= 0) {
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
}

void ParallelizeImage::sobelFilter(Mat inImg, Mat outImg)
{
	ParallelizeImage* parallel = new ParallelizeImage();
	ConvolutionMask* averageFilter = new ConvolutionMask(3, ConvolutionMask::AVERAGE);
	ConvolutionMask* horFilter = new ConvolutionMask(3, ConvolutionMask::SOBEL_HOR);
	ConvolutionMask* verFilter = new ConvolutionMask(3, ConvolutionMask::SOBEL_VER);
	Mat averageImg = inImg.clone();
	Mat horImg = inImg.clone();
	Mat verImg = inImg.clone();
	parallel->partialConvolve(inImg, averageImg, 0, 0, inImg.cols, inImg.rows, *averageFilter);

	parallel->partialConvolve(averageImg, horImg, 0, 0, inImg.cols, inImg.rows, *horFilter);

	parallel->partialConvolve(horImg, verImg, 0, 0, inImg.cols, inImg.rows, *verFilter);

	parallel->addAndBinarizeImgs(horImg, verImg, outImg);
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

void ParallelizeImage::convolve(Mat inImg, ConvolutionMask conv, bool parallel)
{
	thread threadsArray[3];

	if (isGrayImage(inImg)) {
		Mat clone = inImg.clone();
		partialConvolve(clone, inImg, 0, 0, inImg.cols, inImg.rows, conv);
	}
	else{
		Mat channels[3];
		split(inImg, channels);

		Mat clone = inImg.clone();
		Mat cloneChannels[3];
		split(clone, cloneChannels);

		if (parallel) {
			for (int i = 0; i < 3; i++) {

				//threadsArray[i] = thread(&ParallelizeImage::partialConvolve, cloneChannels[i], channels[i], 0, 0, inImg.cols, inImg.rows, conv);
				threadsArray[i] = thread([&](ParallelizeImage* parallel) {parallel->partialConvolve(cloneChannels[i], channels[i], 0, 0, inImg.cols, inImg.rows, conv); }, this);
			}

			for (int i = 0; i < 3; i++)
				threadsArray[i].join();
		}
		else {
			for (int i = 0; i < 3; i++) {
				partialConvolve(cloneChannels[i], channels[i], 0, 0, inImg.cols, inImg.rows, conv);
			}
		}

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
	
