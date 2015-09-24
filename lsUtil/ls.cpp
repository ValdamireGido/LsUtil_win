#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "ls_util.h"

int main( int argc, char** argv )
{
	Result result = OP_SUCCESS;
	std::string dirPath( argv[1] );

#ifdef _DEBUG
	dirPath = "d:\\*";
	LsUtil util( dirPath );
#else
	LsUtil util( dirPath );
#endif 

	util.print_to( std::cout );
	return result;
}