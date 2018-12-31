#ifndef CXC_DEBUGLOGGER_H
#define CXC_DEBUGLOGGER_H

#include <string>
#include <fstream>
#include "Singleton.inl"

#define DEBUG_LOG(Type, LogMsg) cxc::DebugLogger::GetInstance()->DebugLog(Type, LogMsg)

namespace cxc
{

	enum class eLogType : uint16_t
	{
		Error = 0,
		Warning = 1,
		Verbose = 2
	};

	/* The DebugLogger is the class that helps to output debug message */
	class DebugLogger : public Singleton<DebugLogger>
	{
	public:
		friend class Singleton<DebugLogger>;

		DebugLogger();
		~DebugLogger();

	public:

		bool IsEnableWriteLogFile() const { return bEnableWriteLogFile; }
		void SetWriteLogFile(bool Enable) { bEnableWriteLogFile = Enable; }
		void SetVerbosity(bool bIsVerbose) { Verbosity = bIsVerbose; }
		bool GetVerbosity() const { return Verbosity; }

		void SetLogFileDirectory(const std::string& FilePath) { DebugLogFileDirectory = FilePath; }
		std::string GetLogFileDirectory() const { return DebugLogFileDirectory; }

	public:

		void DebugLog(eLogType Type, const std::string& LogMsg);

	private:

		std::string GetSystemDate() const;
		std::string GetSystemTime() const;
		void WriteLogFile(const std::string LogMsg);

	private:

		// Whether to output verbose log
		bool Verbosity;

		// Whether to write the log to the log file
		bool bEnableWriteLogFile;

		// Log file path
		std::string DebugLogFileDirectory;

		// LogFile handlder
		std::ofstream LogFile;
	};
}

#endif // CXC_DEBUGLOGGER_H