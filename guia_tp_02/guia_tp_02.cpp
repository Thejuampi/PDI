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
			ventanaCurva.wait(50);
			ventanaCurva.set_button(); //resetea el mouse.
		}
		else {
			ventanaCurva.wait();
		}
	}
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	ejercicio_1();

	return 0;
}

