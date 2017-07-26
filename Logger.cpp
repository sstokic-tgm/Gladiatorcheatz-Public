#include "Logger.hpp"

namespace Logging
{
	FileLogger::FileLogger(std::string fileNameIn, std::string fileNameOut) : fileNameIn{ fileNameIn }, fileNameOut{ fileNameOut }
	{
		//fileIn.open(fileNameIn, std::ifstream::in);
		fileOut.open(fileNameOut, std::ofstream::out | std::ofstream::trunc);
	}

	void FileLogger::write(const char *fmt, ...)
	{
		if (fileOut.is_open())
		{
			if (fmt == nullptr)
				return;

			char buf[1024];

			va_list list;
			va_start(list, fmt);
			_vsnprintf(buf, sizeof(buf) - 1, fmt, list);
			va_end(list);
			buf[sizeof(buf) - 1] = 0;

			fileOut << buf << std::endl;
		}
	}
}