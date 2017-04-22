#ifndef _MEDIAIO_PLUGIN_FILESYSTEM_READER_HPP_
#define _MEDIAIO_PLUGIN_FILESYSTEM_READER_HPP_

#include <mediaio/api/reader/reader.h>

#include <fstream>

class Reader
{
public:
	Reader();
	~Reader();

	MediaioStatus set_filename(const char* filename);
	const char*   get_filename();
	MediaioStatus open();
	MediaioStatus close();
	int64_t       getSize();
	MediaioStatus setCacheSize(uint64_t size);
	uint64_t      getCacheSize();
	uint64_t      getPosition();

	uint64_t      read(char* data, const uint64_t size);
	MediaioStatus seek(int64_t offset);

private:
	std::string   _filename;
	std::ifstream _file;
};

#endif
