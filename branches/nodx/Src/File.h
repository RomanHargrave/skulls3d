
#ifndef SK_FILE
#define SK_FILE

#include <stdio.h>
#include <string>

class File
{
protected:
	FILE * m_handle; // NULL when file is not opened

public:
	File(const std::string & file_name);
	~File();
	/**
	 * Success = 0,
	 * UnknownError = 1,
	 * DoesNotExist = 2,
	 * AlreadyOpen = 3,
	 */

	void MoveTo(const int seek_position);
	void Skip(const int byte_count);
	int GetPos();

	/**
	 * Error = 0
	 * Bytes read > 1
	 */
	int ReadString(char * buffer, const int length) const;
	std::string ReadString(const int length) const;
	/**
	 * Error = 0
	 * Bytes read > 1
	 */
	int ReadInt4(int * result) const;
	int ReadInt2(short * result) const;
	int ReadInt1(char * result) const;
	int ReadInt4() const;
	short ReadInt2() const;
	char ReadInt1() const;
};

#endif // SK_FILE