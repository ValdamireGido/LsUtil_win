#ifndef _LS_UTIL_H_
#define _LS_UTIL_H_

#include <string>
#include <ostream>
#include <vector>
#include <Windows.h>

enum Result
{
	OP_FOLDER_EMPTY = -2,
	OP_WRONG_PATH = -1,

	OP_SUCCESS = EXIT_SUCCESS, 
	OP_FAIL = EXIT_FAILURE, 
};



class LsUtil
{
public:
#ifdef _WIN32
	typedef WIN32_FIND_DATA FileData;
#endif
#ifdef _WIN64
	typedef WIN64_FIND_DATA FileData;
#endif
	typedef HANDLE Handle;

	typedef std::vector< std::string > DirContent;

private: 
	LsUtil();
	LsUtil( const LsUtil& );
	const LsUtil& operator =( const LsUtil& );

public:
	LsUtil( const std::string& path );

	Result print();

	Result print_to( std::ostream& o_stream );
	Result pring_to( std::string& o_string );

private:
	Result			_ParseDir( std::string& path );
	size_t			_CalculateSpace();
	void			_PutSpaces( std::ostream& o_stream, size_t wordSize );

private:
	std::string		m_path;
	DirContent		m_dir;

	size_t m_maxWordSize;
	size_t m_wordsPerLine;

};

#endif // _LS_UTIL_H_