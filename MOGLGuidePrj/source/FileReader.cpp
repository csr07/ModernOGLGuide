

#include <stdio.h>
#include "FileReader.h"

void FileReader::Read(const char* file, char** source) //test, using pointer to pointer, 
{
	FILE* pf;

	errno_t error = fopen_s(&pf, file, "rb");
	if (error == 0 && pf)
	{
		printf_s("Success to open file: %s \n", file);
		fseek(pf, 0, SEEK_END);
		long size = ftell(pf);
		fseek(pf, 0, SEEK_SET);

		*source = new char[size + 1];
		fread(*source, sizeof(char), size, pf);	
		(*source)[size] = 0; //or
		//source[0][size] = 0; //or 
		//char* src = *source;
		//source[size] = 0;
			
		fclose(pf);
	}
	else
	{
		printf_s("Failed to open file: %s \n", file);
	}
}

void FileReader::Read(const char* file, char*& source) //test, using reference to a pointer
{
	FILE* pf;
	errno_t err = fopen_s(&pf, file, "rb");
	if (err == 0)
	{
		printf_s("Success to open file: %s \n", file);
		fseek(pf, 0, SEEK_END);
		long size = ftell(pf);
		fseek(pf, 0, SEEK_SET);

		source = new char[size + 1];
		fread(source, sizeof(char), size, pf);
		source[size] = 0;

		fclose(pf);
	}
	else
	{
		printf_s("Failed to open file: %s \n", file);
	}
}