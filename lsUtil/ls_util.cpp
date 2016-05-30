#include "ls_util.h"

#include <Windows.h>
#include <iostream>
#include <iomanip>

#if !defined(USE_LS_UTIL_V2)

LsUtil::LsUtil( const std::string& path )
	: m_path( path )
{
	_ParseDir( m_path );
}

Result LsUtil::print()
{
	return print_to(std::cout);
}

Result LsUtil::print_to(std::ostream& o_stream)
{
	if( !m_dir.size() )
	{
		return OP_FOLDER_EMPTY;
	}

	_CalculateSpace();

	DirContent::const_iterator it = m_dir.begin();
	for( int i = 0; it != m_dir.end(); ++it )
	{
		if( it->size() )
		{
			if( !(i % m_wordsPerLine) || (i == 0) )
			{
				o_stream << std::endl << *it;
				_PutSpaces( o_stream, it->size() );

				if( i == m_wordsPerLine )
				{
					i = 0;
				}
			}
			else if( 1 == (m_wordsPerLine - i) )
			{
				o_stream << *it;
			}
			else
			{
				o_stream << *it;
				_PutSpaces( o_stream, it->size() );
			}

			++i;
		}
	}
	o_stream << std::endl;

	return OP_SUCCESS;
}

Result LsUtil::_ParseDir( std::string& path )
{
	FileData fileData;
	Handle hFind;

	hFind = FindFirstFile( path.c_str(), &fileData );
	while( hFind != INVALID_HANDLE_VALUE )
	{
		std::string fileName( fileData.cFileName );

		if( fileName != std::string( "." ) || fileName != std::string( ".." ) )
		{
			m_dir.push_back( fileName );
			
			if( fileName.size() > m_maxWordSize )
			{
				m_maxWordSize = fileName.size();
			}
		}

		if( !FindNextFile( hFind, &fileData ) )
		{
			FindClose( hFind );
			hFind = INVALID_HANDLE_VALUE;

			return OP_WRONG_PATH;
		}
	} 
}

size_t LsUtil::_CalculateSpace()
{
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &consoleInfo );
	size_t consoleWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left;//consoleInfo.dwSize.Y;
	
	CONSOLE_FONT_INFO consoleFontInfo;
	GetCurrentConsoleFont( GetStdHandle( STD_OUTPUT_HANDLE ), false, &consoleFontInfo );
	size_t fontWidth = consoleFontInfo.dwFontSize.X;

	m_maxWordSize += 4; // Adding the size of tab 4 spaces
	m_wordsPerLine = static_cast< float >( consoleWidth ) / static_cast< float >( m_maxWordSize ) + 1;

	return 0;
}

void LsUtil::_PutSpaces( std::ostream& o_stream, size_t wordSize )
{
	size_t leftSpace = m_maxWordSize - wordSize;
	while( leftSpace-- )
	{
		o_stream << ' ';
	}
}

#endif // USE_LS_UTIL_V2

#if defined(USE_LS_UTIL_V2)

///////////////////////////////////////////////////////////////////////////////////////////

size_t DirParser::m_maxWordSize = 0;

Result DirParser::ParseDirForPath(const dirPath_t& i_dirPath, dirContent_t& o_dirContent)
{
	FileData_t fileData;
	Handle hFind;

	hFind = FindFirstFile(i_dirPath.c_str(), &fileData);
	while(hFind != INVALID_HANDLE_VALUE)
	{
		dirEntry_t fileName(fileData.cFileName);

		if(fileName != std::string(".") && fileName != std::string(".."))
		{
			o_dirContent.push_back(fileName);
			if(fileName.size() > m_maxWordSize)
			{
				m_maxWordSize = fileName.size();
			}
		}

		if(!FindNextFile(hFind, &fileData))
		{
			FindClose(hFind);
			hFind = INVALID_HANDLE_VALUE;
			break;
		}
	}
	
	if(o_dirContent.empty())
	{
		return OP_FOLDER_EMPTY;
	}
	else
	{
		return OP_SUCCESS;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

ConsoleWindowDelegate::ConsoleWindowDelegate()
{
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo );
	m_size.width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left;
	m_size.height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top;
	
	CONSOLE_FONT_INFO consoleFontInfo;
	GetCurrentConsoleFont( GetStdHandle(STD_OUTPUT_HANDLE), false, &consoleFontInfo );
	m_fontInfo.height = consoleFontInfo.dwFontSize.Y;
	m_fontInfo.width = consoleFontInfo.dwFontSize.X;
}

LsUtil::LsUtil(const Path& dirPath)
	: m_dirPath(dirPath)
	, m_content()
	, m_nColumns(1)
	, m_windowDelegate()
{
	Result code = DirParser::ParseDirForPath(dirPath, m_content);
	if(code == OP_SUCCESS)
	{
		_InitColumns();
	}
}

Result LsUtil::print()
{
	return print(std::cout);
}

Result LsUtil::print(std::ostream& o_stream)
{
	size_t wordsInColumn = static_cast<size_t>(m_content.size() / m_nColumns);
	while(wordsInColumn < k_minWordsPerColumn && m_nColumns != 1)
	{
		--m_nColumns;
		wordsInColumn = static_cast<size_t>(m_content.size() / m_nColumns);
	}

	for(int i = 0; i < wordsInColumn; ++i)
	{
		for(int j = 0; j < m_nColumns; j++)
		{
			size_t globalIdx = i + (wordsInColumn * j);
			o_stream << std::left << std::setw(DirParser::GetMaxWordSize() + 4) << m_content[globalIdx];
		}
		
		o_stream << '\n';
	}

	return OP_SUCCESS;
}

// ********************************************

void LsUtil::_InitColumns()
{
	size_t maxWordSize = DirParser::GetMaxWordSize();// * m_windowDelegate.m_fontInfo.width;
	m_nColumns = (float)(m_windowDelegate.m_size.width - 10) / (float)maxWordSize;
}

#endif // USE_LS_UTIL_V2