
#ifndef SK_FILE
#define SK_FILE

#include <stdio.h>

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
	bool IsOpen() const;

	void MoveTo(const int seek_position);
	void Skip(const int byte_count);
	int GetPos();

	/**
	 * Error = 0
	 * Bytes read > 1
	 */
	int ReadString(char * buffer, const int length) const;
	/**
	 * Error = 0
	 * Bytes read > 1
	 */
	int ReadInt4(int * result) const;
	int ReadInt2(short * result) const;
	int ReadInt1(char * result) const;
};

#endif // SK_FILE