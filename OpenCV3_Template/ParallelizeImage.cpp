#include "ParallelizeImage.hpp"

void ParallelizeImage::setPixel(Mat inImg, uchar red, uchar green, uchar blue, int x, int y)
{
	Point3_<uchar>* pixel = inImg.ptr<Point3_<uchar>>(x, y);
	uchar* r = (uchar*)&pixel->x;
	uchar* g = (uchar*)&pixel->y;
	uchar* b = (uchar*)&pixel->z;

	*r = red;
	*g = green;
	*b = blue;
}

void ParallelizeImage::ParallelizeConvolve(Mat inImg, ConvolutionMask conv)
{
	Mat clone = inImg.clone();

}

void ParallelizeImage::partialConvolve(Mat inImg, Mat outImg, int imgMinX, int imgMinY, int imgMaxX, int imgMaxY, ConvolutionMask conv)
{
	int maskEdge = (int)((conv.side - 1) / 2); //Distancia entre a borda da mascara e o centro
	std::cout << conv.side << std::endl;
	int rightBottonEdge = 0;
	if (conv.side % 2 == 0) { //Se o lado da mascara tem tamanho par, o centro fica delocado para cima e para a esquerda
		rightBottonEdge = 1;
	}

	int minMaskX, maxMaskX, minMaskY, maxMaskY;
	float totalRed, totalGreen, totalBlue;

	for (int imgI = imgMinX; imgI < imgMaxX; imgI++) {
		for (int imgJ = imgMinY; imgJ < imgMaxY; imgJ++) {
			totalRed = 0;
			totalGreen = 0;
			totalBlue = 0;
			minMaskX = imgI - maskEdge;
			minMaskY = imgJ - maskEdge;
			maxMaskX = imgI + maskEdge + rightBottonEdge;
			maxMaskY = imgJ + maskEdge + rightBottonEdge;
			/*std::cout << totalRed << std::endl;
			std::cout << totalGreen << std::endl;
			std::cout << totalBlue << std::endl;
			std::cout << minMaskX << std::endl;
			std::cout << minMaskY << std::endl;
			std::cout << maxMaskX << std::endl;
			std::cout << maxMaskY << std::endl;
			std::cout << "***********************" << std::endl;*/


			/*std::cout << "minMaskX " << minMaskX << std::endl;
			std::cout << "maxMaskX" << maxMaskX << std::endl;*/

			for (int maskI = minMaskX; maskI <= maxMaskX; maskI++) {
				for (int maskJ = minMaskY; maskJ <= maxMaskY; maskJ++) {
					int maskX = maskI - imgI;
					int maskY = maskJ - imgJ;
					if (maskI < imgMaxX && maskJ < imgMaxY && maskI >= 0 && maskJ >=0 ) {
						Vec3b color = inImg.at<Vec3b>(maskI, maskJ);
						if (maskX < conv.side && maskY < conv.side && maskX >= 0 && maskY >= 0) {
							float maskValue = conv.mask[maskX][maskY];
							totalRed += (float)color.val[0] * (float)maskValue;
							totalGreen += (float)color.val[1] * (float)maskValue;
							totalBlue += (float)color.val[2] * (float)maskValue;

							//std::cout << totalBlue << std::endl;
						}
						else {
						}
					}
					else {
					}
				}
			}
			ParallelizeImage::setPixel(outImg, (uchar)totalRed, (uchar)totalGreen, (uchar)totalBlue, imgI, imgJ);
		}
	}
	imshow("LennaConvo", outImg);
}

void ParallelizeImage::print(string string)
{
	std::cout << string << std::endl;
}
	
