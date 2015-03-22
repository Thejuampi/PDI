#pragma once

#include <cmath> // fabs
#include <vector>
#include "CImg.h"
#include "TjfLogger.h"

class CImgUtils{

private:

protected:
	static inline void agregarPunto(int x, int y, std::vector<std::pair<int, int>> &vector){
		vector.push_back(std::pair<int, int>(x, y));
#ifdef _DEBUG
		static std::string callfrom = "agregarPunto()";
		std::stringstream ss;
		ss << "x=" << x << " y=" << y;
		TjfLogger::getInstance().log(callfrom, ss.str());
#endif
	}

public:

	template <class T> static inline int redondea(const T& value)
	{
		return (value>0) ? (int(value + .5)) : (int(value - .5));
	}

	template <class T> static inline void intercambia(T&t1, T&t2) //swap en conflicto con stl
	{
		T t0 = t1; t1 = t2; t2 = t0;
	} // cambia el contenido y no la direccion (lento pero seguro)

	void static lineaBresenham(int x1, int y1, int x2, int y2, std::vector<std::pair<int,int>> &puntos) { //no tiene divisiones comparado con DDA, y no tiene redondeos, es mas eficiente
		int dx = x2 - x1, dy = y2 - y1, x, y, NE, E, D, xstep, ystep;
		puntos.clear();
		if (!(dx || dy)) { 
			puntos.push_back(std::pair<int, int>(x1, y1));
			return;
		}

		if (abs(dx)>abs(dy)) { //|dx|>|dy| => tendencia horizontal
			if (dx<0) { intercambia(x1, x2); intercambia(y1, y2); dx = -dx; dy = -dy; } // de P2 a P1, intercambia para ir de P1 a P2
			x = x1; y = y1; NE = dx << 1; // (<<1 = *2, shift de un bit)
			ystep = 1; if (dy<0) { ystep = -1; dy = -dy; } //ystep = 1 sino doy vuelta; a y le sumo 1, sino le resto 1 para los siguientes puntos
			E = dy << 1; D = E - dx; //= 2*dy-dx
			while (x<x2) {
				agregarPunto(x, y, puntos);
				if (D>0) { y += ystep; D -= NE;/*D=2*dx*di+2*dy-dx; le resto 2dx siD>0*/ } // determina su subo(bajo) "y", x siempre avanza en 1
				x++; D += E; //E = 2dy
			}
		}
		else{//tendencia vertical
			if (dy<0) { intercambia(y1, y2); intercambia(x1, x2); dy = -dy; dx = -dx; } //de P2 a P1, blabla
			y = y1; x = x1; NE = dy << 1; // (<<1 = *2, shift de un bit)
			xstep = 1; if (dx<0) { xstep = -1; dx = -dx; }
			E = dx << 1; D = E - dy;
			while (y<y2) {
				agregarPunto(x, y, puntos);
				if (D>0) { x += xstep; D -= NE; } //determina si avanzo(retroceso en x, "y" siempre avanza en 1.
				y++; D += E;
			}
		}
		agregarPunto(x, y, puntos); // punto final
	}

	/*Retorna un unsigned char entre /param min y /param max
	Ojo que no chequea que max > min*/
	static inline unsigned char generateRandomChar(unsigned char min, unsigned char max) {
		//srand(time(NULL));
		unsigned char valr = min + rand() % (max - min);
#ifdef _DEBUG
		static std::string callFrom = "generateRandomChar()";
		unsigned v(valr);
		TjfLogger::getInstance().log(callFrom, v);
#endif
		return valr;
	}

	static inline void waitForWindow(cimg_library::CImgDisplay &imagen){
		while (!imagen.is_closed()) { imagen.wait(); }
	}

	static cimg_library::CImg<unsigned char> drawCircle(int width, int height, int circleRadius){
		cimg_library::CImg<unsigned char> image(width, height, 1, 1, 0);
		unsigned char white[] = { 255 };
		image.draw_circle(width/2, height/2, circleRadius, white);
		return image;
	}

};