#include <iostream>
#include <cstring>
#include <string>
#include <fstream>

typedef unsigned int uint32;
typedef int int32;

typedef struct
{
	char wdbc[3];
	uint32 rows;
	uint32 cols;
	uint32 lengh;
	uint32 block; 
} DBC_hlavicka;

typedef struct
{
	uint32 id; 							// 0
	uint32 mapid;						// 1
	uint32 zone;						// 2
	uint32 exploreFlag;					// 3
	uint32 flags;						// 4
	uint32 soundProviderPref;			// 5
	uint32 soundProviderPrefUnderwater;	// 6
	int32 ambienceId;					// 7
	int32 zoneMusic;					// 8
	int32 introSound;					// 9
	int32 area_level;					// 10
	uint32 area_name[16];				// 11-26
	uint32 str_flags;					// 27
	uint32 team;						// 28
	uint32 LiquidTypeOverride[4];		// 29-32
	uint32 minElevation;				// 33
	uint32 ambientMultiplier;			// 34
	uint32 lightId;						// 35
} AreaTable;

std::string getText(FILE *f, uint32 offs)
{
	std::string ret;

	char r;
	uint32 pos = ftell(f);
	fseek(f, offs, SEEK_SET);

	while(fread(&r, 1, sizeof(r), f))
	{
		if(r != NULL)
		{
			ret += r;
		}
		else
		{
			fseek(f, pos, SEEK_SET);
			return ret;
		}
	}
	return "";
}

int main(int argc, char const *argv[])
{
	if (argc % 2 == 0)
	{
		std::cout << "Chyba argumentace" << std::endl;
		return -1;
	}

	std::string path;

	for (int i = 0; i < argc; ++i)
	{
		if(strcmp(argv[i], "-src") == 0)
		{
			path = argv[i+1];
		}
	}

	std::cout << path << std::endl;

	FILE *soubor;
	if ((soubor = fopen(path.c_str(), "rb")) == NULL) 
	{
		std::cout << "Chybišká" << std::endl;
		return -1;
	}

	fseek(soubor, 0, SEEK_END);
	uint32 fsize = ftell(soubor);
	fseek(soubor, 0, SEEK_SET);

	size_t ret;
	DBC_hlavicka head;
	ret = fread(&head, 1, sizeof(head), soubor);

	std::cout << "Format: " 	<< head.wdbc << std::endl;
	std::cout << "Rows: " 		<< head.rows << std::endl;
	std::cout << "Cols: " 		<< head.cols << std::endl;
	std::cout << "Row lengh: " 	<< head.lengh << std::endl;
	std::cout << "Text block: " << head.block << std::endl;
	
	AreaTable row;

	for (int q = 0; q < head.rows; ++q)
	{
		ret = fread(&row, 1, sizeof(row), soubor);
		std::cout << "Id: " << row.id << std::endl;
		std::cout << "Area name: [" << row.area_name[0] 
			<< "] " << getText(soubor, fsize-head.block+row.area_name[0]) << std::endl;
	}

	return 0;
}