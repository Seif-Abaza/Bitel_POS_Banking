
#ifndef __CFG_H
#define __CFG_H

#define NUM_TO_STR(n)				#n
#define MACRO_VALUE_TO_STR(n)		NUM_TO_STR(n)

#define PROGRAM_DEVICE			"BITEL F5100"
#define PROGRAM_DATE			__DATE__

// версия (должно быть ровно по одной цифре!)


// #define PROGRAM_NAME		"DAMAN"
// #define PROGRAM_VERSION		"v0.5.0"

#define APP_MAIN 1
#define APP_ACCESSORY 0

// Настраивается в makefile_settings
//#define MY_DEBUG			// для вывода в ком
//#define MY_DEBUG_ROUTER		// включить вместе с MY_DEBUG для распределения данных в сервер и в дебаг
//#define MEM_DEBUG			// для поиска утечек памяти

#define NO_CRYPTO_CARD
//#define PROJ_ETHERNET

#ifdef MEM_DEBUG
void* operator new(size_t nReqSize, const char* strFile, long nLine)
void* operator new[](size_t nReqSize, const char* strFile, long nLine);
#define new new(__FILE__, __LINE__)
#endif

#endif
