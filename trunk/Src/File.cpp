
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

int File::Load()
{
	return 0;
}

void File::UnLoad()
{
}

void File::Close()
{
	if (m_handle == NULL)
		return;
	fclose(m_handle);
}