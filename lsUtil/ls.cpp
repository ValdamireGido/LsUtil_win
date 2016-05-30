#include <cstdio>
#include <direct.h>
#include <cstdlib>
#include <iostream>

#include "ls_util.h"

//#define LS_DEBUG

int main(int argc, char** argv)
{
	Result result = OP_SUCCESS;
	std::string dirPath;
	if(argc > 1)
	{
		dirPath = argv[1];
	}
	else
	{
		dirPath = argv[0];
		dirPath.resize(dirPath.find_last_of("\\") + 1);
		dirPath += '*';
	} 

#ifdef LS_DEBUG
	dirPath = "d:\\*";
	LsUtil util(dirPath);
#else
	LsUtil util(dirPath);
#endif 

	util.print(std::cout);
	return result;
}