#pragma once

/*

TODO:

-COLORES: RETORNAR AZUL, ROJO, VERDE, ETC PARA USAR EN RGB O EN HSI
-FILTROS: IMPLEMENTARLOS POR SU NOMBRE

*/

#include <algorithm>    // std::sort
#include <cmath> // fabs
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <sstream>
#include <random>
#include "CImg.h"
#include "TjpLogger.h"

using namespace cimg_library;

template<class T>
struct Pixel {
	int x;
	int y;
	T value;

	template<class T>
	friend inline bool operator<(const Pixel<T> &m0, const Pixel<T> &m1) {
		return m0.value < m1.value;
	}
};

template <class T = unsigned char> class Punto{
private:
	T _x;
	T _y;
public:
	Punto(T x, T y) :_x(x), _y(y){}
	Punto() :_x(-1), _y(-1){}
	T x(){ return this->_x; }
	T y(){ return this->_y; }

	template <class T>
	friend inline bool operator<(const Punto<T> &p0, const Punto<T> &p1) {
		return p0._x < p1._x;
	}
#if 0
	bool operator ==(const Punto<T> &p0){ // simil java equals, p nunca es nulo
		return _x == p0.x() && _y == p.y();
	}
	bool operator !=(const Punto<T> &p0){
		return _x != p0.x() || _y != p.y();
	}

	bool operator<(const Punto<T> &p0) {
		return _x < p0.x();
	}
	bool operator<=(const Punto<T> &p0) {
		return _x <= p0.x();
	}
#endif
};
//template <typename T>
//struct PuntoComparator{
//	bool operator< (const Punto<T> &p0, const Punto<T> &p1) {
//		return p0.x() < p1.x();
//	}
//};
//
//bool std::operator<(const Punto<unsigned char> &p0, const Punto<unsigned char> &p1){
//	return p0.x() < p1.x();
//}

class CImgUtils{

private:

protected:
	static inline void agregarPunto(int x, int y, std::vector<std::pair<int, int>> &vector){
		vector.push_back(std::pair<int, int>(x, y));
#ifdef _DEBUG
		static std::string callfrom = "agregarPunto()";
		std::stringstream ss;
		ss << "x=" << x << " y=" << y;
		TjpLogger::getInstance().log(callfrom, ss.str());
#endif
	}

public:

	template <class T> static inline T min(const T &v1, const T &v2){
		return v2 < v1 ? v2 : v1;
	}

	template <class T> static inline T max(const T &v1, const T &v2){
		return v2 > v1 ? v2 : v1;
	}

	template <class T> static inline int redondea(const T& value)
	{
		return (value > 0) ? (int(value + .5)) : (int(value - .5));
	}

	template <class T> static inline void intercambia(T&t1, T&t2) //swap en conflicto con stl
	{
		T t0 = t1; t1 = t2; t2 = t0;
	} // cambia el contenido y no la direccion (lento pero seguro)

	template <class T> static inline T min(CImg<T> &image){
		T minimo = T(255);
		cimg_forXY(image, x, y){
			if (image(x, y) < minimo) minimo = (image(x, y));
		}
		return minimo;
	}

	template <class T> static inline CImg<T> new2DImage(unsigned int width, unsigned int height, unsigned spectrum){
		return CImg<T>(width, height, 1, spectrum, T(0));
	}

	static inline CImg<unsigned char> new2DImageUchar(unsigned int width, unsigned int height){
		return new2DImage<unsigned char>(width, height, 1);
	}

	void static lineaBresenham(int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>> &puntos) { //no tiene divisiones comparado con DDA, y no tiene redondeos, es mas eficiente
		int dx = x2 - x1, dy = y2 - y1, x, y, NE, E, D, xstep, ystep;
		puntos.clear();
		if (!(dx || dy)) {
			puntos.push_back(std::pair<int, int>(x1, y1));
			return;
		}

		if (abs(dx) > abs(dy)) { //|dx|>|dy| => tendencia horizontal
			if (dx < 0) { intercambia(x1, x2); intercambia(y1, y2); dx = -dx; dy = -dy; } // de P2 a P1, intercambia para ir de P1 a P2
			x = x1; y = y1; NE = dx << 1; // (<<1 = *2, shift de un bit)
			ystep = 1; if (dy < 0) { ystep = -1; dy = -dy; } //ystep = 1 sino doy vuelta; a y le sumo 1, sino le resto 1 para los siguientes puntos
			E = dy << 1; D = E - dx; //= 2*dy-dx
			while (x < x2) {
				agregarPunto(x, y, puntos);
				if (D > 0) { y += ystep; D -= NE;/*D=2*dx*di+2*dy-dx; le resto 2dx siD>0*/ } // determina su subo(bajo) "y", x siempre avanza en 1
				x++; D += E; //E = 2dy
			}
		}
		else{//tendencia vertical
			if (dy < 0) { intercambia(y1, y2); intercambia(x1, x2); dy = -dy; dx = -dx; } //de P2 a P1, blabla
			y = y1; x = x1; NE = dy << 1; // (<<1 = *2, shift de un bit)
			xstep = 1; if (dx < 0) { xstep = -1; dx = -dx; }
			E = dx << 1; D = E - dy;
			while (y < y2) {
				agregarPunto(x, y, puntos);
				if (D > 0) { x += xstep; D -= NE; } //determina si avanzo(retroceso en x, "y" siempre avanza en 1.
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
		TjpLogger::getInstance().log(callFrom, v);
#endif
		return valr;
	}

	/*Deja una ventana ociosa a la espera de un evento. Sirve principalmente para que no se cierre.*/
	static inline void waitForWindow(CImgDisplay &imagen){
		static std::string callFrom("waitForWindow()");
		TjpLogger::getInstance().log(callFrom, "Esperando Evento...");
		while (!imagen.is_closed()) { imagen.wait(); }
	}

	/*Crea una CImg y le dibuja un círculo en el medio*/
	template <typename T>
	inline static CImg<T> drawCircle(int width, int height, int circleRadius, T &tipo = T(0)){
		static T white[] = { T(255) };
		if (width*height <= 0) throw std::logic_error("Alto o ancho icorrecto/s, revisar los valores");
		CImg<T> image(width, height, 1, 1, 0);
		image.draw_circle(width / 2, height / 2, circleRadius, white);
		return image;
	}

	/*Reduce el tamaño de la imagen a la mitad. Descarta pixeles intermedios*/
	template <class T = unsigned char>
	static inline CImg<T> subSampleBy2(CImg<T> &image) {
		CImg<T> subsampled(image.width() / 2, image.height() / 2, image.depth(), image.spectrum(), 0);
		for (int x = 0; x < image.width(); x += 2){
			for (int y = 0; y < image.height(); y += 2){
				subsampled(x / 2, y / 2) = image(x, y);
			}
		}
		return subsampled;
	}
	/*Reemplaza una sub region de la imagen por otra. Ojo que no chequea limites*/
	template <class T>
	static inline void replaceSubRegion(CImg<T> &destiny, CImg<T> &source, int x_from, int y_from) {
		for (int x = x_from; x < min(source.width() + x_from, destiny.width()); ++x){
			for (int y = y_from; y < min(source.height() + y_from, destiny.height()); ++y){
				destiny(x, y) = source(x - x_from, y - y_from);
			}
		}
	}

	/*Crea una imagen de medios tonos. No modifica la original*/
	template<class T>
	static CImg<T> halfToning(CImg<T> &imagen){
		CImg<T> &halfToned = imagen.get_resize(imagen._width / 3, imagen._height / 3);
		//transform(imagen, T(100), T(200));
		imagen.log();
		halfToned.quantize(10, false);
		std::map<T, CImg<T>> mapa;
		CImg<T> img(3, 3, 1, 1, 0);

		mapa.insert(std::pair<T, CImg<T>>(T(0), img));

		img(1, 0) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(1), img));

		img(2, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(2), img));

