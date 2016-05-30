#ifndef _LS_UTIL_H_
#define _LS_UTIL_H_

#include <string>
#include <ostream>
#include <vector>
#include <Windows.h>

#define USE_LS_UTIL_V2

enum Result
{
	OP_FOLDER_EMPTY = -2,
	OP_WRONG_PATH = -1,

	OP_SUCCESS = EXIT_SUCCESS, 
	OP_FAIL = EXIT_FAILURE, 
};


#if !defined(USE_LS_UTIL_V2)
class LsUtil
{
public:
#	ifdef _WIN32
	typedef WIN32_FIND_DATA FileData;
#	endif
#	ifdef _WIN64
	typedef WIN64_FIND_DATA FileData;
#	endif
	typedef HANDLE Handle;

	typedef std::vector< std::string > DirContent;

private: 
	LsUtil() {}
	LsUtil(const LsUtil&) {}
	const LsUtil& operator =(const LsUtil&) {}

public:
	LsUtil(const std::string& path);

	Result print();

	Result print_to(std::ostream& o_stream);
	Result print_to(std::string& o_string);

private:
	Result			_ParseDir(std::string& path);
	size_t			_CalculateSpace();
	void			_PutSpaces(std::ostream& o_stream, size_t wordSize);

private:
	std::string		m_path;
	DirContent		m_dir;

	size_t m_maxWordSize;
	size_t m_wordsPerLine;

};
#endif // USE_LS_UTIL_V2 || 

#if defined(USE_LS_UTIL_V2)

typedef std::string Path;

class DirParser
{
public:
#	ifdef _WIN32
	typedef WIN32_FIND_DATA FileData_t;
#	endif
#	ifdef _WIN64
	typedef WIN64_FIND_DATA FileData_t;
#	endif
	typedef HANDLE Handle;

	typedef std::string		dirEntry_t;
	typedef std::string		dirPath_t;
	typedef std::vector<dirEntry_t> dirContent_t;

	static Result ParseDirForPath(const dirPath_t& i_dirPath, dirContent_t& o_dirContent);
	static size_t GetMaxWordSize() 
	{
		return m_maxWordSize;
	}

private:
	static size_t m_maxWordSize;
};

///////////////////////////////////////////////////////////////

struct ConsoleWindowDelegate
{
public:
	ConsoleWindowDelegate();

	struct Size
	{
		unsigned int width;
		unsigned int height;

	} m_size;

	struct FontInfo
	{
		unsigned int width;
		unsigned int height;
	} m_fontInfo;
};

class LsUtil
{
	typedef DirParser::dirContent_t dirContent_t;

	static const int k_minWordsPerColumn = 6;

private:
	LsUtil() {}
	LsUtil(const LsUtil&) {}
	const LsUtil& operator=(const LsUtil&) {}

public:
	LsUtil(const Path& dirPath);

	Result print();
	Result print(std::ostream& o_stream);

	friend std::ostream& operator<<(std::ostream& o_stream, const LsUtil& i_lsUtil);

private:
	void _InitColumns();

private:
	Path m_dirPath;

	dirContent_t m_content;
	unsigned int m_nColumns;

	ConsoleWindowDelegate m_windowDelegate;
};

#endif // USE_LS_UTIL_V2
#endif // _LS_UTIL_H_