#pragma once

#include<string>
#include<sstream>
#include<fstream>
#include<ctime>

/*clase singleton utilizada para logear mensajes*/
class TjpLogger{
private:
	TjpLogger() : logCache(""), converter(), fileStream("log.log", std::ios::app) {
		logCache.reserve(4096);
	}
	TjpLogger(TjpLogger &log){} // para mantener la compatibilidad con c++ prev 11
	//void operator=(TjpLogger &log){} //idem anterior

	/*Retorna un string con la fecha actual yyyy/MM/dd HH:MM:SS*/
	std::string getCurrentDate(){
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "[%Y/%m/%d %H:%M:%S]", timeinfo);
		return std::string(buffer);
	}

	std::string logCache;
	std::stringstream converter;
	std::ofstream fileStream;
public:
	static TjpLogger &getInstance(){
		static TjpLogger instance;
		return instance;
	}

	~TjpLogger(){
		fileStream.close();
	}

	template <class T> void log(std::string &callFrom, T &mensaje){
		std::string &currentDate = getCurrentDate();
		fileStream << currentDate << " " << callFrom << " " << mensaje<<std::endl;
		fileStream.flush(); //para que escriba enseguida
	}

	template <class T> void log(char* callFrom, T &mensaje){
		log(std::string(callFrom), mensaje);
	}

};