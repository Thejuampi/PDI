// guia_tp_07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../guia_tp_01/CImg.h"
#include "../guia_tp_01/CImgUtils.h"

using namespace cimg_library;

typedef CImg<float> cimgf;
typedef CImg<double> cimgd;
typedef CImg<unsigned char> cimguc;


void ejercicio_1(){
	cimgd imagen("img/estanbul.tif");

	cimgd& conRoberts = CImgUtils::detectorBordesRoberts(imagen, 40.0);
	cimgd& conPrewitt = CImgUtils::detectorBordesPrewitt(imagen, 40.0);
	cimgd& conSobel = CImgUtils::detectorBordesSobel(imagen, 40.0);

	(imagen, conRoberts, conPrewitt, conSobel).display("Original, roberts, prewitt, sobel");
}


void ejercicio_2() {
	cimgd imagen("img/estanbul.tif");
	cimgd& hough = CImgUtils::transformadaHough(imagen);
	std::vector<Pixel<double> >& maximos = CImgUtils::obtenerLosMaximos(hough, 1);
	//std::stringstream ss;
	cimgd& imagenConMaximos = hough.get_fill(0.0);

	for (Pixel<double>& max : maximos) {
		imagenConMaximos(max.x, max.y, 0, 0) = max.value;
	}
	cimgd& imagenInversa = CImgUtils::transformadaHough(imagenConMaximos, true);
	//imagenInversa.display("INVERSA");
	(imagen, imagenInversa, imagen + imagenInversa).display("Original, Lineas, Original+Lineas");

}

void ejercicio_2_2() {
	//cimgd letras1("img/snowman.png");
	cimgd letras1("img/letras2.tif");
	letras1.resize_doubleXY();
	letras1 = CImgUtils::detectorBordesSobelDiagonales(letras1, 80.0);
	letras1.resize_halfXY();
	cimgd& hough1 = CImgUtils::transformadaHough(letras1);
	std::vector<Pixel<double> >& maximos = CImgUtils::obtenerLosMaximos(hough1, 6, true, 15,15);
	cimgd& imagenConMaximos = hough1.get_fill(0.0);

	for (Pixel<double>& max : maximos) {
		imagenConMaximos(max.x, max.y, 0, 0) = max.value;
	}
	cimgd& imagenInversa = CImgUtils::transformadaHough(imagenConMaximos, true);
	(letras1, imagenInversa, letras1 + imagenInversa).display("Original, Lineas, Original+Lineas");

#if 0
	cimgd letras2("img/letras2.tif");
	cimgd& hough2 = CImgUtils::transformadaHough(letras2);
	std::vector<Pixel<double> >& maximos2 = CImgUtils::obtenerLosMaximos(hough2, 5);
	//std::stringstream ss;
	cimgd& imagenConMaximos2 = hough2.get_fill(0.0);

	for (Pixel<double>& max : maximos2) {
		imagenConMaximos2(max.x, max.y, 0, 0) = max.value;
	}
	cimgd& imagenInversa2 = CImgUtils::transformadaHough(imagenConMaximos2, true);
	//imagenInversa.display("INVERSA");
	(letras2, imagenInversa2, letras2 + imagenInversa2).display("Original, Lineas, Original+Lineas");
#endif
}

int _tmain(int argc, _TCHAR* argv[])
{
	ejercicio_2_2();
	return 0;
}

