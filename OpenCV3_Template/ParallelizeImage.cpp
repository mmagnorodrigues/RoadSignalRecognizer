#include "ParallelizeImage.hpp"

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
	Mat clone = inImg.clone();

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
			red = color1.val[0] + color2.val[0];
			green = color1.val[1] + color2.val[1];
			blue = color1.val[2] + color2.val[2];

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


bool ParallelizeImage::isGrayImage(Mat img)
{
	Mat dst;
	Mat bgr[3];
	split(img, bgr);
	absdiff(bgr[0], bgr[1], dst);

	if (countNonZero(dst))
		return false;

	absdiff(bgr[0], bgr[2], dst);
	return !countNonZero(dst);
}

	
