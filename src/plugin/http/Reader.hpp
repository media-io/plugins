#ifndef _MEDIAIO_PLUGIN_FILESYSTEM_READER_HPP_
#define _MEDIAIO_PLUGIN_FILESYSTEM_READER_HPP_

#include <mediaio/api/reader/reader.h>

#include <fstream>
#include <vector>

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
	MediaioStatus load_buffer(uint64_t min_size);

private:
	std::string       _filename;
	std::vector<char> _buffer;
	double            _size;
	double            _position;
};

#endif