		img(0, 0) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(3), img));

		img(0, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(4), img));

		img(2, 0) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(5), img));

		img(2, 1) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(6), img));

		img(1, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(7), img));

		img(0, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(8), img));

		img(0, 2) = T(255);
		mapa.insert(std::pair<T, CImg<T>>(T(9), img));

		halfToned.resize(imagen.width(), imagen.height());

		for (int x = 0; x < halfToned.width(); x += 3){
			for (int y = 0; y < halfToned.height(); y += 3){
				T &key = halfToned(x, y);
				CImg<T> &value = mapa.at(key);
				replaceSubRegion(halfToned, value, x, y);
			}
		}

		CImgDisplay display;
		display.display(halfToned);
		display.show();

		waitForWindow(display);

		return halfToned;

	}

	/*NO USAR*/
	template <class T>
	CImg<T> brightness(CImg<T> &image, float scale){
		return image * scale;
	}

	/*Transforma una imagen, a partir de un valor de inicio (aplica 0 hasta dicho valor) y un valor final(todo lo superior va a 255)*/
	template <class T>
	static void transform(CImg<T> &image, T riseStart, T riseEnd){
		if (riseEnd < riseStart){
			throw std::logic_error("Error: riseEnd < riseStart");
		}
		float m = 255.0f / (riseEnd - riseStart);
		float b = 255.0f*riseStart / (riseStart - riseEnd);
		cimg_forXY(image, x, y){
			T &val = image(x, y);
			if (val < riseStart){
				val = T(0);
			}
			else if (val > riseEnd){
				val = T(255);
			}
			else {
				val = T(b + m*val); //transform
			}
		}
	}

	/*Crea una ventana y muestra la imagen que se pasa como parametro. EL titulo es opcional. Retorna la ventana*/
	template <class T> static CImgDisplay showImage(CImg<T> &image, const char* title = ""){
		CImgDisplay display;
		display.display(image);
		display.set_title(title);
		return display;
	}

	/*Transforma linealmente un pixel, no modifica el original*/
	template <class T> static T linearTransformation(T &value, float gain = 1.0f, float offset = 0.0f){
		T val = value;
		float fVal = float(val);
		fVal = gain*fVal + offset;
		if (fVal < 0.0f) fVal = 0.0f;
		if (fVal > 255.0f) fVal = 255.0f;
		val = T(redondea(fVal));
		return val;
	}

	/*Transforma linealmente una imagen. No modifica la imagen original*/
	template <class T> static CImg<T> linearTransformation(CImg<T> &source, float gain = 1.0f, float offset = 0.0f){
		CImg<T> copy = source; //copia?
		cimg_forXY(copy, x, y){
			T &val = copy(x, y);
			float fVal = float(val);
			fVal = gain*fVal + offset;
			if (fVal < 0.0f) fVal = 0.0f;
			if (fVal > 255.0f) fVal = 255.0f;
			val = T(redondea(fVal));
		}
		return copy;
	}
	/*Aplica la transformacion logarítimica a una imagen*/
	template <class T> static inline CImg<T> logarithmicTransformation(CImg<T> &image){
		CImg <float> floatImage(image);
		floatImage.normalize(0.0f, 1.0f);
		cimg_forXY(floatImage, x, y){
			floatImage(x, y) = log(floatImage(x, y) + 1.0f);
		}
		floatImage.normalize(0.0f, 255.0f);
		/*image = floatImage;*/
		cimg_forXY(floatImage, x, y){
			image(x, y) = T(redondea(floatImage(x, y)));
		}
		return image;
	}

	template <class T> static inline CImg<T> powTransformation(CImg<T> &image, float exp){
		CImg <float> floatImage(image);
		floatImage.normalize(0.0f, 1.0f);
		cimg_forXY(floatImage, x, y){
			floatImage(x, y) = pow(floatImage(x, y), exp);
		}
		floatImage.normalize(0.0f, 255.0f);
		/*image = floatImage;*/
		cimg_forXY(floatImage, x, y){
			image(x, y) = T(redondea(floatImage(x, y)));
		}
		return image;
	}

	template <class T> static inline CImg<T> XOR(CImg<T> &left, CImg<T> &right) {
		CImg<T> &result = left.get_fill(0);
		if (righ.size() != left.size()) {
			right.resize(left);
		}
		cimg_forXY(result, x, y){
			result(x, y) = (left(x, y) & 1) ^ (right(x, y) & 1) ? T(255) : T(0);
		}
		return result;
	}

	template <class T> static inline CImg<T> AND(CImg<T> &left, CImg<T> &right) {
		CImg<T> &result = left.get_fill(0);
		if (right.size() != left.size()) {
			right.resize(left);
		}
		cimg_forXY(result, x, y){
			result(x, y) = left(x, y) && right(x, y) ? T(255) : T(0);
		}
		return result;
	}

	template <class T> static inline CImg<T> OR(CImg<T> &left, CImg<T> &right) {
		CImg<T> &result = left.get_fill(0);
		if (righ.size() != left.size()) {
			right.resize(left);
		}
		cimg_forXY(result, x, y){
			result(x, y) = left(x, y) || right(x, y) ? T(255) : T(0);
		}
		return result;
	}

	/*Modifica una LUT*/
	template <class T = unsigned char>
	static void alterLUTLinear(std::vector<T> &LUT, const float gain = 1.0f, const float offset = 0.0f, T start = T(0), T end = T(255)){
		if (start == end) return;
		if (end < start) intercambia(start, end);
		if (start < T(0)) start = T(0);
		if (end > T(255)) end = T(255);
		for (T x = start; x < end; ++x){
			LUT[x] = linearTransformation(x, gain, offset);
		}
		//No se puede hacer el <= porque se pasa de rango al hacer 255+1 = 0
		LUT[end] = linearTransformation(end, gain, offset);
#ifdef _DEBUG
		static std::string c("alterLut()");
		std::stringstream ss;
		for (auto i : LUT){
			ss << int(i) << ", ";
		}
		TjpLogger::getInstance().log(c, ss.str());
#endif
		//return LUT;
	}

	/*Crea una LUT, con ganancia, offset, inicio del rango y fin del rango */
	template <class T>
	static inline std::vector<T> createLut255(float gain = 1.0f, float offset = 0.0f, T start = T(0), T end = T(255)) {
		if (end < start) intercambia(start, end);
		if (start < T(0)) start = T(0);
		if (end > T(255)) end = T(255);
		int range = abs(end - start) + 1;
		std::vector<T> result(range);
		alterLUTLinear(result, gain, offset, start, end);
		return result;
	}

	/*Mapea una LUT sobre una imagen y retorna la imagen resultante. No altera la imagen Original*/
	template <class T = unsigned char> static inline CImg<T> mapLUT(CImg<T> &source, std::vector<T> &LUT) {
		CImg<T> copy = source;
		cimg_forXY(source, x, y){
			T &val = copy(x, y);
			val = LUT[val];
		}
		return copy;
	}
	/*Dados dos puntos, calcula la pendiente de la linea que los une*/
	template <class T = unsigned char> static inline float calcularGanancia(Punto<T> actual, Punto<T> next) {
		float pend = float(next.y() - actual.y()) / float(next.x() - actual.x());
		return pend;
	}

	/*Dados dos puntos, calcula la constante de la ecuación de la recta de los une*/
	template <class T = unsigned char> static inline float calcularOffset(Punto<T> actual, Punto<T> next){
		//c=(x2*y1 - x1*y2) / (x2 - x1)
		float x1 = float(actual.x());
		float x2 = float(next.x());
		float y1 = float(actual.y());
		float y2 = float(next.y());
		float offset = (x2*y1 - x1*y2) / (x2 - x1);
		return offset;
	}
	/*TODO add doc*/
	template <class T = unsigned char> static inline void addRange(Punto<T> &valorActual, CImg<T> &image, std::vector<T> &LUT, CImg<T> &curve){
		static std::set<Punto<T>> rangos = { Punto<T>(T(0), T(0)), Punto<T>(T(255), T(255)) }; // se crea una ves. Ojo, no es para nada thread safe.

		std::set<Punto<T>>::iterator rangoIterator = rangos.find(valorActual);

		if (rangoIterator == rangos.end()){ // no tiene el valor -> modificar la curva
			rangos.insert(valorActual); //ordena automaticamente de menor a mayor.
			std::set<Punto<T>>::iterator valorInsertadoIterator = rangos.find(valorActual);
			std::set<Punto<T>>::iterator copiaMas = valorInsertadoIterator;
			std::set<Punto<T>>::iterator copiaMenos = valorInsertadoIterator;

			//std::set<Punto<T> >::iterator valorInsertadoIterator = rangos.insert(punto); //ordena automaticamente de menor a mayor.

			//Punto<T> &valorActual = punto;
			const Punto<T> &valorAnterior = *(--copiaMenos);
			const Punto<T> &valorSiguiente = *(++copiaMas);
			/*Tengo que modificar la curva anterior y la siguiente al punto actual*/
			/*Curva anterior, no confundir con el valor que tenia previamente, sino con la curva que esta antes.*/
			float pendienteAnterior = calcularGanancia(valorAnterior, valorActual);
			float offsetAnterior = calcularOffset(valorAnterior, valorActual);
			T x_anterior = const_cast<Punto<T>&>(valorAnterior).x();
			T x_actual = const_cast<Punto<T>&>(valorActual).x();
			if (x_actual > x_anterior)
				alterLUTLinear(LUT, pendienteAnterior, offsetAnterior, x_anterior, x_actual);
			/*Curva siguiente*/
			float pendienteSiguiente = calcularGanancia(valorActual, valorSiguiente);
			float offsetSiguiente = calcularOffset(valorActual, valorSiguiente);
			T x_siguiente = const_cast<Punto<T>&>(valorSiguiente).x();
			if (x_siguiente > ++x_actual)
				alterLUTLinear(LUT, pendienteSiguiente, offsetSiguiente, x_actual, x_siguiente);

			/*Segunda etapa, mapear de nuevo el LUT a la imagen. TODO: optimizar para que solo mapee los segmentos nuevos*/
			image = mapLUT(image, LUT);

			/*Tercera etapa, redibujar la curva*/
			redrawCurve(rangos, curve);
#if 0
#endif
		} // else -> no hacer nada, porque el valor ya esta.

	}

