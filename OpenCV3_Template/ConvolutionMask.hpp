#pragma once
#ifndef CONVOLUTIONMASK
#define CONVOLUTIONMASK

class ConvolutionMask {
public:
	enum MaskType {AVERAGE};
	ConvolutionMask();
	ConvolutionMask(int side, ConvolutionMask::MaskType maskType);
	
	int side;
	float** mask;

};

#endif