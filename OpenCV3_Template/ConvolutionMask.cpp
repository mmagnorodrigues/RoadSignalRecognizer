#include "ConvolutionMask.h"
#include <cmath>

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
	case ConvolutionMask::AVERAGE:
		int average = 1 / pow(side, 2);
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				mask[i][j] = average;
			}
		}
		break;
	default:
		break;
	}
}