protected:
	template <class T> static inline void redrawCurve(std::set<Punto<T>> &rangos, CImg<T> &image) {
		static unsigned char color[] = { 255 };
		image.fill(T(0));
		std::set<Punto<T>>::iterator actualPoint = rangos.begin();
		if (actualPoint == rangos.end()) return;
		std::set<Punto<T>>::iterator nextPoint = actualPoint;
		++nextPoint;
		if (nextPoint == rangos.end()) return;
		while (nextPoint != rangos.end()){
			const Punto<T> &p0 = (*actualPoint++);
			const Punto<T> &p1 = (*nextPoint++);
			image.draw_line(const_cast<Punto<T>&>(p0).x(),
				255 - const_cast<Punto<T>&>(p0).y(),
				const_cast<Punto<T>&>(p1).x(),
				255 - const_cast<Punto<T>&>(p1).y(),
				color);
		}
#ifdef _DEBUG
		static std::string c("redrawCurve()");
		TjpLogger::getInstance().log(c, "Guardando imagen de curva");
		image.save_bmp("click.bmp");
#endif
	}
public:
	/*Suma pixel a pixel el valor de la imagen derecha sobre el valor de la imagen izquierda. Nunca se sale de rango [0,255]*/
	template<class T> static inline CImg<T> addImages(CImg<T> &leftSideImage, CImg<T> &rightSideImage){
		CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		CImg<T> rightCopy(rightSideImage);

		if (result.width() != rightCopy.width() || result.height() != rightCopy.height()){
			rightCopy.resize(result);
		}

		cimg_forXY(result, x, y){
			float val = result(x, y);
			val += rightCopy(x, y);
			if (val > 255.0f || val < 0.0f){
				TjpLogger::getInstance().log("addImages()", "Valor fuera de rango. Corregido");
				val = val > 255.0f ? val - 255.0f : 255.0f - val; // doy la vuelta.
			}
			val /= 2;
			result(x, y) = T(redondea(val));
		}
		return result;
	}

	/*Resta pixel a pixel el valor de la imagen derecha sobre el valor de la imagen izquierda. Nunca se sale de rango [0,255]*/
	template <class T> static inline CImg<T> substractImages(CImg<T> &leftSideImage, CImg<T> &rightSideImage){
		CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		CImg<T> rightCopy(rightSideImage);

		if (result.width() != rightCopy.width() || result.height() != rightCopy.height()){
			rightCopy.resize(result);
		}

		cimg_forXY(result, x, y){
			float val = result(x, y);
			val -= rightCopy(x, y);
			val /= 2;
			if (val > 255.0f || val < 0.0f){
				TjpLogger::getInstance().log("addImages()", "Valor fuera de rango. Corregido");
				val = val > 255.0f ? val - 255.0f : 255.0 - val;
			}
			result(x, y) = T(redondea(val));
		}
		return result;
	}

	/*Multiplica pixel a pixel el valor de la imagen derecha sobre el valor de la imagen izquierda. Nunca se sale de rango [0,255]*/
	template <class T> static inline CImg<T> multiplyImages(CImg<T> &leftSideImage, CImg<T> &rightSideImage){
		CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		CImg<T> rightCopy(rightSideImage);

		if (result.width() != rightCopy.width() || result.height() != rightCopy.height()){
			rightCopy.resize(result);
		}

		cimg_forXY(result, x, y){
			float val = result(x, y);
			val *= rightCopy(x, y);
			//val /= 255.0f; //normalization
#ifdef _DEBUG
			if (val > 255.0f || val < 0.0f){
				TjpLogger::getInstance().log("multiplyImages()", "Valor fuera de rango. Corregido");
				val = val > 255.0f ? 255.0f : 0.0f;
			}
#endif
			result(x, y) = T(redondea(val));
		}
		return result;
	}

	/*Multiplica la inversa de la segunda imagen (derecha) sobre la primera (izquierda).*/
	template <class T> static inline CImg<T> divideImages(CImg<T> &leftSideImage, CImg<T> &rightSideImage){
		//CImg<T> result(leftSideImage); // se podria pasar por copia directamente, pero es para mantener el formato
		CImg<T> rightCopy(rightSideImage);

		if (leftSideImage.width() != rightCopy.width() || leftSideImage.height() != rightCopy.height()){
			rightCopy.resize(leftSideImage);
		}

		std::vector<unsigned char> LUT = createLut255(-1.0f, 255.0f); //r = -1*s-255 : inversa.
		mapLUT(rightCopy, LUT);
		return multiplyImages(leftSideImage, rightCopy);
	}

	/*Convierte una imagen en binaria a partir de un valor de umbral. No altera la imagen original*/
	template <class T> static inline CImg<T> toBinary(const CImg<T> &image, T umbral = T(127), T min = T(0), T max = T(255)){
		CImg<T> copy(image);
		cimg_forXY(copy, x, y){
			copy(x, y) = copy(x, y) > umbral ? max : min;
		}
		return copy;
	}

	/*Agrega ruido de distribución normal, con media = mean y desviación = deviation, no altera la imagen original*/
	template <class T> static inline CImg<T> addNoise(const CImg<T> &image, float mean, float deviation) {
		static std::default_random_engine generator;
		std::normal_distribution<float> distribution(mean, deviation);
		CImg<float> noiseMap(image.width(), image.height(), 1, 1, 0.0f);
		cimg_forXY(noiseMap, x, y){
			float noise = distribution(generator) + float(image(x, y));
			if (noise < 0.0f) noise = 0.0f;
			if (noise > 255.0f) noise = 255.0f;
			noiseMap(x, y) = noise - image(x, y);
		}

		CImg<T> copy(image);
		cimg_forXY(copy, x, y){
			copy(x, y) = copy(x, y) + T(noiseMap(x, y));
		}
		return copy;
	}

	/*TODO add doc, supone que todas la imagenes tienen el mismo tamaño*/
	template <class T> static inline CImg<T> reduceNoise(std::vector<CImg<T> > &images) {
		int width = images.begin()->width();
		int height = images.begin()->height();
		CImg<double> sum(width, height, 1, 1, 0.0);
		if (images.size() > 0){

			for (auto &image : images){
				sum += image;
			}

			sum /= images.size();
#ifdef _DEBUG
			sum.save_bmp("sum.bmp");
#endif
		}
		return CImg<T>(sum);
	}

	template <class T> static inline CImg<T> moverImagen(CImg<T> &image, int deltaX = 1, int deltaY = 0){
		CImg<T> &result = image.get_fill(0); // para que tenga el mismo tamaño

		for (int x = deltaX; x < image.width(); ++x){
			for (int y = deltaY; y < image.height(); ++y){
				result(x, y) = image(x - deltaX, y - deltaY);
			}
		}

		return result;
	}

	template <class T> static inline CImg<T> embossFilter(CImg<T> &image, int deltaX = 1, int deltaY = 0) {
		std::vector<unsigned char> &LUT = createLut255(-1.0f, 255.0f);
		CImg<T> semi_inverse = mapLUT(image, LUT);
		semi_inverse = moverImagen(semi_inverse, deltaX, deltaY);
		return addImages(image, semi_inverse);
	}
	/*
	bit 7 => 1000 0000 = 128
	bit 6 => 0100 0000 = 64
	...
	bit 0 = > 0000 0001 = 1
	*/
	template <class T> static inline CImg<T> toBit(CImg<T> &image, unsigned char bit = 0){
		bit = bit % 8; // para que no se pase de largo.
		CImg<T> result(image);
		unsigned char mascara = (1 << bit);
		result &= mascara;
		return result;
	}

	template <class T> static inline float meanSquaredError(const CImg<T> &actual, const CImg<T> &aprox) {
		CImg<T> aproxCorrected = aprox.size() == actual.size() ? aprox.get_resize(actual) : aprox;
		float mse = 0.0;
		cimg_forXY(actual, x, y) {
			float aux = fabs(float(actual(x, y)) - float(aproxCorrected(x, y)));
			mse += aux*aux;
		}
		return (mse / actual.size()) / actual.spectrum();
	}

	template <class T> static inline void showImageAndWait(CImg<T> &image, char* title = ""){
		CImgUtils::waitForWindow(CImgUtils::showImage(image, title));
	}

	/*pasar por parametro los valores donde se quiere retornar el calculo*/
	static void getCentroid(int x0, int y0, int x1, int y1, int &cx, int &cy) {
		if (x0 > x1) intercambia(x0, x1);
		if (y0 > y1) intercambia(y0, y1);
		cx = x0 + (x1 - x0) / 2;
		cy = y0 + (y1 - y0) / 2;
	}

	template < class T > static float getDistance(CImg<T> &left, CImg<T> &right) {
		float result = 0.0f;

		cimg_forXY(left, x, y){
			float aux = float(left(x, y)) - float(right(x, y));
			result += aux*aux;
		}

		return result;
	}
