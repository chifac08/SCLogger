/*
 * LogTypes.h
 *
 *  Created on: Apr 16, 2019
 *      Author: chifac08
 */

#ifndef INCL_LOGTYPES_H_
#define INCL_LOGTYPES_H_

typedef enum
{
	FATAL=0,
    ERROR=1,
    WARN=2,
    INFO=3,
    DEBUG=4,
	TRACE=5,
	ALL=6,
	OFF=7
} LOG_LEVEL;

typedef struct
{
	LOG_LEVEL logLevel;
	char logFile[4048];
	unsigned int logBuffer;
} LOG_CONFIG;


#endif /* INCL_LOGTYPES_H_ */
