#pragma once
#ifndef CONVOLUTIONMASK
#define CONVOLUTIONMASK

class ConvolutionMask {
public:
	enum MaskType {AVERAGE};
	ConvolutionMask();
	ConvolutionMask(int, MaskType);

private:
	int side;
	float** mask;

};

#endif