#define M_PI 3.14159265358979323846f

	template<typename T>
	static inline CImg<T> gaussFilter(T sigma = T(1.0f), T deriv = T(0)) {
		T width = 3 * sigma;               // may be less width?
		T sigma2 = sigma*sigma;
		CImg<T> filter;
		filter.assign(int(2 * width) + 1);

		T i = 0;
		for (T x = -width; x <= width; x += 1.0f) {
			T g = T(exp(-0.5*x*x / sigma2)) / sqrt(2 * M_PI) / sigma;
			if (deriv == 1) g *= -x / sigma2;
			if (deriv == 2) g *= (x*x / sigma2 - 1.0f) / sigma2;
			filter(i) = T(g);
			i++;
		}
		return filter;
	}

	/**
	* Copiado desde las funciones de la catedra, modificado para funcionar con double. Optimizado ligeramente
	*/
	inline static void draw_3D_image(const CImg<double> &imagen, const double sigma = 1.0f, const double ratioz = 1.0, const unsigned int di = 10){
		// Init data
		CImg<double> &img = imagen.get_blur(sigma).get_resize(-100, -100, 1, 3);
		CImg<double> &norm = img.get_norm().get_normalize(0, 255);

		CImgList<unsigned int> primitives;
		CImgList<double> colors;
		const CImg<> &points = img.get_elevation3d(primitives, colors, norm*-ratioz);

		// Compute image isophotes.
		std::fprintf(stderr, "\n- Compute image isophotes"); std::fflush(stderr);
		CImgList<unsigned int> isoprimitives;
		CImgList<double> isocolors;
		CImg<> isopoints;

		for (unsigned int i = 0; i < 255; i += di) {
			CImgList<> prims;
			const CImg<> &pts = norm.get_isoline3d(prims, (float)i);
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
		CImg<float> &pose = CImg<float>::identity_matrix(4);

		while (!disp.is_closed()) {
			const double white[3] = { 255, 255, 255 };
			CImg<double> visu(disp.width(), disp.height(), 1, 3, 0);
			visu.draw_text(10, 10, "Render : %s", white, 0, 1, 19, rtype == 0 ? "Points" : (rtype == 1 ? "Lines" : (rtype == 2 ? "Faces" : (rtype == 3 ? "Flat-shaded faces" :
				(rtype == 4 ? "Gouraud-shaded faces" : (rtype == 5 ? "Phong-shaded faces" : "Isophotes"))))));
			if (rtype == 6){
				visu.display_object3d(disp, isopoints, isoprimitives, isocolors, true, 1, -1, true, 500.0f, 0.0f, 0.0f, -5000.0f, 0.0f, 0.0f, true, pose.data());
			}
			else {
				visu.display_object3d(disp, points, primitives, colors, true, rtype, -1, true, 500.0f, 0.0f, 0.0f, -5000.0f, 0.0f, 0.0f, true, pose.data());
			}
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



	template <class T = double>
	inline static CImg<T> getSpectrum(CImgList<T> &fft) {
		if (fft.size() != 2) {
			throw std::runtime_error("fft debe poseer dos componentes");
		}
		CImg<T> &real = fft(0);
		CImg<T> &imag = fft(1);
		int ancho = real.width();
		int alto = real.height();
		CImg<T> &magnitud = real.get_fill(T(0));
		cimg_forXY(magnitud, x, y) {
			T &r = real(x, y); T &i = imag(x, y);
			magnitud(x, y) = r*r + i*i;
		}
		return magnitud.sqrt();
	}


	template <class T = double>
	inline static void showSpectrum(CImgList<T> &fft, boolean center = true, boolean save = false) {
		CImg<T> &magnitud = CImgUtils::getSpectrum(fft);
		int alto = magnitud.height();
		int ancho = magnitud.width();
		magnitud += 1;
		magnitud.log();
		magnitud.normalize(0, 255);
		if (center) {
			magnitud.shift(ancho / 2, alto / 2, 0, 0, 2);
		}
		if (save) {
			magnitud.save_bmp("magnitud.bmp");
		}
		magnitud.display();
	}

	template <class T> inline static void showPhase(CImgList<T> &fft) {
		CImg<T> &real = fft(0);
		CImg<T> &imag = fft(1);
		int alto = real.width();
		int ancho = real.height();
		CImg<T> phase(real.get_fill(T(0)));
		cimg_forXY(phase, x, y) {
			T &r = real(x, y); T &i = imag(x, y);
			phase(x, y) = atan2(i, r);
		}
		phase.display();
	}

	template <class T>
	inline static CImg<T> getPhase(CImgList<T> &fft){
		CImg<T> &real = fft(0);
		CImg<T> &imag = fft(1);
		int alto = real.width();
		int ancho = real.height();
		CImg<T> phase(real.get_fill(T(0)));
		cimg_forXY(phase, x, y) {
			T &r = real(x, y); T &i = imag(x, y);
			phase(x, y) = atan2(i, r);
		}
		return phase;
	}

	template <class T>
	inline static CImg<double> getModuloOnly(CImgList<T> &fft, bool aplicarLogaritmo = false){
		CImg<double> &magnitud = CImgUtils::getSpectrum(fft);
		//usar la magnitud como parte real de la fft, y dejar la parte imaginaria en 0 es lo que tengo que hacer.
		CImgList<double> solo_modulo;
		solo_modulo.push_back(magnitud);
		solo_modulo.push_back(magnitud.get_fill(0));
		cimgd imagenSoloModulo = solo_modulo.get_FFT(true)[0]; // parte real, la imaginaria se descarta.
		if (aplicarLogaritmo) {
			imagenSoloModulo += 1.0;
			imagenSoloModulo.log();
		}
		return imagenSoloModulo;
	}
	template <class T>
	inline static CImg<double> getModuloUnitarioFaseIgual(CImgList<double> &fft) {
		cimgd real_solo_fase = fft[0]; //TODO ver si anda usando referencias, ahoar no tengo tiempo
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

		return imagenSoloFase;
	}

	template <class T>
	static inline CImg<T> filtroMediana(CImg<T> &imagen, unsigned int vecindad) {
		CImg<T> copia(imagen);
		//if (vecindad % 2 != 0) ++vecindad;
		vecindad /= 2;
		cimg_forXY(copia, x, y) {
			CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
			vecindario.sort();
			T& val = vecindario(vecindad);
			copia(x, y) = val;
		}
		return copia.normalize(T(0), T(255));
	}

	template <class T>
	static inline CImg<T> filtroPuntoMedio(CImg<T> imagen, unsigned int vecindad) {
		CImg<T> copia(imagen);

		vecindad /= 2;
		if (copia.spectrum() == 1) {//GRISES
			cimg_forXY(copia, x, y) {
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				T max, min;
				min = vecindario.min_max(max);
				copia(x, y) = (max - min) / 2;
			}
		}
		else if (copia.spectrum() == 3) {
			cimg_forXY(copia, x, y) {
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				T min[3], max[3];
				min[0] = vecindario.get_channel(0).min_max(max[0]);
				min[1] = vecindario.get_channel(1).min_max(max[1]);
				min[2] = vecindario.get_channel(2).min_max(max[2]);
				copia(x, y, 0, 0) = (max[0] - min[0]) / 2;
				copia(x, y, 0, 1) = (max[1] - min[1]) / 2;
				copia(x, y, 0, 2) = (max[2] - min[2]) / 2;
			}
		}
		return copia;
	}

	/*CUIDADO, usar solo con float o double, no esta diseñado para que ande con enteros*/
	template <class T>
	static inline CImg<T> filtroMediaAritmetica(CImg<T> &imagen, unsigned int vecindad) {
		CImg<T> copia(imagen);
		if (vecindad % 2 != 0) ++vecindad;
		T data = 1.0 / (vecindad*vecindad);
		CImg<T> kernel(vecindad, vecindad, 1, 1, data);
		copia.convolve(kernel);
		return copia.normalize(T(0), T(255));
	}

	template <class T>
	static inline CImg<T> filtroMediaGeometrica(CImg<T> imagen, unsigned int vecindad){
		CImg<T> copia(imagen);
		//if (vecindad % 2 != 0) ++vecindad;
		vecindad /= 2;
		if (copia.spectrum() == 1) { //GRISES
			cimg_forXY(copia, x, y){
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				int vecindarioSize = vecindario.size();
				T value = T(1);
				cimg_forXY(vecindario, s, t){
					value *= vecindario(s, t);
				}
				copia(x, y) = pow(value, 1.0 / vecindarioSize);
			}
		}
		else if (copia.spectrum() == 3) { //RGB
			cimg_forXY(copia, x, y){
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				int vecindarioSize = vecindario.size() / 3;
				T value[] = { T(1), T(1), T(1) };
				cimg_forXY(vecindario, s, t){
					value[0] *= vecindario(s, t, 0, 0);
					value[1] *= vecindario(s, t, 0, 1);
					value[2] *= vecindario(s, t, 0, 2);
				}
				copia(x, y, 0, 0) = pow(value[0], 1.0 / vecindarioSize);
				copia(x, y, 0, 1) = pow(value[1], 1.0 / vecindarioSize);
				copia(x, y, 0, 2) = pow(value[2], 1.0 / vecindarioSize);
			}
		}
		return copia.normalize(T(0), T(255));
	}

	/*TODO validar correcto funcionamiento*/
	template <class T>
	static inline CImg<T> filtroMediaArmonica(CImg<T>& imagen, unsigned int vecindad) {
		CImg<T> normalize(T(0), T(255)); normalize(T(0), T(255)); copia(imagen);
		//if (vecindad % 2 != 0) ++vecindad;
		vecindad /= 2;
		T uno = T(1);

		if (copia.spectrum() == 1) { //GRISES
			cimg_forXY(copia, x, y){
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				int vecindarioSize = vecindario.size();
				T acum = T(0);
				cimg_forXY(vecindario, s, t){
					acum += uno / vecindario(s, t);
				}
				copia(x, y) = vecindarioSize / acum;
			}
		}
		else if (copia.spectrum() == 3) { //RGB
			cimg_forXY(copia, x, y){
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				int vecindarioSize = vecindario.size() / 3;
				T acum[] = { T(0), T(0), T(0) };
				cimg_forXY(vecindario, s, t){
					acum[0] += uno / vecindario(s, t, 0, 0);
					acum[1] += uno / vecindario(s, t, 0, 1);
					acum[2] += uno / vecindario(s, t, 0, 2);
				}
				copia(x, y, 0, 0) = vecindarioSize / acum[0];
				copia(x, y, 0, 1) = vecindarioSize / acum[1];
				copia(x, y, 0, 2) = vecindarioSize / acum[2];
			}
		}
		return copia.normalize(T(0), T(255));
	}

	template <class T>
	static inline CImg<T> filtroMediaContraArmonica(CImg<T>& imagen, unsigned int vecindad, double ordenQ){
		CImg<T> copia(imagen);
		//if (vecindad % 2 != 0) ++vecindad;
		vecindad /= 2;

		if (copia.spectrum() == 1) { // GRISES
			cimg_forXY(copia, x, y){
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				T numeradorAcum = T(0);
				T denominadorAcum = T(0);

				cimg_forXY(vecindario, s, t) {
					double aux = pow(vecindario(s, t), ordenQ);
					denominadorAcum += T(aux); // ^Q
					numeradorAcum += T(aux*aux); // ^Q+1
				}
				copia(x, y) = numeradorAcum / denominadorAcum;
			}

		}
		else if (copia.spectrum() == 3) { // RGB
			cimg_forXY(copia, x, y){
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				T numeradorAcumR = T(0);
				T denominadorAcumR = T(0);

				T numeradorAcumG = T(0);
				T denominadorAcumG = T(0);

				T numeradorAcumB = T(0);
				T denominadorAcumB = T(0);

				cimg_forXY(vecindario, s, t) {
					double auxR = pow(vecindario(s, t, 0, 0), ordenQ);
					double auxG = pow(vecindario(s, t, 0, 1), ordenQ);
					double auxB = pow(vecindario(s, t, 0, 2), ordenQ);

					denominadorAcumR += T(auxR); // ^Q
					numeradorAcumR += T(auxR*auxR); // ^Q+1

					denominadorAcumG += T(auxG); // ^Q
					numeradorAcumG += T(auxG*auxG); // ^Q+1

					denominadorAcumB += T(auxB); // ^Q
					numeradorAcumB += T(auxB*auxB); // ^Q+1
				}
				copia(x, y, 0, 0) = numeradorAcumR / denominadorAcumR;
				copia(x, y, 0, 1) = numeradorAcumG / denominadorAcumG;
				copia(x, y, 0, 2) = numeradorAcumB / denominadorAcumB;
			}
		}
		return copia.normalize(T(0), T(255));
	}

	template <class T>
	static inline CImg<T> filtroMediaAlfaRecortado(CImg<T>& imagen, unsigned int vecindad, unsigned int d) {
		CImg<T> copia(imagen);
		d /= 2;
		vecindad /= 2;
		if (copia.spectrum() == 1) { // grises
			cimg_forXY(copia, x, y) {
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				vecindario.sort();
				T acum = T(0);
				for (unsigned i = d; i <= vecindario.size() - d; ++i) {
					acum += vecindario(i);
				}
				acum /= (vecindario.size() - 2 * d);
				copia(x, y) = acum;
			}
		}
		else  if (copia.spectrum() == 3) { // rgb
			cimg_forXY(copia, x, y) {
				CImg<T>& vecindario = imagen.get_crop(x - vecindad, y - vecindad, x + vecindad, y + vecindad, true);
				CImg<T> R[] = {
					vecindario.get_channel(0),
					vecindario.get_channel(1),
					vecindario.get_channel(2)
				};

				//vecindario.save_bmp("vecindario.bmp");

				R[0].sort();
				R[1].sort();
				R[2].sort();
				T acum[] = { T(0), T(0), T(0) };
				int vecindarioSize = vecindario.size() / 3;
				for (unsigned i = d; i < vecindarioSize - d; ++i) {
					acum[0] += R[0](i);
					acum[1] += R[1](i);
					acum[2] += R[2](i);
				}

				acum[0] /= (vecindario.size() - 2 * d);
				acum[1] /= (vecindario.size() - 2 * d);
				acum[2] /= (vecindario.size() - 2 * d);

				copia(x, y, 0, 0) = acum[0];
				copia(x, y, 0, 1) = acum[1];
				copia(x, y, 0, 2) = acum[2];
			}
		}
		return copia;
	}

	template <class T>
	static inline CImg<T> filtroFrecuenciaFilaColumna(CImg<T>& imagen, int filas[], int columnas[], int anchos[], int altos[], int nFilasColumnas) {
		// aca convendría hacer un gauss, pero no tengo tiempo, asi que va a ser filtro ideal.
		CImgList<T>& fft = imagen.get_FFT();
		CImg<T>& real = fft[0];
		CImg<T>& imag = fft[1];

		int alto = real.height(),
			ancho = real.width();

		real.shift(ancho / 2, alto / 2, 0, 0, 2);
		imag.shift(ancho / 2, alto / 2, 0, 0, 2);

		for (int i = 0; i < nFilasColumnas; ++i) {
			cimg_forX(imagen, x) {
				real(x, filas[i]) = T(0);
				imag(x, filas[i]) = T(0);
			}
			for (int j = 1; j <= anchos[i] / 2; ++j) {
				cimg_forX(imagen, x) {
					real(x, filas[i] - j) = T(0);
					real(x, filas[i] + j) = T(0);

					imag(x, filas[i] - j) = T(0);
					imag(x, filas[i] + j) = T(0);
				}
			}

			cimg_forY(imagen, y) {
				real(columnas[i], y) = T(0);
				imag(columnas[i], y) = T(0);
			}
			for (int j = 1; j <= altos[i] / 2; ++j) {
				cimg_forY(imagen, y) {
					real(columnas[i] - j, y) = T(0);
					real(columnas[i] + j, y) = T(0);

					imag(columnas[i] - j, y) = T(0);
					imag(columnas[i] + j, y) = T(0);
				}
			}
		}

		real.shift(ancho / 2, alto / 2, 0, 0, 2);
		imag.shift(ancho / 2, alto / 2, 0, 0, 2);

		CImgList<T> inversa = fft.get_FFT(true);
		return inversa[0];
	}

	template <class T>
	static inline CImg<T> agregarRuidoGaussiano(CImg<T>& imagen, double sigma) {
		return imagen.get_noise(sigma, 0);
	}

	template <class T>
	static inline CImg<T> agregarRuidoUniforme(CImg<T>& imagen, double sigma) {
		return imagen.get_noise(sigma, 1);
	}

	template <class T>
	static inline CImg<T> agregarRuidoSalYPimienta(CImg<T>& imagen, double sigma) {
		return imagen.get_noise(sigma, 2);
	}

	template<class T>
	static inline CImg<T> agregarRuidoImpulsivo(CImg<T>& imagen, double sigma) {
		return agregarRuidoSalYPimienta(imagen, sigma);
	}

	static CImg<float> new3x3SharpKernel(bool sumaUno = true, bool conDiagonales = true) {
		CImg<float> kernel(3, 3, 1, 1, 1.0f);
		float* d = kernel._data;
		if (conDiagonales&&sumaUno) {
			*(d + 0) = -1.0f; *(d + 1) = -1.0f; *(d + 2) = -1.0f;
			*(d + 3) = -1.0f; *(d + 4) = 9.0f; *(d + 5) = -1.0f;
			*(d + 6) = -1.0f; *(d + 7) = -1.0f; *(d + 8) = -1.0f;
		}
		else { //no diagonal o no sumaUno
			if (conDiagonales && !sumaUno) {
				*(d + 0) = -1.0f; *(d + 1) = -1.0f; *(d + 2) = -1.0f;
				*(d + 3) = -1.0f; *(d + 4) = 8.0f; *(d + 5) = -1.0f;
				*(d + 6) = -1.0f; *(d + 7) = -1.0f; *(d + 8) = -1.0f;
			}
			if (!conDiagonales && sumaUno) {
				*(d + 0) = -0.0f; *(d + 1) = -1.0f; *(d + 2) = -0.0f;
				*(d + 3) = -1.0f; *(d + 4) = 5.0f; *(d + 5) = -1.0f;
				*(d + 6) = -0.0f; *(d + 7) = -1.0f; *(d + 8) = -0.0f;
			}
			if (!conDiagonales && !sumaUno) {
				*(d + 0) = -0.0f; *(d + 1) = -1.0f; *(d + 2) = -0.0f;
				*(d + 3) = -1.0f; *(d + 4) = 4.0f; *(d + 5) = -1.0f;
				*(d + 6) = -0.0f; *(d + 7) = -1.0f; *(d + 8) = -0.0f;
			}
		}
		return kernel;
	}

	static CImg<float> new3x3SmothKernel(bool sumaUno = true, bool conDiagonales = true) {
		CImg<float> kernel(3, 3, 1, 1, 1.0f);
		float* d = kernel._data;
		if (conDiagonales&&sumaUno) {
			*(d + 0) = -1.0f; *(d + 1) = -1.0f; *(d + 2) = -1.0f;
			*(d + 3) = -1.0f; *(d + 4) = 9.0f; *(d + 5) = -1.0f;
			*(d + 6) = -1.0f; *(d + 7) = -1.0f; *(d + 8) = -1.0f;
		}
		else { //no diagonal o no sumaUno
			if (conDiagonales && !sumaUno) {
				*(d + 0) = -1.0f; *(d + 1) = -1.0f; *(d + 2) = -1.0f;
				*(d + 3) = -1.0f; *(d + 4) = 8.0f; *(d + 5) = -1.0f;
				*(d + 6) = -1.0f; *(d + 7) = -1.0f; *(d + 8) = -1.0f;
			}
			if (!conDiagonales && sumaUno) {
				*(d + 0) = -0.0f; *(d + 1) = -1.0f; *(d + 2) = -0.0f;
				*(d + 3) = -1.0f; *(d + 4) = 5.0f; *(d + 5) = -1.0f;
				*(d + 6) = -0.0f; *(d + 7) = -1.0f; *(d + 8) = -0.0f;
			}
			if (!conDiagonales && !sumaUno) {
				*(d + 0) = -0.0f; *(d + 1) = -1.0f; *(d + 2) = -0.0f;
				*(d + 3) = -1.0f; *(d + 4) = 4.0f; *(d + 5) = -1.0f;
				*(d + 6) = -0.0f; *(d + 7) = -1.0f; *(d + 8) = -0.0f;
			}
		}
		return kernel;
	}

	static void cargarPaleta(char* path, std::vector<std::vector<float>> &paleta) {

		std::ifstream archivoPaleta(path);
		if (archivoPaleta.is_open()) {
			paleta.clear();
			paleta.resize(256, std::vector<float>(3)); // 256 filas, 3 columnas;
			for (std::vector<float>& rgb : paleta) {
				float& r = rgb[0];
				float& g = rgb[1];
				float& b = rgb[2];
				archivoPaleta >> r;
				archivoPaleta >> g;
				archivoPaleta >> b;
			}
		}
		else {
			throw std::runtime_error("No se pudo abrir la paleta");
		}
	}

	template <class T>
	static inline void aplicarPaleta(std::vector<std::vector<float>> &paleta, CImg<T> &imagenOriginal, CImg<T> &imagenNueva) {
		imagenNueva.clear();
		int alto = imagenOriginal.height();
		int ancho = imagenOriginal.width();
		int prof = 1;
		int canales = 3;
		imagenNueva.resize(alto, ancho, prof, canales);

		cimg_forXY(imagenNueva, x, y) {
			int indice = redondea(imagenOriginal(x, y));
			std::vector<float>& fila = paleta[indice];
			imagenNueva(x, y, 0, 0) = fila[0];
			imagenNueva(x, y, 0, 1) = fila[1];
			imagenNueva(x, y, 0, 2) = fila[2];
		}
		imagenNueva.normalize(T(0), T(255));
	}

	template<class T>
	static inline void aplicarColor(CImg<T> &imagenOriginal, CImg<T> &imagenModificada, T* valorBuscado, int radio, T* valorNuevo) {
		imagenModificada.clear();
		imagenModificada.resize(imagenOriginal.width(), imagenOriginal.height(), 1, 3, -1);

		cimg_forXY(imagenOriginal, x, y) {
			T& rOriginal = imagenOriginal(x, y, 0, 0);
			if (imagenOriginal.spectrum() == 3){
				T& gOriginal = imagenOriginal(x, y, 0, 1);
				T& bOriginal = imagenOriginal(x, y, 0, 2);

				T auxR = rOriginal - valorBuscado[0];
				T auxG = gOriginal - valorBuscado[1];
				T auxB = bOriginal - valorBuscado[2];

				bool criterio = (auxR*auxR + auxG*auxG + auxB*auxB) <= radio*radio;

				if (criterio) {
					imagenModificada(x, y, 0, 0) = valorNuevo[0];
					imagenModificada(x, y, 0, 1) = valorNuevo[1];
					imagenModificada(x, y, 0, 2) = valorNuevo[2];
				}
				else {
					imagenModificada(x, y, 0, 0) = rOriginal;
					imagenModificada(x, y, 0, 1) = gOriginal;
					imagenModificada(x, y, 0, 2) = bOriginal;
				}
			}
			else {
				if (abs(rOriginal - valorBuscado[0]) <= radio){
					imagenModificada(x, y, 0, 0) = valorNuevo[0];
					imagenModificada(x, y, 0, 1) = valorNuevo[1];
					imagenModificada(x, y, 0, 2) = valorNuevo[2];
				}
				else {
					imagenModificada(x, y, 0, 0) = rOriginal;
					imagenModificada(x, y, 0, 1) = rOriginal;
					imagenModificada(x, y, 0, 2) = rOriginal;
				}
			}
		}

	}

	template <class T>
	static void inline aplicarColorHSI(CImg<T>& imagenOriginal, CImg<T>& imagenModificada, T colorBuscado[], int radio, T colorNuevo[]) {
		if (imagenOriginal.spectrum() < 3) { throw std::runtime_error("La imagen debe tener 3 canales para operar"); }
		imagenModificada = imagenOriginal; //copia
		imagenModificada.RGBtoHSI();
		int radioCuadrado = radio*radio;
		cimg_forXY(imagenModificada, x, y){
			T& h = imagenModificada(x, y, 0, 0);
			T& s = imagenModificada(x, y, 0, 1);
			T auxH = h - colorBuscado[0];
			T auxS = s - colorBuscado[1];
			bool criterio = auxH*auxH + auxS*auxS < radioCuadrado;
			if (criterio) {
				h = colorNuevo[0];
				s = colorNuevo[1];
			}
		}
		imagenModificada.HSItoRGB();
	}

	template <class T>
	static inline CImg<T> detectorBordesRoberts(CImg<T>& imagen) {
		static CImg<T> mascaraGx(3, 3, 1, 1,
			0.0, 0.0, 0.0,
			0.0, -1.0, 0.0,
			0.0, 0.0, 1.0
			);
		static CImg<T> mascaraGy(3, 3, 1, 1,
			0.0, 0.0, 0.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, 0.0
			);

		CImg<T> Gx = imagen.get_convolve(mascaraGx);
		Gx.abs();
		CImg<T> Gy = imagen.get_convolve(mascaraGy);
		Gy.abs();
		return (Gx + Gy).normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesPrewitt(CImg<T>& imagen) {
		static CImg<T> mascaraGx(3, 3, 1, 1,
			-1.0, -1.0, -1.0,
			0.0, 0.0, 0.0,
			1.0, 1.0, 1.0
			);
		static CImg<T> mascaraGy(3, 3, 1, 1,
			-1.0, 0.0, 1.0,
			-1.0, 0.0, 1.0,
			-1.0, 0.0, 1.0
			);
		CImg<T> Gx = imagen.get_convolve(mascaraGx);
		Gx.abs();
		CImg<T> Gy = imagen.get_convolve(mascaraGy);
		Gy.abs();
		return (Gx + Gy).normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesPrewittDiagonal(CImg<T>& imagen) {
		static CImg<T> mascaraGx(3, 3, 1, 1,
			0.0, 1.0, 1.0,
			-1.0, 0.0, 1.0,
			-1.0, -1.0, 0.0
			);
		static CImg<T> mascaraGy(3, 3, 1, 1,
			-1.0, -1.0, 0.0,
			-1.0, 0.0, 1.0,
			0.0, 1.0, 1.0
			);

		CImg<T> Gx = imagen.get_convolve(mascaraGx);
		Gx.abs();
		CImg<T> Gy = imagen.get_convolve(mascaraGy);
		Gy.abs();
		return (Gx + Gy).normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesSobel(CImg<T>& imagen) {
		static CImg<T> mascaraGx(3, 3, 1, 1,
			-1.0, -2.0, -1.0,
			0.0, 0.0, 0.0,
			1.0, 2.0, 1.0
			);
		static CImg<T> mascaraGy(3, 3, 1, 1,
			-1.0, 0.0, 1.0,
			-2.0, 0.0, 2.0,
			-1.0, 0.0, 1.0
			);
		CImg<T> Gx = imagen.get_convolve(mascaraGx);
		Gx.abs();
		CImg<T> Gy = imagen.get_convolve(mascaraGy);
		Gy.abs();
		return (Gx + Gy).normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesSobelDiagonales(CImg<T>& imagen) {
		static CImg<T> mascaraGx(3, 3, 1, 1,
			0.0, 1.0, 2.0,
			-1.0, 0.0, 1.0,
			-2.0, -1.0, 0.0
			);
		static CImg<T> mascaraGy(3, 3, 1, 1,
			-2.0, -1.0, 0.0,
			-1.0, 0.0, 1.0,
			0.0, 1.0, 2.0
			);

		CImg<T> Gx = imagen.get_convolve(mascaraGx);
		Gx.abs();
		CImg<T> Gy = imagen.get_convolve(mascaraGy);
		Gy.abs();
		return (Gx + Gy).normalize(0, 255);
	}

	template<class T>
	static inline CImg<T> detectorBordesRoberts(CImg<T>& imagen, T umbral) {
		CImg<T>& bordes = detectorBordesRoberts(imagen);
		bordes.threshold(umbral);
		return bordes.normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesPrewitt(CImg<T>& imagen, T umbral) {
		CImg<T> &bordes = detectorBordesPrewitt(imagen);
		bordes.threshold(umbral);
		return bordes.normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesPrewittDiagonal(CImg<T>& imagen, T umbral) {
		CImg<T> &bordes = detectorBordesPrewittDiagonal(imagen);
		bordes.threshold(umbral);
		return bordes.normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesSobel(CImg<T>& imagen, T umbral) {
		CImg<T> &bordes = detectorBordesSobel(imagen);
		bordes.threshold(umbral);
		return bordes.normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesSobelDiagonales(CImg<T>& imagen, T umbral) {
		CImg<T> &bordes = detectorBordesSobelDiagonales(imagen);
		bordes.threshold(umbral);
		return bordes.normalize(0, 255);
	}

	template <class T>
	static inline CImg<T> detectorBordesLaplacianoN4(CImg<T>& imagen, T umbral = T(-1)) {
		CImg<T>& mascara = new3x3SharpKernel(false, false);
		CImg<T>& bordes = imagen.get_convolve(mascara);
		if (umbral != T(-1)){
			bordes.threshold(umbral);
		}
		return bordes;
	}

	template <class T>
	static inline CImg<T> detectorBordesLaplacianoN8(CImg<T>& imagen, T umbral = T(-1)) {
		CImg<T>& mascara = new3x3SharpKernel(false, true);
		CImg<T>& bordes = imagen.get_convolve(mascara);
		if (umbral != T(-1)){
			bordes.threshold(umbral);
		}
		return bordes;
	}

	template<class T>
	static inline CImg<T> detectorBordesLoG(CImg<T>& imagen, T desvio) {
		throw std::runtime_error("Not yet implemented");
		return imagen;
	}

	///****************************************
	/// Crecimiento de regiones
	///****************************************
	static inline CImg<unsigned char> regionGrowing(CImg<unsigned char> &orig, int x, int y, int delta, int etiqueta){
		// orig: imagen a procesar
		// x,y: posición de la semilla
		// delta: define el rango de pertenencia como [semilla-delta/2, semilla+delta/2]
		// etiqueta: nro de la etiqueta, no debe pertenecer al rango

		//Basicamente pinto la semilla y la pongo en la cola
		//despues tomo el primer nodo de la cola como referencia (a su vez lo elimino de la cola),
		// miro sus vecinos, los pinto y los agrego al final de la cola

		struct node{
			int x;
			int y;
		}node;
		// armo el rango
		int valor = orig(x, y);
		int rango_min = valor - delta / 2,
			rango_max = valor + delta / 2 + delta % 2; //para delta impar suma 1 para delta par suma 0
		if (rango_min < 0) rango_min = 0;
		if (rango_max > 255) rango_max = 255;

		//muestro en pantalla y controlo etiqueta
		//cout << "semilla: " << valor << " rango: [" << rango_min << "," << rango_max << "] etiqueta: " << etiqueta << endl;
		//if (etiqueta >= rango_min && etiqueta <= rango_max){
		//	cout << "error: la etiqueta (" << etiqueta << ") no puede pertenecer al rango, vuelva a elegir" << endl;
		//	cout << "etiqueta: ";
		//	cin >> etiqueta;
		//}
		CImg<> dest(orig);
		std::vector<int> camino;

		dest(x, y) = etiqueta;
		//agrego el nodo (x,y) al final de la cola sólo por una cuestion de inicializacion
		//para que el erase del while tenga sentido en la primer ejecucion
		camino.push_back(x);
		camino.push_back(y);
		//mientras exista cola
		while (camino.size() != 0){
			//     cout<<camino.size()<<" ";
			//tomo los dos primeros valores de la cola
			node.x = camino[0];
			node.y = camino[1];

			//elimino el nodo de la cola xq ya lo tengo pintado
			camino.erase(camino.begin(), camino.begin() + 2);

			//miro a la derecha
			if ((node.x + 1 > 0) && (node.x + 1 < dest.width())) {
				if ((node.y > 0) && (node.y < dest.height())) {
					if (dest(node.x + 1, node.y) >= rango_min && dest(node.x + 1, node.y) <= rango_max){
						//si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
						dest(node.x + 1, node.y) = etiqueta;
						camino.push_back(node.x + 1);
						camino.push_back(node.y);
					}
				}
			}
			//miro a la izquierda
			if ((node.x - 1 > 0) && (node.x - 1 < dest.width())){
				if ((node.y > 0) && (node.y < dest.height())) {
					if (dest(node.x - 1, node.y) >= rango_min && dest(node.x - 1, node.y) <= rango_max){
						//si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
						dest(node.x - 1, node.y) = etiqueta;
						camino.push_back(node.x - 1);
						camino.push_back(node.y);
					}
				}
			}
			//miro abajo
			if ((node.x > 0) && (node.x < dest.width())) {
				if ((node.y + 1 > 0) && (node.y + 1 < dest.height())) {
					if (dest(node.x, node.y + 1) >= rango_min && dest(node.x, node.y + 1) <= rango_max){
						//si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
						dest(node.x, node.y + 1) = etiqueta;
						camino.push_back(node.x);
						camino.push_back(node.y + 1);
					}
				}
			}
			//miro arriba
			if ((node.x > 0) && (node.x < dest.width())) {
				if ((node.y - 1 > 0) && (node.y - 1 < dest.height())) {
					if (dest(node.x, node.y - 1) >= rango_min && dest(node.x, node.y - 1) <= rango_max){
						//si esta en el rango lo pinto y agrego el nodo nuevo al final de la cola
						dest(node.x, node.y - 1) = etiqueta;
						camino.push_back(node.x);
						camino.push_back(node.y - 1);
					}
				}
			}
		}
		return dest;
	}


	///****************************************
	/// Etiquetado de componentes conectadas
	///****************************************
	static inline CImg<int> label_cc(CImg<int> img, int blanco = 1, int nueva_etiqueta = 2){

		std::vector<int> equiv(nueva_etiqueta + 1, 0); //vector de equivalencias
		std::vector<int> vecinos;                   //vector de etiquetas vecinos superiores e izquierda
		int pos, etiqueta, aux;

		cimg_forXY(img, x, y){           // recorro la imagen
			if (img(x, y) == blanco){       // si es blanco
				vecinos.clear();           // inicializo 
				if (x && y)                // si x no es borde izq e y no es borde superior miro el vecino sup izq
					if (img(x - 1, y - 1) != 0)  vecinos.push_back(img(x - 1, y - 1)); // si tiene etiqueta la guardo
				if (y)                     // si y no es borde superior miro vecino superior
					if (img(x, y - 1) != 0)    vecinos.push_back(img(x, y - 1));   // si tiene etiqueta la guardo
				if (y && x != img.width() - 1)  // si x no es borde derecho e y borde superior miro vecino sup der
					if (img(x + 1, y - 1) != 0)  vecinos.push_back(img(x + 1, y - 1)); // si tiene etiqueta la guardo
				if (x)                     // si x no es borde izquierdo miro vecino izq
					if (img(x - 1, y) != 0)    vecinos.push_back(img(x - 1, y));   // si tiene etiqueta la guardo
				if (vecinos.empty()) {     // si no tengo vecinos etiquetados debo generar nueva etiqueta
					vecinos.push_back(nueva_etiqueta); // de vecinos voy a sacar la etiqueta que corresponde al pixel
					equiv[nueva_etiqueta] = nueva_etiqueta; // guardo en la posicion nva etiqueta el valor nva etiqueta
					nueva_etiqueta++;        // genero una nueva etiqueta para cdo necesite
					equiv.push_back(0);      // agrego una posicion en equivalencias con 0
				}
				else {
					for (int i = 0; i < vecinos.size() - 1; i++)  // controlo la lista de etiquetas vecinas
						if (vecinos[i] != vecinos[i + 1]){ // si hay diferentes etiquetas en el vecindario anoto
							etiqueta = vecinos[i];
							pos = vecinos[i + 1];
							if (pos < etiqueta){ // en la pos de la mayor etiqueta anoto el valor de la menor 
								aux = etiqueta;
								etiqueta = pos;
								pos = aux;
							}
							if (equiv[pos] != etiqueta){ // si tengo una entrada en esa pos reviso la cadena
								if (equiv[pos] != pos){
									aux = etiqueta;
									etiqueta = equiv[pos];
									pos = aux;
									while (equiv[pos] != pos)
										pos = equiv[pos];
									if (equiv[pos] < etiqueta)
										etiqueta = equiv[pos];
								}
								equiv[pos] = etiqueta;
							}
						}
				}
				img(x, y) = vecinos.front(); // asigno etiqueta
			}
		}
		img.display("Primera Pasada");

		// Muestro como quedo la tabla
		//cout << "Tabla de equivalencias" << endl << endl;
		//for (int j = 0; j<equiv.size(); j++)
		//	cout << j << " " << equiv[j] << endl;
		//cout << endl;

		// reasigno la etiqueta
		cimg_forXY(img, x, y)
			if (img(x, y) != 0)
				if (equiv[img(x, y)] != img(x, y)){
					etiqueta = equiv[img(x, y)];
					while (equiv[etiqueta] != etiqueta)
						etiqueta = equiv[etiqueta];
					img(x, y) = etiqueta;
				}

		return img;
	}
	template <class T>
	static inline double calcularAnguloHoughInverso(Pixel<T>& p, CImg<T>& img) {
		const unsigned& M = img.width(), &N = img.height();
		int t = p.x;

		double
			step_theta = M_PI / (M - 1),
			angulo = t*step_theta - M_PI / 2;

		return angulo;
	}

	///****************************************
	/// Transformada Hough directa e inversa
	///****************************************
	static inline CImg<double> transformadaHough(CImg<double>& img, bool inverse = false) {

		CImg<double> iHough(img); iHough.fill(0.0);
		const unsigned M = img.width(),
			N = img.height();

		double max_rho = sqrt(float(pow(N - 1, 2) + pow(M - 1, 2))), //maximo valor posible de radio se da en la diagonal pcipal
			step_rho = 2.*max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
			step_theta = M_PI / (M - 1),     //paso en eje theta (M_PI=pi) (theta=[-90,90])
			rho, theta;

		if (!inverse){
			int r;  // radio mapeado en los N pixeles
			cimg_forXY(img, y, x){
				if (img(y, x) > 0.5)
					for (int t = 0; t < M; t++) { //calculo rho variando theta en todo el eje, con x e y fijo
						theta = t*step_theta - M_PI / 2;  // mapea t en [0,M-1] a t en [-90,90]
						rho = x*cos(theta) + y*sin(theta); // calcula rho para cada theta
						r = floor((rho + max_rho) / step_rho + .5); // mapea r en [-max_rho , max_rho] a r en [0,N-1] el floor(r+.5) sirve para redondear
						iHough(t, r) += 1;               // suma el acumulador     
					}
			}
		}
		else{
			const double blanco[1] = { 255.f };
			float x0, x1, y0, y1;
			cimg_forXY(img, t, r){
				if (img(t, r) > 0.5) {
					theta = t*step_theta - M_PI / 2;   // mapea t en [0,M-1] a t en [-90,90]
					rho = r*step_rho - max_rho;      // mapea r en [0,N-1] a r en [-max_rho,max_rho]
					if (theta > -M_PI / 2 && theta < M_PI / 2){
						y0 = 0; y1 = M - 1;
						x0 = rho / cos(theta);      // calcula y para y=0
						x1 = rho / cos(theta) - (M - 1)*tan(theta); // calcula y para y=M-1	  
					}
					else{
						x0 = 0; x1 = N - 1;
						y0 = rho / sin(theta);      // calcula y para x=0
						y1 = rho / sin(theta) - (N - 1) / tan(theta); // calcula y para x=M-1
					}
					//cout<<endl<<"("<<t<<","<<r<<")->("<<theta<<","<<rho<<") "<<"("<<y0<<","<<x0<<")->("<<y1<<","<<x1<<")"<<endl;
					iHough.draw_line(y0, x0, y1, x1, blanco); // dibuja una línea de (0,y0) a (M-1,y1)
				}
			}
		}
		return iHough;
	}

	static inline CImg<double> recortarTransformadaHough(CImg<double>& hough, double min_rho, double max_rho, double min_theta, double max_theta) {
		const unsigned M = hough.width(),
			N = hough.height();

		double max_rho2 = sqrt((N - 1)*(N - 1) + (M - 1)* (M - 1)), //maximo valor posible de radio se da en la diagonal pcipal
			step_rho = 2.*max_rho2 / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
			step_theta = M_PI / (M - 1),     //paso en eje theta (M_PI=pi) (theta=[-90,90])
			rho, theta;

		cimg_forXY(hough, t, r) {
			theta = t*step_theta - M_PI / 2;   // mapea t en [0,M-1] a t en [-90,90]
			rho = r*step_rho - max_rho2;      // mapea r en [0,N-1] a r en [-max_rho,max_rho]
			if (theta<min_theta || theta > max_theta || rho < min_rho || rho > max_rho) {

				hough(t, r) = 0;               // vuelo a la miercole el valor

			}
		}
		return hough;
	}
	static inline CImg<double> recortarTransformadaHoughPorAngulo(CImg<double>& hough, double min_theta, double max_theta) {
		const unsigned M = hough.width(),
			N = hough.height();

		double max_rho = sqrt((N - 1)*(N - 1) + (M - 1)* (M - 1)), //maximo valor posible de radio se da en la diagonal pcipal
			step_rho = 2.*max_rho / (N - 1), //paso en eje rho (rho=[-max_rho , max_rho])
			step_theta = M_PI / (M - 1),     //paso en eje theta (M_PI=pi) (theta=[-90,90])
			rho, theta;

		cimg_forXY(hough, t, r) {
			theta = t*step_theta - M_PI / 2;   // mapea t en [0,M-1] a t en [-90,90]
			rho = r*step_rho - max_rho;      // mapea r en [0,N-1] a r en [-max_rho,max_rho]
			if (theta<min_theta || theta > max_theta) {

				hough(t, r) = 0;               // vuelo a la miercole el valor

			}
		}
		return hough;
	}


	template <typename T>
	static inline bool evaluarCercaniaHough(Pixel<T>& p0,
		typename std::vector< Pixel<T> >::iterator& begin,
		typename std::vector< Pixel<T> >::iterator& end,
		int r, int t)
	{
		std::vector<Pixel<T>>::iterator it(begin);
		while (it != end) {
			Pixel<T>& val = *it++;
			if (abs(p0.y - val.y) < r && abs(p0.x - val.x) < t){
				return false;
			}
		}
		return true;
	}

	template <class T>
	static std::vector<Pixel<T> > obtenerLosMaximos(CImg<T>& imagen, unsigned int cantidad, bool descartarCercanos = false, int r = 10, int t = 10) {
		std::vector<Pixel<T> > copia1D(imagen.size() / imagen.spectrum());
		{
			int i = 0;
			cimg_forXY(imagen, x, y) {
				auto& v = copia1D[i++];
				v.x = x;
				v.y = y;
				v.value = imagen(x, y, 0, 0);
			}
		}
		std::sort(copia1D.begin(), copia1D.end());
		std::vector<Pixel<T> > result(cantidad);
		result[0] = copia1D[copia1D.size() - 1]; // el maximo de todos.
		if (descartarCercanos) {
			for (int i = 1, j = 1; i < cantidad && j < copia1D.size(); ++j){
				auto& punto = copia1D[copia1D.size() - 1 - j];
				//if (abs(punto.x - result[i - 1].x) + abs(punto.y - result[i - 1].y) > 4) {
				if (evaluarCercaniaHough(punto, result.begin(), result.begin() + i, r, t)) {
					result[i++] = punto;
				}
			}
		}
		else {
			for (int i = 0; i < cantidad; ++i) {
				result[i] = copia1D[copia1D.size() - 1 - i];
			}
		}
		return result;
	}

	template <typename T>
	static inline CImg<T> obtenerLineasPrincipales(CImg<T>& imagen, int cantidadLineas, T umbral = T(128), int toleranciaAngulo = 10, int toleranciaDistancia = 10) {
		CImg<T>& bordes = detectorBordesSobelDiagonales(imagen, umbral);
		CImg<T> hough = transformadaHough(bordes);
		std::vector<Pixel<T>>& maximos = obtenerLosMaximos(bordes, cantidadLineas, true, toleranciaAngulo, toleranciaDistancia);
		hough.fill(0);
		for (Pixel<T>& pixel : maximos) {
			hough(pixel.x, pixel.y) = pixel.value;
		}
		CImg<T> ret = transformadaHough(hough, true);
		return ret;
	}

	template <class T>
	static inline CImg<T> morfologiaApertura(CImg<T>& A, CImg<T>& B){
		CImg<T>& AP = A.get_erode(B);
		AP.dilate(B);
		return AP;
	}

	template <class T>
	static inline CImg<T> morfologiaCierre(CImg<T>& A, CImg<T>& B) {
		CImg<T>& AP = A.get_dilate(B);
		AP.erode(B);
		return  AP;
	}

	static inline CImg<bool> nuevoElementoEstructuranteCruz3x3() {
		static CImg<bool> B(3, 3, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0);
		return B;
	}

	static inline CImg<bool> nuevoElementoEstructuranteCruz15x15() {
		static CImg<bool> B(15, 15, 1, 1,
			0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
			);
		return B;
	}

	static CImg<bool> complemento(CImg<bool> imagen){
		cimg_forXY(imagen, x, y){
			imagen(x, y) = !imagen(x, y);
		}
		return imagen;
	}

	template<class T>
	static inline CImg<T> RGBtoGray(CImg<T>& image) {
		CImg<T> ret(image.width(), image.height(), 1, 1, 0);
		cimg_forXY(image, x, y) {
			T& R = image(x, y, 0, 0);
			T& G = image(x, y, 0, 1);
			T& B = image(x, y, 0, 2);
			T grayValueWeight = (R*0.299) + (G*0.587) + (B*0.114);
			ret(x, y) = grayValueWeight;
		}
		return ret;
	}

	template<class T>
	static inline void mostrarHistograma(CImg<T>& imagen) {
		imagen.get_histogram(256, 0, 255).display_graph();
	}
};