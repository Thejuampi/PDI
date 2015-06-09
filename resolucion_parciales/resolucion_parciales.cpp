// resolucion_parciales.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../guia_tp_01/CImg.h"
#include "../guia_tp_01/CImgUtils.h"
#include "../guia_tp_01/TjpLogger.h"

using namespace std;
using namespace cimg_library;

typedef CImg<double> cimgd;
//typedef CImg<

/*
Rotacion completamente automatica de imagenes
*/
void prueba_1() {
	
	//const char* filename;// = cimg_option("-i", (char*)0, "Ruta y nombre de la imagen que sera cargada para corregir.\n");
	cimgd imagenOriginal("img/escaneo10.png");

	cimgd& imagenGrises = imagenOriginal.spectrum() == 3 ? CImgUtils::RGBtoGray(imagenOriginal) : cimgd(imagenOriginal);
	imagenGrises.normalize(0.0, 255.0);
	
	//MEJORAS VISUALES
	vector<double>& LUT = CImgUtils::createLut255(1.0f, 0.0f, 0.0, 255.0);
	for (int i = 160; i < 256; ++i) {
		LUT[i] = 255.0;
	}
	imagenGrises = CImgUtils::mapLUT(imagenGrises, LUT);
	imagenGrises = CImgUtils::powTransformation(imagenGrises, 3);
	imagenGrises.display();

	//PROCESAMIENTO
	cimgd& bordes = CImgUtils::detectorBordesSobelDiagonales(imagenGrises, 128.0);
	CImg<bool>& ee = CImgUtils::nuevoElementoEstructuranteCruz3x3();
	//bordes.dilate(ee);
	//bordes.erode(ee);
	bordes.display();


	//DETECCION DE BORDES
	CImg<double>& hough = CImgUtils::transformadaHough(bordes);
	hough = CImgUtils::recortarTransformadaHoughPorAngulo(hough, -30.0*M_PI / 180.0, 30.0*M_PI / 180.0);
	std::vector<Pixel<double>>& maximos = CImgUtils::obtenerLosMaximos(hough, 2, true, 10, 10);
	vector<double> angulos;
	for (Pixel<double>& pixel : maximos) {
		double angulo = CImgUtils::calcularAnguloHoughInverso(pixel, hough);
		angulos.push_back(angulo);
	}
	sort(angulos.begin(), angulos.end());


	double anguloMaximo = angulos[angulos.size() - 1];
	imagenOriginal.rotate( anguloMaximo * 180.0f / M_PI);
	imagenOriginal.display();

}

//Otra forma de auto rotar imagen
void prueba_2(){

	cimgd imagen("img/escaneo3.png");
	imagen.resize(512, 512);
	imagen = CImgUtils::RGBtoGray(imagen);
	CImgUtils::showSpectrum(imagen.get_FFT());
	
	cimgd& espectro = CImgUtils::getSpectrum(imagen.get_FFT());

	espectro.get_histogram(256).display_graph();
	//espectro.normalize(0.0, 1.0);
	//espectro = CImgUtils::logarithmicTransformation(espectro);
	//espectro.display();
	//cimgd& bordes = CImgUtils::detectorBordesSobelDiagonales(espectro);

	//bordes.display();

}


int _tmain(int argc, _TCHAR* argv[])
{

	prueba_1();
	return 0;
}

