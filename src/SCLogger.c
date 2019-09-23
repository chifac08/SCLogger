/*
 * SCLogger.c
 *
 * Simple Logging Library for C/C++
 *
 * Supported Logging Level:
 *  # FATAL
 *  # ERROR
 *  # WARN
 *  # INFO
 *  # DEBUG
 *  # TRACE
 *  # ALL
 *  # OFF
 *
 *  Created on: Apr 16, 2019
 *      Author: chifac08
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "SCLogger.h"

//defines
#define DATE_SIZE 26
#define LOG_MESSAGE_SIZE 4048
#define DEFAULT_LOG_FILE "/var/log/sclogger/log.txt"

//vars
typedef struct node
{
    char time[DATE_SIZE];
    char message[LOG_MESSAGE_SIZE];
    struct node* next;
} logNode;

static LOG_CONFIG logConfig;
static int iListSize = 0;
static logNode* logList = NULL;
static logNode* lastNode = NULL;

static const char* logLevelString[] = {
		"FATAL",
	    "ERROR",
	    "WARN",
	    "INFO",
	    "DEBUG",
		"TRACE",
		"ALL",
		"OFF"
};

/**
 * @brief return log level as string
 * @author chifac08
 */
static const char* getLogLevel(LOG_LEVEL logLevel)
{
	return logLevelString[logLevel];
}

/*
 * @brief create a new logger node
 * @param cpTime ... log time
 * @param cpMessage ... log message
 * @author chifac08
 */
static logNode* createNode(char* cpTime, char* cpMessage)
{
    logNode* newNode = (logNode*) malloc(sizeof(logNode));
    memset(newNode->message, 0, sizeof(newNode->message));
    memset(newNode->time, 0, sizeof(newNode->time));
    snprintf(newNode->time, sizeof(newNode->time)-1, cpTime);
    snprintf(newNode->message, sizeof(newNode->message)-1, cpMessage);

    return newNode;
}

/**
 * @brief store a log message
 * @param cpTime ... log time
 * @param cpMessage ... log message
 */
static void pushLog(char* cpTime, char* cpMessage)
{
	logNode* newNode = createNode(cpTime, cpMessage);
    newNode->next = logList;
    logList = newNode;

    //increment list size counter
    iListSize++;
}

/**
 * @brief append log node to log list
 * @param cpTime ... log time
 * @param cpMessage ... log message
 * @author chifac08
 */
static void appendLog(char* cpTime, char* cpMessage)
{
	logNode* newNode = createNode(cpTime, cpMessage);

	if(logList == NULL)
	{
		logList = newNode;
		lastNode = logList;
	}

	//link new node to the end
	lastNode->next=newNode;

	//link new node to null
	newNode->next=NULL;

	//point to last node
	lastNode=newNode;

	//increment list size counter
	iListSize++;
}

/**
 * @brief remove specific log from linked list
 * @param index ... index for log node
 * @author chifac08
 */
static void removeLog(int iIndex)
{
	//store head node
	logNode* temp = logList;

    // If head needs to be removed
    if (iIndex == 0)
    {
        logList = temp->next;   // Change head
        free(temp);               // free old head
        return;
    }

    for(; iIndex > 1; iIndex--)
    	temp = temp->next;

    logNode* next = temp->next;
    temp->next = next->next;
    free(next);

    //decrement list size counter
    iListSize--;
}

/**
 * @brief print log list
 * @author chifac08
 */
static void printList()
{
    while(logList != NULL)
    {
        printf("%s %s\n", logList->time, logList->message);
        logList=logList->next;
    }
}

/**
 * @brief flush the whole linked list
 * @author chifac08
 */
static void flushList()
{
	logNode* next = NULL;

	while(logList != NULL)
	{
		next = logList->next;
		free(logList);
		logList=next;
	}
	iListSize=0;
	logList = NULL;
}

/**
 * @brief writes to log file and stdout (when debugging mode is active)
 * @author chifac08
 */
static void writeLog()
{
	FILE* logFile = NULL;
	int iFullMsgLength = DATE_SIZE+4+5+LOG_MESSAGE_SIZE+1;
	char* cpFullMessage = NULL;

	logFile = fopen(logConfig.logFile, "a+");

	if(!logFile)
	{
		printf("Could not open logfile: %s", logConfig.logFile);
		return;
	}

	if(!cpFullMessage)
		cpFullMessage = (char*)malloc(iFullMsgLength);

	while(logList != NULL)
	{
		memset(cpFullMessage, 0, iFullMsgLength);
		snprintf(cpFullMessage, iFullMsgLength-1, "%s %s\n", logList->time, logList->message);
		printf("%s", cpFullMessage);
		fprintf(logFile, cpFullMessage);
		logList = logList->next;
	}

	//flush current list
	flushList();

	if(cpFullMessage)
		free(cpFullMessage);

	fclose(logFile);
}

/**
 * @brief initialize logging
 * @param logLevel
 * @param cpLogFile ... absolute path for log file
 * @author chifac08
 * @return 0 ... ok
 *         1 ... Could not create log file
 */
int initLogging(LOG_CONFIG config)
{
	FILE* logFile = NULL;
	int iRet = 0;

	memset(&logConfig, 0, sizeof(LOG_CONFIG));
	memcpy(&logConfig, &config, sizeof(LOG_CONFIG));

	if(logConfig.logFile[0] == '\0')
		snprintf(logConfig.logFile, sizeof(logConfig.logFile)-1, DEFAULT_LOG_FILE);

	if(logConfig.logLevel == 0)
		logConfig.logLevel = ERROR;

	if(logConfig.logBuffer == 0)
		logConfig.logBuffer = 1;

	logFile = fopen(logConfig.logFile, "rb");

	if(!logFile)
	{
		logFile = fopen(logConfig.logFile, "wb");
		if(!logFile)
			iRet = 1;
	}

	return iRet;
}

/**
 * @brief log message to log file
 * @param logLevel  ... wanted log level (enum)
 * @param cpMessage ... formatted message
 * @author chifac08
 */
void logIt(LOG_LEVEL logLevel, char* cpMessage)
{
    char szLogMessage[LOG_MESSAGE_SIZE] = {0};
    char szTime[DATE_SIZE] = {0};
    time_t now;
    struct tm* timeinfo;

    if(logLevel <= logConfig.logLevel)
    {
        time(&now);
        timeinfo = localtime(&now);
        strftime(szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", timeinfo);
        snprintf(szLogMessage, sizeof(szLogMessage)-1, "[%s]: %s", getLogLevel(logLevel), cpMessage);
        appendLog(szTime, szLogMessage);

    	if(iListSize >= logConfig.logBuffer)
        	writeLog();
    }
}

/**
 * @brief return current set log level
 * @author chifac08
 */
char* getCurrentLogLevel()
{
	return getLogLevel(logConfig.logLevel);
}

/*
 * @brief Free memory
 * @author chifac08
 */
void destroyLogging()
{
	flushList();
}

/**
 * @brief convert string to log level
 * @param cpLogLevel ... log level to parse
 * @author chifac08
 * @return log level enum
 */
LOG_LEVEL parseLogLevel(char* cpLogLevel)
{
	LOG_LEVEL logLevel = logConfig.logLevel;

	for(int i = 0; i < sizeof(logLevelString);i++)
	{
		if(strcmp(cpLogLevel, logLevelString[i]) == 0)
		{
			logLevel = i;
			break;
		}
	}

	return logLevel;
}
