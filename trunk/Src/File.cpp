
#include <string.h>
#include <stdio.h>

#include "File.h"

File::File(const char * filename)
{
	this->m_filename = _strdup(filename);
	this->m_handle = NULL;
}

File::~File()
{
	Close();
	delete this->m_filename;
}


int File::Open()
{
	if (IsOpen())
		return 3;
	fopen_s(&m_handle, m_filename, "rb");
	if (m_handle == 0)
		return 1;
	return 0;
}
bool File::IsOpen() const
{
	return (m_handle != 0);
}
void File::Close()
{
	if (m_handle == NULL)
		return;
	fclose(m_handle);
}

void File::MoveTo(const int seek_position)
{
	if (!IsOpen())
		return;

	fseek(m_handle, seek_position, SEEK_SET);
}

void File::Skip(const int byte_count)
{
	fseek(m_handle, byte_count, SEEK_CUR);
}

int File::ReadString(char * buffer, const int length) const
{
	if (!IsOpen())
		return 0;

	return (int) fread(buffer, 1, length, m_handle);
}

int File::ReadInt4(int * result) const
{
	if (!IsOpen())
		return 0;

	unsigned char buffer[4];
	int retval = (int) fread(&buffer, 1, 4, m_handle);
	// wad files are Little endian
	* result = (buffer[3]<<24) | (buffer[2]<<16) | (buffer[1]<<8) | (buffer[0]);
	return retval;
}

int File::ReadInt2(short * result) const
{
	if (!IsOpen())
		return 0;

	unsigned char buffer[2];
	short retval = (int) fread(&buffer, 1, 2, m_handle);
	// wad files are Little endian
	* result = (buffer[1]<<8) | (buffer[0]);
	return retval;
}

