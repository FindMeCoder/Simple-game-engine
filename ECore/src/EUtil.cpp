
#include "EUtil.h"

namespace E3D
{

	

	std::ofstream *GLogStream = NULL;

	bool InitLog(const EString& fileName)
	{
		if (GLogStream == NULL)
		{
			GLogStream = new std::ofstream();
			GLogStream->open(fileName.c_str());
			if (GLogStream->good())
				return true;

			return false;
		}

		return true;
	}

	

	void Log(const EChar *string, ...)
	{
		if (GLogStream == NULL)
			return;

		EChar buffer[256];

		// make sure both the error file and string are valid
		if (!string || !GLogStream)
			return;

		va_list arglist; // variable argument list
		// print out the string using the variable number of arguments on stack
		va_start(arglist,string);
		vsprintf(buffer,string,arglist);
		va_end(arglist);

		EString info(buffer);
		*GLogStream << info << std::endl;
		GLogStream->flush();
	}

	

	void CloseLog()
	{
		GLogStream->close();
		SafeDelete(GLogStream);
	}
}