// guia_tp_05.cpp : Defines the entry point for the console application.
//
//#ifndef cimg_use_openmp
//#define cimg_use_openmp 1
//#endif
#include "stdafx.h"
using namespace cimg_library;

typedef void(*ejercicio)(void);

static const char* cameramanPath = "../guia_tp_01/img/cameraman.tif";

void draw_3D_image(CImg<double> imagen, const float sigma = 1.0f, const double ratioz = 1.0, const unsigned int di = 10){
	// Init data
	const CImg<double> img = imagen.blur(sigma).resize(-100, -100, 1, 3);
	const CImg<double> norm = img.get_norm().normalize(0, 255);

	CImgList<unsigned int> primitives;
	CImgList<double> colors;
	const CImg<> points = img.get_elevation3d(primitives, colors, norm*-ratioz);

	// Compute image isophotes.
	std::fprintf(stderr, "\n- Compute image isophotes"); std::fflush(stderr);
	CImgList<unsigned int> isoprimitives;
	CImgList<double> isocolors;
	CImg<> isopoints;

	for (unsigned int i = 0; i<255; i += di) {
		CImgList<> prims;
		const CImg<> pts = norm.get_isoline3d(prims, (float)i);
		isopoints.append_object3d(isoprimitives, pts, prims);
	}
	cimglist_for(isoprimitives, l) {
		const unsigned int i0 = isoprimitives(l, 0);
		const float x0 = isopoints(i0, 0), y0 = isopoints(i0, 1);
		const double
			r = (double)img.linear_atXY(x0, y0, 0),
			g = (double)img.linear_atXY(x0, y0, 1),
			b = (double)img.linear_atXY(x0, y0, 2);
		isocolors.insert(CImg<double>::vector(r, g, b));
	}
	cimg_forX(isopoints, ll) isopoints(ll, 2) = -ratioz*norm.linear_atXY(isopoints(ll, 0), isopoints(ll, 1));

	// Enter event loop
	std::fprintf(stderr, "\n- Enter interactive loop.\n\n"
		"Reminder : \n"
		" + Use mouse to rotate and zoom object\n"
		" + key 'F' : Toggle fullscreen\n"
		" + key 'Q' or 'ESC' : Quit\n"
		" + Any other key : Change rendering type\n\n"); std::fflush(stderr);
	const char *const title = "Image viewed as a surface";
	CImgDisplay disp(800, 600, title, 0);
	unsigned int rtype = 2;
	CImg<float> pose = CImg<float>::identity_matrix(4);

	while (!disp.is_closed()) {
		const double white[3] = { 255, 255, 255 };
		CImg<double> visu(disp.width(), disp.height(), 1, 3, 0);
		visu.draw_text(10, 10, "Render : %s", white, 0, 1, 19, rtype == 0 ? "Points" : (rtype == 1 ? "Lines" : (rtype == 2 ? "Faces" : (rtype == 3 ? "Flat-shaded faces" :
			(rtype == 4 ? "Gouraud-shaded faces" : (rtype == 5 ? "Phong-shaded faces" : "Isophotes"))))));
		if (rtype == 6) visu.display_object3d(disp, isopoints, isoprimitives, isocolors, true, 1, -1, true, 500.0f, 0.0f, 0.0f, -5000.0f, 0.0f, 0.0f, true, pose.data());
		else visu.display_object3d(disp, points, primitives, colors, true, rtype, -1, true, 500.0f, 0.0f, 0.0f, -5000.0f, 0.0f, 0.0f, true, pose.data());
		switch (disp.key()) {
		case 0: break;
		case cimg::keyBACKSPACE: rtype = (7 + rtype - 1) % 7; break;
		case cimg::keyQ:
		case cimg::keyESC: disp.close(); break;
		case cimg::keyF:
			if (disp.is_fullscreen()) disp.resize(800, 600); else disp.resize(disp.screen_width(), disp.screen_height());
			disp.toggle_fullscreen();
			break;
		default: rtype = (rtype + 1) % 7; break;
		}
	}
}


void ejercicio_1_1(){
	CImg<double>img("../guia_tp_01/img/cameraman.tif");
	//Transformada directa y obtenci´on de las partes real e imaginaria
	CImgList<double> &TDF_img = img.get_FFT();
	CImg<double> &TDF_real = TDF_img[0];
	CImg<double> &TDF_imaginaria = TDF_img[1];
	CImg<double> magnitud(256, 256, 1, 1);
	CImg<double> fase(256,256,1,1);

	cimg_forXY(magnitud, x, y) {
		magnitud(x, y) = sqrt(TDF_real(x, y)*TDF_real(x, y) + TDF_imaginaria(x, y)*TDF_imaginaria(x, y));
	}
	magnitud.log();
	magnitud.shift(128, 128, 0, 0, 2);
	//CImgUtils::showImageAndWait(magnitud);
	draw_3D_image(magnitud);
}

void pueba_parcial_1() {
	//CImg<double> imagen("../guia_tp_01/img/cameraman.tif");
	CImg<double> imagen(256, 256, 1, 1, 0);
	CImg<double> cuadroBlanco(40, 40, 1, 1, 255);
	CImgUtils::replaceSubRegion(imagen, cuadroBlanco, 128-cuadroBlanco.width()/2, 128-cuadroBlanco.height()/2);
	//imagen.display();
	int alto = imagen.height();
	int ancho = imagen.width();
	
	CImgUtils::showSpectrum(imagen.get_FFT(), true);

}

