
#include <string.h>

#include "File.h"

File::File(const char * filename)
{
	this->m_filename = _strdup(filename);
}

File::~File()
{
	UnLoad();
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
}
bool File::IsOpen()
{
	return (m_handle != 0);
}
void File::Close()
{
	if (m_handle == NULL)
		return;
	fclose(m_handle);
}

int File::Load()
{
	return 0;
}

void File::UnLoad()
{
}