/*
 * Log.h
 *
 *  Created on: 31 Jan 2013
 *      Author: hukka
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>

class Log {
public:

	static void Write(const std::string& log);
	static void WriteBytes(const std::string& log);

	static void Clear();
	static void SetLogFile(const std::string& logfile);

private:

	static std::string mLogfile;

};

#endif /* LOG_H_ */