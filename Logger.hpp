#pragma once

#include "harCs.hpp"

namespace Logging
{
	class FileLogger
	{

	public:

		FileLogger(std::string fileNameIn, std::string fileNameOut);
		~FileLogger() { /*fileIn.close();*/ fileOut.close(); }

		void readFile(std::string &out) = delete;
		void write(const char *fmt, ...);

	private:

		std::string fileNameIn;
		std::string fileNameOut;

		std::ifstream fileIn;
		std::ofstream fileOut;
	};
}