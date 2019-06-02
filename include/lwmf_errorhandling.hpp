/*
********************************************************
*                                                      *
* lwmf_errorhandling - lightweight media framework     *
*                                                      *
* (C) 2019 - present by Stefan Kubsch                  *
*                                                      *
********************************************************
*/

#pragma once

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>

namespace lwmf
{

	void LogErrorAndThrowException(const std::string& ErrorMessage);

	//
	// Functions
	//

	inline void LogErrorAndThrowException(const std::string& ErrorMessage)
	{
		std::ofstream ErrorlogFile;
		ErrorlogFile.open("Errorlog.txt");

		ErrorlogFile << ErrorMessage << std::endl;
		ErrorlogFile.close();

		throw std::runtime_error(ErrorMessage);
	}


} // namespace lwmf