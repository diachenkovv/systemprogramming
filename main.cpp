// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#define bufsize 10240

int main(int argc, char *argv[]) {


	HANDLE hFileSource, hFileTarget;
	char DataBuffer[bufsize];
	DWORD dwBytesToWrite = bufsize;
	DWORD dwBytesWritten = 0, dwBytesWrittenAll=0;
	DWORD dwBytesToTransfer = bufsize;
	DWORD dwBytesTransfered = 0;
	BOOL bErrorFlag = FALSE;

	printf("\n");

	if (argc != 3)
	{
		printf("Usage Error:\tIncorrect number of arguments\n\n");
		printf("%s <file_name>\n", argv[0]);
		return 0;
	}

	hFileTarget = CreateFile(argv[2],                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_ALWAYS,          // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFileTarget == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile");
		printf("Terminal failure: Unable to open file \"%s\" for write.\n", argv[2]);
		return 0;
	}

	printf("Writing %d bytes to %s.\n", dwBytesToWrite, argv[2]);

	//**************************************************************************
	hFileSource = CreateFile(argv[1],                // name of the write
		GENERIC_READ,          // open for writing
		FILE_SHARE_READ,                      // do  share
		NULL,                   // default security
		OPEN_EXISTING,             // 
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFileSource == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile");
		printf("Terminal failure: Unable to open file \"%s\" for read.\n", argv[1]);
		return 0;
	}

	//*********************************

	do {
		bErrorFlag = ReadFile(
			hFileSource,           // open file handle
			DataBuffer,      // start of data to write
			dwBytesToTransfer,  // number of bytes to write
			&dwBytesTransfered, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			printf("readFile");
			printf("Terminal failure: Unable to read file.\n");
			return 0;
		}




		//**********************************

		bErrorFlag = WriteFile(
			hFileTarget,           // open file handle
			DataBuffer,      // start of data to write
			dwBytesTransfered,  // number of bytes to write
			&dwBytesWritten, // number of bytes that were written
			NULL);            // no overlapped structure

		if (FALSE == bErrorFlag)
		{
			printf("WriteFile");
			printf("Terminal failure: Unable to write to file.\n");
		}
		else
		{
			if (dwBytesWritten != dwBytesTransfered)
			{
				// This is an error because a synchronous write that results in
				// success (WriteFile returns TRUE) should write all data as
				// requested. This would not necessarily be the case for
				// asynchronous writes.
				printf("Error: dwBytesWritten != dwBytesToWrite\n");
			}
			else
			{
				printf("Wrote %10.0f Kbytes to %s successfully. \r", (dwBytesWrittenAll+=dwBytesWritten)*1.0/1024, argv[2]);
			}
		}

	} while (dwBytesTransfered);


	CloseHandle(hFileSource);
	CloseHandle(hFileTarget);

	return 0;
}