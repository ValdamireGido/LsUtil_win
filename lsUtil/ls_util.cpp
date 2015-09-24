#include "ls_util.h"

#include <Windows.h>
#include <iostream>
#include <iomanip>

LsUtil::LsUtil( const std::string& path )
	: m_path( path )
{
	_ParseDir( m_path );
}

Result LsUtil::print()
{
	return print_to( std::cout );
}

Result LsUtil::print_to( std::ostream& o_stream )
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