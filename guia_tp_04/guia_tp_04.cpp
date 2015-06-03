// guia_tp_04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../guia_tp_01/CImg.h"
#include "../guia_tp_01/CImgUtils.h"

using namespace cimg_library;
typedef void(*ejercicio)(void);
typedef CImg<double> cimgd;
typedef CImg<float> cimgf;
typedef std::vector<std::vector<float>> FloatMatrix;

void ejercicio_1_a(){
	cimgd patron("../guia_tp_01/img/patron.tif");

	cimgd& rojo = patron.get_channel(0);
	cimgd& verde = patron.get_channel(1);
	cimgd& azul = patron.get_channel(2);

	(patron,rojo, verde, azul).display();
}

void ejercicio_1_b(){
	cimgf patron("../guia_tp_01/img/patron.tif");
	cimgf& patronHSI = patron.get_RGBtoHSI();

	cimgf& hue = patronHSI.get_channel(0);
	cimgf& saturation = patronHSI.get_channel(1);
	cimgf& intensity = patronHSI.get_channel(2);

	(patron, hue, saturation, intensity).display("Visualización: Patron - HUE - SAT - INT");

	/***************Segunda parte*****************/

	/*
	* Rojo = 0 grados = 0 en Cimg
	* Verde = 120 grados = 80 en Cimg
	* Azul = 240 = 160 en Cimg
	*/

	/*Para pasar de azul a rojo hay que cambiar el hue = matiz de 240(160) a 0*/

	cimg_forXY(patronHSI, x, y){
		float& hueValue = patronHSI(x, y, 0, 0); // z = 0; canal = 0 = hue;
		float& saturationValue = patronHSI(x, y, 0, 1);
		float& intensityValue = patronHSI(x, y, 0, 2);
		//if (hueValue >= 10-240.0f/255.0f && hueValue <= 10 + 240.0/255.0f) hueValue = 0;
		hueValue = patronHSI(patronHSI.width()-1-x, y, 0, 0); // falla porque esta sobreescribiendo sobre los mismos datos de donde saca la info.
		saturationValue = 1;
		intensityValue = 1;

	}
	patron = patronHSI.get_HSItoRGB();
	patron.display();

}

void ejercicio_2(){
	FloatMatrix paleta;
	CImgUtils::cargarPaleta("../guia_tp_01/img/paletas/cool.pal", paleta);
	cimgf imagenCameraman("../guia_tp_01/img/cameraman.tif");
	cimgf imagenDegrade(256, 256, 1, 1);
	
	cimg_forXY(imagenDegrade, x, y) {
		imagenDegrade(x, y) = x;
	}
	cimgf paleteada, degradePaleteada;
	CImgUtils::aplicarPaleta(paleta, imagenCameraman, paleteada);
	CImgUtils::aplicarPaleta(paleta, imagenDegrade, degradePaleteada);
	(paleteada, degradePaleteada).display();
}

void ejercicio_3() {
	cimgf rio("../guia_tp_01/img/rio.jpg");
	rio.get_histogram(255).display_graph("Histograma");

	cimgf rioColor;
	float colorBuscado[] = { 0.0f };
	int radio = 40;
	float colorNuevo[] = { 255.0f, 255.0f, 0.0f };

	CImgUtils::aplicarColor(rio, rioColor, colorBuscado, radio, colorNuevo);

	(rio, rioColor).display();

}

void ejercicio_4() {
	cimgf chairsOscura("../img/chairs_oscura.jpg");
	cimgf chairsOriginal("../img/chairs.jpg");
	cimgf& equalizada = chairsOscura.get_equalize(255);
	cimgf& chairsHSI = chairsOscura.get_RGBtoHSI();
	
	cimgf& intensidad = chairsHSI.get_channel(2);

	intensidad.equalize(256);

	cimg_forXY(chairsHSI, x, y){
		chairsHSI(x, y, 0, 2) = intensidad(x, y);
	}

	(chairsOriginal, chairsOscura, equalizada, chairsHSI.get_HSItoRGB()).display("Original - Oscura - RGB - HSI");

}

void ejercicio_4_2() {
	cimgf caminoOriginal("../img/camino.tif");
	cimgf mask = CImgUtils::new3x3SharpKernel();
	cimgf& caminoHSI = caminoOriginal.get_RGBtoHSI();

	caminoOriginal.convolve(mask);
	cimgf& canalIntensidad = caminoHSI.get_channel(2);

	canalIntensidad.convolve(mask);

	cimg_forXY(caminoHSI, x, y) {
		caminoHSI(x, y, 0, 2) = canalIntensidad(x, y);
	}

	caminoHSI.HSItoRGB();

	(caminoOriginal, caminoHSI).display("Filtrado: RGB Vs HSI");

}

void ejercicio_5() {
	cimgf img("../img/futbol.jpg");
	cimgf imgModificada;
	
	/*cimgf& canalRojo = img.get_channel(0);
	cimgf& canalVerde = img.get_channel(1);
	cimgf& canalAzul = img.get_channel(2);

	canalRojo.get_histogram(256).display_graph("ROJO");
	canalVerde.get_histogram(256).display_graph("VERDE");
	canalAzul.get_histogram(256).display_graph("AZUL");*/

	float colorBuscado[] = {40.0f, 90.0f, 200.0f};
	float colorNuevo[] = { 255.0f, 255.0f, 0.0f };
	int radio = 90;
	CImgUtils::aplicarColor(img, imgModificada, colorBuscado, radio, colorNuevo);

	imgModificada.display("COLOR MODFICADO");

}

void ejercicio_5_2() {
	cimgf img("../img/futbol.jpg");
	cimgf imgModificada;
	float buscado[] = { 120.0f, 1.0f };
	int radio = 30;
	float nuevo[] = { 240, 0.75f };
	CImgUtils::aplicarColorHSI(img, imgModificada, buscado, radio, nuevo);

	imgModificada.display("MODIFICADA");

}

ejercicio ejercicios[] = {
	ejercicio_1_a,
	ejercicio_1_b,
	ejercicio_2,
	ejercicio_3,
	ejercicio_4,
	ejercicio_4_2,
	ejercicio_5,
	ejercicio_5_2
};

int _tmain(int argc, _TCHAR* argv[])
{
	ejercicios[7]();
	return 0;
}

