// guia_tp_08.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../guia_tp_01/CImg.h"
#include "../guia_tp_01/CImgUtils.h"


using namespace std;
using namespace cimg_library;

typedef CImg<double> cimgd;
typedef CImg<float> cimgf;
typedef CImg<bool> cimgb;
typedef CImg<unsigned char> cimguc;


void ejercicio_1() {
	cimgb imagen(11, 12, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	);
	const int nee = 5;
	cimgb ee[nee] = {
		cimgb(3, 3, 1, 1, true),
		cimgb(3,3,1,1, 0,1,0,1,1,1,0,1,0),
		cimgb(3,3,1,1, 0,1,0,0,1,1,0,0,0),
		cimgb(3,3,1,1, 0,1,1,0,1,1,0,1,1),
		cimgb(3,3,1,1, 0,0,1,0,0,1,0,0,1)
	};
	imagen.display();
	cimgb dilatadas[5] = {};

	for (int i = 0; i < nee; ++i) {
		dilatadas[i] = imagen.get_dilate(ee[i], 1);
	}

	int c = 0;
	(dilatadas[c++],
		dilatadas[c++],
		dilatadas[c++],
		dilatadas[c++],
		dilatadas[c++]).display("dilatadas");
	
	cimgb erosionadas[5] = {};

	for (int i = 0; i < nee; ++i) {
		erosionadas[i] = imagen.get_erode(ee[i], 1);
	}
	c = 0;
	(erosionadas[c++],
		erosionadas[c++],
		erosionadas[c++],
		erosionadas[c++],
		erosionadas[c++]).display("erosionadas");
}

void ejercicio1_n(){

	cimgd imagend("img/letras2.tif");
	cimgb imagen = imagend.quantize(2,false);
	const int nee = 5;
	cimgb ee[nee] = {
		cimgb(3, 3, 1, 1, true),
		cimgb(3, 3, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0),
		cimgb(3, 3, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0),
		cimgb(3, 3, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1),
		cimgb(3, 3, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1)
	};
	imagen.display();
	cimgb dilatadas[5] = {};

	for (int i = 0; i < nee; ++i) {
		dilatadas[i] = imagen.get_dilate(ee[i], 1);
	}

	int c = 0;
	(dilatadas[c++],
		dilatadas[c++],
		dilatadas[c++],
		dilatadas[c++],
		dilatadas[c++]).display("dilatadas");

	cimgb erosionadas[5] = {};

	for (int i = 0; i < nee; ++i) {
		erosionadas[i] = imagen.get_erode(ee[i], 1);
	}
	c = 0;
	(erosionadas[c++],
		erosionadas[c++],
		erosionadas[c++],
		erosionadas[c++],
		erosionadas[c++]).display("erosionadas");
}


void ejercicio_2() {
	cimgb imagen(17, 9, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		),
		ee(3, 3, 1, 1, 1);

	cimgb abierto = CImgUtils::morfologiaApertura(imagen, CImgUtils::nuevoElementoEstructuranteCruz3x3());
	cimgb cerrado = CImgUtils::morfologiaCierre(imagen, CImgUtils::nuevoElementoEstructuranteCruz3x3());

	(abierto, cerrado).display(); //muestra como unas alas

	abierto = CImgUtils::morfologiaApertura(imagen, ee);
	cerrado = CImgUtils::morfologiaCierre(imagen, ee);

	(abierto, cerrado).display(); //no entiendo bien el resultado
	
}

void ejercicio_3_1(){
	cimgb original("img/tarjeta.png");
	original = original.get_channel(0);
	cimgb tarjeta = original;
	
	CImg<bool> ee(3, 3, 1, 1, 
		0,1,0,
		1,1,1,
		0,1,0 );

	tarjeta = CImgUtils::complemento(tarjeta);
	original = CImgUtils::complemento(original);
	tarjeta.erode(ee);
	//tarjeta.erode(ee);
	tarjeta.dilate(ee);
	tarjeta.dilate(ee);

	original = CImgUtils::AND(original, tarjeta);

	original.display();

}

void ejercicio_n1(){

	cimgb ee(3, 3, 1, 1,
		0, 0, 1,
		0, 1, 0,
		1, 0, 0
		);

	cimguc fugaz("img/fugaz.png");
	cimguc& fugazGray = CImgUtils::RGBtoGray(fugaz);
	//fugazGray.display();
	fugazGray.threshold(60).normalize(0, 1);
	//fugazGray.display();
	fugazGray.erode(ee);
	//fugazGray.display();
	fugazGray.dilate(ee);
	//fugazGray.display();
	//TODO : corregir los operadores logicos.
	fugaz = CImgUtils::AND(fugaz, fugazGray);

	fugaz.display();

}

void ejercicio_n2() {
	cimgd estrellas("img/estrellas.png");
	cimgd& estrellasGray = CImgUtils::RGBtoGray(estrellas);
	CImgUtils::mostrarHistograma(estrellasGray);
	estrellasGray.threshold(130).normalize(0,1);
	estrellasGray.erode(CImgUtils::nuevoElementoEstructuranteCruz15x15());
	//estrellasGray.erode(CImgUtils::nuevoElementoEstructuranteCruz15x15());
	//estrellasGray.display();
	cimg_forXYC(estrellas,x,y,c){
		estrellas(x, y, 0, c) = estrellas(x, y, 0, c) * estrellasGray(x, y);
	}

	estrellas.display();
}



int _tmain(int argc, _TCHAR* argv[])
{

	ejercicio_n2();
	return 0;
}