typedef CImg<double> cimgd;
typedef CImgList<double> cimgld;
void ejercicio_1_2(){
	double zero = 0.0;
	static const double color[] = { 255.0 };
	cimgd lineaVertical(256, 256, 1, 1, 0);
	lineaVertical.draw_line(128,0,128,256,color);
	cimgld &fft = lineaVertical.get_FFT();
	CImgUtils::showSpectrum(fft);

	cimgd cuadradoCentrado(256, 256, 1, 1, 0);
	CImgUtils::replaceSubRegion(cuadradoCentrado, cimgd(40,40,1,1,255), 128 - 20, 128 - 20);
	CImgUtils::showSpectrum(cuadradoCentrado.get_FFT());

	cimgd rectanguloCentrado(256, 256, 1, 1, 0);
	CImgUtils::replaceSubRegion(rectanguloCentrado, cimgd(120, 40, 1, 1, 255), 128 - 60, 128 - 20);
	CImgUtils::showSpectrum(rectanguloCentrado.get_FFT());

	cimgd &circulo = CImgUtils::drawCircle(int(512), int(512), int(16), zero);
	CImgUtils::showSpectrum(circulo.get_FFT(), true, false);
}

void ejercicio_1_3() {
	double zero = 0.0;
	static const double color[] = { 255.0 };
	cimgd lineaVertical(256, 256, 1, 1, 0);
	lineaVertical.draw_line(10, 0, 10, 256, color); // en ves del medio, esta a x = 10;
	cimgld &fft = lineaVertical.get_FFT();
	CImgUtils::showSpectrum(fft);

	cimgd cuadradoCentrado(256, 256, 1, 1, 0);
	CImgUtils::replaceSubRegion(cuadradoCentrado, cimgd(40, 40, 1, 1, 255), 20, 20); // en (x, y) = (20, 20)
	CImgUtils::showSpectrum(cuadradoCentrado.get_FFT());

	cimgd rectanguloCentrado(256, 256, 1, 1, 0);
	CImgUtils::replaceSubRegion(rectanguloCentrado, cimgd(120, 40, 1, 1, 255), 0, 0); // en 0,0
	CImgUtils::showSpectrum(rectanguloCentrado.get_FFT());

	cimgd &circulo = CImgUtils::drawCircle(int(512), int(512), int(64), zero);
	CImgUtils::showSpectrum(circulo.get_FFT(), true, false);
}

void ejercicio_1_4(){
	double color[] = { 255 };
	cimgd lineaVerticalCentrada(512, 512, 1, 1, 0);
	lineaVerticalCentrada.draw_line(256, 0, 256, 512, color);
	cimgd &lineaVerticalRotada = lineaVerticalCentrada.get_rotate(20, 0, 0);
	CImgUtils::showSpectrum(lineaVerticalRotada.get_FFT());

	lineaVerticalRotada.crop(219, 198, 474, 453);
	lineaVerticalCentrada.crop(128, 128, 383, 383);
	//lineaVerticalCentrada.display();
	//lineaVerticalRotada.display();

	CImgUtils::showSpectrum(lineaVerticalCentrada.get_FFT());
	CImgUtils::showSpectrum(lineaVerticalRotada.get_FFT());

}

void ejercicio_2_1(){
	cimgd imagen(cameramanPath);
	cimgd dummy(imagen.get_fill(0));
	cimgld &fft = imagen.get_FFT();
	cimgd &magnitud	= CImgUtils::getSpectrum(fft);
	//usar la magnitud como parte real de la fft, y dejar la parte imaginaria en 0 es lo que tengo que hacer.
	cimgld solo_modulo;
	solo_modulo.push_back(magnitud);
	solo_modulo.push_back(magnitud.get_fill(0));
	cimgd imagenSoloModulo = solo_modulo.get_FFT(true)[0]; // parte real, la imaginaria se descarta.

	//en este caso es diferente, porque tengo que crear la imagen, con la misma fase, pero con magnitud 1.
	cimgd real_solo_fase = fft[0];
	cimgd imag_solo_fase = fft[1];

	cimg_forXY(real_solo_fase, x, y){
		double &r = real_solo_fase(x, y);
		double &i = imag_solo_fase(x, y);
		double &mag = magnitud(x, y);
		//normalizo el numero complejo por su norma, para que quede mag 1.
		r /= mag;
		i /= mag;
	}

	cimgld solo_fase;
	solo_fase.push_back(real_solo_fase);
	solo_fase.push_back(imag_solo_fase);

	cimgd imagenSoloFase = solo_fase.get_FFT(true)[0]; // parte real, la imag se desprecia
	
	imagenSoloModulo.display("Modulo Igual, fase = 0");
	imagenSoloFase.display("Modulo unitario, fase queda igual");
}

ejercicio ejercicios[] = {
	ejercicio_1_1,
	ejercicio_1_2,
	ejercicio_1_3,
	ejercicio_1_4,
	ejercicio_2_1
};

int nEjercicios = 5;

int _tmain(int argc, _TCHAR* argv[])
{
	ejercicios[nEjercicios - 1]();
	return 0;
}

