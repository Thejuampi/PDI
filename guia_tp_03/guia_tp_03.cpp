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

void ejercicio_2_1() {
	static CImg < float > mascaraPromedio3x3(3, 3, 1, 1, 1.0f/9.0f); // imagen de 3x3 con todos 1.
	CImg < unsigned char > imagenOriginal("../guia_tp_01/img/imagenC.tif");

	auto imagenPromediada = imagenOriginal.get_convolve(mascaraPromedio3x3);

	CImgUtils::showImageAndWait(imagenPromediada);
	CImgUtils::showImageAndWait(imagenOriginal);

	CImg<float> mascaraPromedio2(3, 3, 1, 1,
		.0f, .2f, .0f,
		.2f, .2f, .2f,
		.0f, .2f, .0f);

	imagenPromediada = imagenOriginal.get_convolve(mascaraPromedio2);
	CImgUtils::showImageAndWait(imagenPromediada);
}

void ejercicio_2_2() {
	CImg<float> mascaraGausiana = CImgUtils::gaussFilter(3.0f, 1.0f);
	CImg < float > imagenOriginal("../guia_tp_01/img/imagenC.tif");
	auto imagenFiltrada = imagenOriginal.convolve(mascaraGausiana);
	imagenFiltrada.display();
}

void ejercicio_2_3() {
	CImg<float> mascara7x7(7,7, 1, 1, 1.0f / 9.0f);
	CImg < float > imagenOriginal("../guia_tp_01/img/hubble.tif");

	auto &imagenFiltrada = imagenOriginal.get_convolve(mascara7x7);
	imagenFiltrada.normalize(0.0f, 255.0f);
	
	auto &mascaraBinaria = CImgUtils::toBinary(imagenFiltrada);
	mascaraBinaria.normalize(0.0f, 1.0f);

	auto imagenResaltada = CImgUtils::multiplyImages(imagenOriginal, mascaraBinaria);
	imagenResaltada.display();
	imagenOriginal.display();

}

void ejercicio_3_1(){
	CImg<float> filtroPasaAlto(3, 3, 1, 1,
		 -0.0f, -1.0f,  -0.0f,
		 -1.0f,  8.0f, -1.0f,
		 -0.0f, -1.0f,  -0.0f
		);
	CImg < float > imagenOriginal("../guia_tp_01/img/cameraman.tif");
	int alto = imagenOriginal.height();
	int ancho = imagenOriginal.width();
	auto imagenFiltrada = imagenOriginal.get_convolve(filtroPasaAlto);
	//imagenFiltrada = (imagenFiltrada*0.1) + (imagenOriginal*0.9);
	CImgUtils::showImageAndWait(imagenFiltrada);
}

void ejercicio_4_1(){
	CImg<float> mascaraDifusa(3, 3, 1, 1, 1.0f / 9.0f);
	CImg<float> imagen("../guia_tp_01/img/cameraman.tif");
	CImg<float> &imagenDifusa = imagen.get_convolve(mascaraDifusa);
	CImg<float> &imagenMejorada = 3.0f*imagen - imagenDifusa;

	imagenMejorada.display();
}

int _tmain(int argc, _TCHAR* argv[])
{
	ejercicio_4_1();
	return 0;
}

