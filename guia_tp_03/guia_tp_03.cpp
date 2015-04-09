// guia_tp_03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace cimg_library;

void ejercicio_1_1(){
	CImg < unsigned char > imagenOriginal("../guia_tp_01/img/imagenD.tif");
	CImg < unsigned char > histogramaOriginal = imagenOriginal.get_histogram(256, 0, 255);

	CImg < unsigned char > imagenEqualizada = imagenOriginal.get_equalize(256, 0, 255);
	CImg < unsigned char > histogramaEqualizado = imagenEqualizada.get_histogram(256, 0, 255);

	histogramaOriginal.display_graph();
	histogramaEqualizado.display_graph();

	imagenOriginal.display();
	imagenEqualizada.display();

}

void ejercicio_1_2() {
	TjpLogger &log = TjpLogger::getInstance();
	CImg < unsigned char > imagenOriginal("../guia_tp_01/img/imagenC.tif");
	CImg < unsigned char > histogramaOriginal = imagenOriginal.get_histogram(256, 0, 255);
	histogramaOriginal.display_graph();
	const static char* ejercicio2Minimo = "ejercicio_2() : minimo de la imagen";
	int minimo = (int)CImgUtils::min(imagenOriginal);
	log.log(ejercicio2Minimo, minimo);
}

void ejercicio_3() {

}

int _tmain(int argc, _TCHAR* argv[])
{
	ejercicio_1_2();
	return 0;
}

