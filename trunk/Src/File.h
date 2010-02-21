#include <stdio.h>

#ifndef SK_FILE
#define SK_FILE

class File
{
protected:
	char * m_filename;
	FILE * m_handle; // NULL when file is not opened

public:
	File(const char * filename);
	~File();

	/**
	 * Success = 0,
	 * UnknownError = 1,
	 * DoesNotExist = 2,
	 * AlreadyOpen = 3,
	 */
	int Open();
	void Close();
	bool IsOpen();

	/**
	 * Success = 0,
	 * UnknownError = 1,
	 * FileNotOpen = 2,
	 */
	virtual int Load();
	virtual void UnLoad();

	void MoveTo(int seek_position);
	void Skip(int byte_count);
};

#endif // SK_FILE