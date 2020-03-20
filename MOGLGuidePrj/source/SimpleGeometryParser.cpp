#include <stdio.h>
#include <string.h>

#include "SimpleGeometryParser.h"
#include "SimpleGeometry.h"

#include "FileReader.h"

SimpleGeometryParser::SimpleGeometryParser(Geometry* pg) :
	GeometryParser(pg)
{
	printf_s("in SimpleGeometryParser(Geometry*)");	
}

SimpleGeometryParser::~SimpleGeometryParser()
{
	printf_s("in ~SimpleGeometryParser()");
}

//////////////////////////////////

void SimpleGeometryParser::Parse()
{
	//Hacer la lectura del archivo	linea por linea
	printf_s("in SimpleGeometryParser::Parse()\n");	

	char* sourceFile;
	FileReader::Read(pGeometry->GetFileName(), &sourceFile);
	int sourceFileSize = sizeof(sourceFile);
	int sourceFileLen = strlen(sourceFile);	

	char* line;
	char* nextToken;
	
	line = strtok_s(sourceFile, "\n", &nextToken );

	int tNumVertex = 0;
	int tNumTriangles = 0;

	while (line)
	{
		//printf_s("-->%s\n", line);		

		SimpleGeometryParser::enumTokens token= CheckForTokenEnum(line);

		switch (token)
		{
		case SimpleGeometryParser::enumTokens::NAME :
		{
			char tname[100];
			int ret = sscanf_s(line, "#Name%s\n", &tname, (unsigned int)sizeof(tname));
			if (ret != 0)
			{
				printf_s("valor leido: %s <--\n", tname);
				printf_s("sizeof(tname): %d <--\n", (unsigned int)sizeof(tname));
				printf_s("strlen(tname) : %d <--\n", (int)strlen(tname));

				pGeometry->SetName(tname);
			}
		}
			break;
		case SimpleGeometryParser::enumTokens::VERTICES:
		{			
			int vertexPosDim = 0; // 2 for vec2 , 3 for vec3
			int vertexStrideDim = 0;
			sscanf_s(line, "#Vertices %d %d\n", &tNumVertex, &vertexPosDim);			

			vertexStrideDim = vertexPosDim + 3 + 2; // +3floats of color, + 2 floats for uvs
			
			float* buffer = new float[(tNumVertex * vertexStrideDim)]; 
			float* p = buffer;

			for (int i = 0; i < tNumVertex * vertexStrideDim; i += vertexStrideDim)
			{
				line = strtok_s(NULL, "\n", &nextToken);

				if (vertexPosDim == 2)
					int ret = sscanf_s(line, "%f, %f, %f, %f, %f, %f, %f\n", buffer + i, buffer + i + 1, buffer + i + 2, buffer + i + 3, buffer + i + 4, buffer + i + 5, buffer + i + 6);
				else if (vertexPosDim == 3)
					int ret = sscanf_s(line, "%f, %f, %f, %f, %f, %f, %f, %f\n", buffer + i, buffer + i + 1, buffer + i + 2, buffer + i + 3, buffer + i + 4, buffer + i + 5, buffer + i + 6, buffer + i + 7);
			}

			//print the vertex array
			for (int i = 0; i < tNumVertex * vertexStrideDim; i += vertexStrideDim)
			{
				if (vertexPosDim == 2)
					printf_s("vertex%d : ( %f, %f, %f, %f, %f, %f, %f )\n", i, *(buffer + i), *(buffer + i + 1), *(buffer + i + 2), *(buffer + i + 3), *(buffer + i + 4), *(buffer + i + 5), *(buffer + i + 6));
				if (vertexPosDim == 3)
					printf_s("vertex%d : ( %f, %f, %f, %f, %f, %f, %f, %f )\n", i, *(buffer + i), *(buffer + i + 1), *(buffer + i + 2), *(buffer + i + 3), *(buffer + i + 4), *(buffer + i + 5), *(buffer + i + 6), *(buffer + i + 7));
			}

			pGeometry->SetNumVertex(tNumVertex);
			pGeometry->SetVerticesLen(tNumVertex * vertexStrideDim);
			pGeometry->SetVertices(buffer);
		}
			break;
		case SimpleGeometryParser::enumTokens::TRIS:
		{
			sscanf_s(line, "#Triangles %d\n", &tNumTriangles);

			//ReadDataOfTypeAndDimension(pGeometry->GetVertices(), vertexStrideDim);		

			int* buffer = new int[(tNumTriangles * 3)];			

			for (int i = 0; i < tNumTriangles * 3; i += 3)
			{
				line = strtok_s(NULL, "\n", &nextToken);
								
				int ret = sscanf_s(line, "%d, %d, %d\n", buffer + i, buffer + i + 1, buffer + i + 2);
			}

			//print the vertex array
			for (int i = 0; i < tNumTriangles * 3; i += 3)
			{
				printf_s("triangle%d : ( %d , %d , %d )\n", i, *(buffer + i), *(buffer + i + 1), *(buffer + i + 2));
			}

			pGeometry->SetNumTriangles(tNumTriangles);
			pGeometry->SetElements(buffer);
		}
			break;
		default:
			break;
		}

		//read next line
		line = strtok_s(NULL, "\n", &nextToken);
	}

	delete sourceFile;
	sourceFile = NULL;
}

const char* SimpleGeometryParser::CheckForToken(const char* line)
{
	int tokenCount = strlen(*simpleGeometryTokens);
	const char* retToken = NULL;
	for (int i = 0; i < tokenCount; i++)
	{
		retToken = strstr(line, simpleGeometryTokens[i]);
		if (retToken != NULL) // Token Found
		{
			break;
		}
	}
	return retToken;
}

const SimpleGeometryParser::enumTokens SimpleGeometryParser::CheckForTokenEnum(const char* line)
{
	int tokenCount = 3;
	const char* retToken = NULL;
	SimpleGeometryParser::enumTokens found = SimpleGeometryParser::enumTokens::NONE;
	for (int i = 0; i < tokenCount; i++)
	{
		retToken = strstr(line, simpleGeometryTokens[i]);
		if (retToken != NULL) // Token Found
		{
			found = SimpleGeometryParser::enumTokens(i);
			break;
		}
	}
	return found;
}