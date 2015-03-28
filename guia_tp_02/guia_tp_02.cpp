// guia_tp_02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace cimg_library;

void ejercicio_1(){
	std::vector<unsigned char> &LUT = CImgUtils::createLut255(4.0f, -2.0f);
	
	CImg<unsigned char> image("../guia_tp_01/cameraman.tif");



}

int _tmain(int argc, _TCHAR* argv[])
{
	ejercicio_1();

	return 0;
}

