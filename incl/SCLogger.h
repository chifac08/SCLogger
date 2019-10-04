/*
 * SCLogger.h
 *
 *  Created on: Apr 16, 2019
 *      Author: chifac08
 */

#ifndef INCL_SCLOGGER_H_
#define INCL_SCLOGGER_H_
#include "LogTypes.h"

extern int initLogging(LOG_CONFIG config);
extern void logIt(LOG_LEVEL logLevel, char* cpMessage);
extern char* getCurrentLogLevel();
extern void destroyLogging();
extern LOG_LEVEL parseLogLevel(char* cpLogLevel);
extern void formatLog(char* cpReturn, int iFormatLength, const char* cpLogMessage, ...);

#endif /* INCL_SCLOGGER_H_ */
