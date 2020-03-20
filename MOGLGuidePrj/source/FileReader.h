#pragma once

class FileReader
{
public:
	//static void Read(const char* file, char* source);
	static void Read(const char* file, char** source); // with pointer to pointer
	static void Read(const char* file, char*& source); //with reference to a char pointer
};