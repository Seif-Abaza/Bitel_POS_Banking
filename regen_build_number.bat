@echo off
set GEN_PATH=.
set GEN_TO=.

set GEN_EXE=build_incremet.exe
set GEN_HEADER=build_number.h

set GEN_PATH_EXE=%GEN_TO%\%GEN_EXE%
set GEN_PATH_HEADER=%GEN_PATH%\%GEN_HEADER%
set GEN_TO_HEADER=%GEN_TO%\build_number.h


IF EXIST %GEN_PATH_EXE% (

	%GEN_PATH_EXE% %GEN_PATH_HEADER%
	copy %GEN_PATH_HEADER% %GEN_TO_HEADER%

) ELSE (

 	%GEN_PATH_EXE% not found
)


IF NOT EXIST %GEN_TO_HEADER% (

	echo #define GENERATED_BUILD_NUMBER 0 > %GEN_TO_HEADER%
)



:: ВНИМАНИЕ! Установка Target для билда (используется в makefile)
set BITEL_PROGRAM_TARGET=DAMAN