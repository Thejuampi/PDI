// guia_tp_02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace cimg_library;

void ejercicio_1(){
	static unsigned char color[] = { 255 };
	std::vector<unsigned char> &LUT = CImgUtils::createLut255(); // por defecto, la identidad
	
	CImg<unsigned char> originalImage("../guia_tp_01/img/cameraman.tif");

	CImg<unsigned char> curva = CImgUtils::new2DImageUchar(255, 255);
	CImgDisplay ventanaCurva(255, 255, "Curva");
	CImgDisplay ventanaImagen(originalImage);
	curva.draw_line(0, 255, 255, 0, color);
	ventanaCurva.display(curva);
	ventanaCurva.wait();
	while (!ventanaCurva.is_closed()) {
		if (ventanaCurva.button() & 1) { // boton izquierdo
			unsigned char x = ventanaCurva.mouse_x();
			unsigned char y = ventanaCurva.mouse_y();
			y = 255 - y;
			Punto<unsigned char> puntoCliqueado(x, y);
			CImg<unsigned char> editedImage(originalImage); // copia?
			CImgUtils::addRange(puntoCliqueado, editedImage, LUT, curva);
			ventanaCurva.display(curva);
			ventanaImagen.display(editedImage);
			//ventanaCurva.wait(50);
			ventanaCurva.set_button(); //resetea el mouse.
		}
		else {
			ventanaCurva.wait();
		}
	}
	
}

void ejercicio_2(){
	CImg<unsigned char> originalImage("../guia_tp_01/img/rmn.jpg");

	CImg<unsigned char> powImage(originalImage);
	CImg<unsigned char> logImage(originalImage);
	CImgUtils::logarithmicTransformation(logImage, 1024.0f);
	CImgUtils::powTransformation(powImage, 3.0f);
	CImgList<unsigned char> imagesList;
	imagesList.push_back(originalImage);
	imagesList.push_back(powImage);
	imagesList.push_back(logImage);

	CImgDisplay ventana;
	ventana.display(imagesList);
	CImgUtils::waitForWindow(ventana);

}

void ejercicio_3(){

	CImg<unsigned char> originalImage("../guia_tp_01/img/cameraman.tif");
	std::vector<CImg<unsigned char>> images;
	images.reserve(100);
	for (unsigned counter = 0; counter < 100; ++counter){
		images.push_back(CImgUtils::addNoise(originalImage, 0.0f, 255.0f*0.05f));
	}

	CImg<unsigned char> &modifiedImage = CImgUtils::reduceNoise(images);

	CImgDisplay originalImageWindow(originalImage);
	CImgDisplay modifiedImageWindow(modifiedImage);

	originalImageWindow.display(originalImage);
	modifiedImageWindow.display(modifiedImage);

	while (!originalImageWindow.is_closed() && !modifiedImageWindow.is_closed()){
		originalImageWindow.wait();
		modifiedImageWindow.wait();
	}

}

/*Filtro Emboss*/
void ejercicio_4(){
	CImg<unsigned char> originalImage("../guia_tp_01/img/cameraman.tif");
	CImg<unsigned char> embossFilterImage = CImgUtils::embossFilter(originalImage, 1, 1);

	CImgDisplay displayOriginal = CImgUtils::showImage(originalImage, "Original");
	CImgDisplay displayFiltered = CImgUtils::showImage(embossFilterImage, "Filtrada");
	
	CImgUtils::waitForWindow(displayOriginal);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//ejercicio_1();
	ejercicio_4();

	return 0;
}

