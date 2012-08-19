

#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "File.h"

File::File(const std::string & file_name)
	:m_handle(nullptr)
{
	fopen_s(&m_handle, file_name.c_str(), "rb");
	if (m_handle == nullptr)
		throw std::exception("File not found");
}
File::~File()
{
	if (m_handle != nullptr)
	{
		fclose(m_handle);
		m_handle = nullptr;
	}
}

void File::MoveTo(const int seek_position)
{
	fseek(m_handle, seek_position, SEEK_SET);
}

void File::Skip(const int byte_count)
{
	fseek(m_handle, byte_count, SEEK_CUR);
}

int File::GetPos()
{
	return ftell(m_handle);
}

int File::ReadString(char * buffer, const int length) const
{
	return (int) fread(buffer, 1, length, m_handle);
}

std::string File::ReadString(const int length) const
{
	std::vector<char> tmp(length+1, '\0');
	fread(&tmp[0], 1, length, m_handle);
	return std::string(&tmp[0]);
}


int File::ReadInt4(int * result) const
{
	unsigned char buffer[4];
	int retval = (int) fread(&buffer, 1, 4, m_handle);
	// wad files are Little endian
	* result = (buffer[3]<<24) | (buffer[2]<<16) | (buffer[1]<<8) | (buffer[0]);
	return retval;
}

int File::ReadInt2(short * result) const
{
	unsigned char buffer[2];
	short retval = (int) fread(&buffer, 1, 2, m_handle);
	// wad files are Little endian
	* result = (buffer[1]<<8) | (buffer[0]);
	return retval;
}

int File::ReadInt1(char * result) const
{
	short retval = (int) fread(result, 1, 1, m_handle);
	return retval;
}

int File::ReadInt4() const
{
	unsigned char buffer[4];
	if (fread(&buffer, 1, 4, m_handle) != 4)
		throw("Bad file");
	// wad files are Little endian
	return (buffer[3]<<24) | (buffer[2]<<16) | (buffer[1]<<8) | (buffer[0]);
}

short File::ReadInt2() const
{
	unsigned char buffer[2];
	if (fread(&buffer, 1, 2, m_handle) != 2)
		throw("Bad file");
	// wad files are Little endian
	return (buffer[1]<<8) | (buffer[0]);
}

char File::ReadInt1() const
{
	char result;
	if (fread(&result, 1, 1, m_handle) != 1)
		throw("Bad file");
	return result;;
}

