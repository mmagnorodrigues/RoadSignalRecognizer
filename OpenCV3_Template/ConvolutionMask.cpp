#include "ConvolutionMask.hpp"
#include <cmath>

using namespace std;

ConvolutionMask::ConvolutionMask()
{
	this->side = 0;
	this->mask = nullptr;
}

ConvolutionMask::ConvolutionMask(int side, ConvolutionMask::MaskType maskType)
{
	this->mask = new float*[side];
	for (int i = 0; i < side; i++) {
		mask[i] = new float[side];
	}

	switch (maskType)
	{
	case ConvolutionMask::AVERAGE: {
		float averageType = 1 / ((float)side*(float)side);
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {

				mask[i][j] = averageType;
			}
		}
		this->side = side;
		break;
	}

	case ConvolutionMask::SOBEL_VER: {
		this->side = 3;
		mask[0][0] = 1;	mask[0][1] = 0;	mask[0][2] = -1;
		mask[1][0] = 2;	mask[1][1] = 0;	mask[1][2] = -2;
		mask[2][0] = 1;	mask[2][1] = 0;	mask[2][2] = -1;
		break;
	}
	case ConvolutionMask::SOBEL_HOR: {
		this->side = 3;
		mask[0][0] = -1; mask[0][1] = -2;	mask[0][2] = -1;
		mask[1][0] = 0;	 mask[1][1] = 0;	mask[1][2] = 0;
		mask[2][0] = 1;	 mask[2][1] = 2;	mask[2][2] = 1;

		break;
	}
	}
}
