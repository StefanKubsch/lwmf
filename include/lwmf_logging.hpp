/*
**************************************************
*                                                *
* lwmf_logging - lightweight media framework     *
*                                                *
* (C) 2019 - present by Stefan Kubsch            *
*                                                *
**************************************************
*/

#pragma once

#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

namespace lwmf
{


	class Logging final
	{
	public:
		enum class LogLevels : std::int_fast32_t
		{
			Info,
			Debug,
			Warn,
			Error,
			Critical
		};

		Logging(const std::string& Logfilename);
		~Logging();
		void AddEntry(LogLevels Level, const std::string& Message);

	private:
		std::string GetTimeStamp();

		std::ofstream Logfile;
	};

	Logging::Logging(const std::string& Logfilename)
	{
		Logfile.open(Logfilename);

		if (Logfile.fail())
		{
			throw std::runtime_error("lwmf_logging: Error creating logfile!");
		}

		Logfile << "lwmf logging / (c) Stefan Kubsch\n";
		Logfile << "logging started at: " << GetTimeStamp();
		Logfile << "--------------------------------------------------------------------------------------------------------------" << std::endl;
	}

	Logging::~Logging()
	{
		if (Logfile.is_open())
		{
			Logfile << "--------------------------------------------------------------------------------------------------------------\n";
			Logfile << "logging ended at: " << GetTimeStamp() << std::endl;
		}
	}

	inline void Logging::AddEntry(const LogLevels Level, const std::string& Message)
	{
		std::string LogLevelString;
		bool IsError{};

		switch (Level)
		{
			case LogLevels::Info:
			{
				LogLevelString = "** Info: ";
				break;
			}
			case LogLevels::Debug:
			{
				LogLevelString = "** Debug: ";
				break;
			}
			case LogLevels::Warn:
			{
				LogLevelString = "** Warning: ";
				break;
			}
			case LogLevels::Error:
			{
				LogLevelString = "** Error: ";
				IsError = true;
				break;
			}
			case LogLevels::Critical:
			{
				LogLevelString = "** Critical error: ";
				IsError = true;
				break;
			}
			default: {}
		}

		if (Logfile.is_open())
		{
			if (!IsError)
			{
				Logfile << LogLevelString << Message << std::endl;
			}
			else
			{
				Logfile << "\n" << GetTimeStamp() << LogLevelString << Message << std::endl;
				Logfile.close();

				throw std::runtime_error(Message);
			}
		}
	}

	inline std::string Logging::GetTimeStamp()
	{
		const auto CurrentTime{ std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };
		char TimeString[26];
		ctime_s(TimeString, sizeof(TimeString), &CurrentTime);

		return std::string(TimeString);
	}


} // namespace lwmf