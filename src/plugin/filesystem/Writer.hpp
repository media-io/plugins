#ifndef _MEDIAIO_PLUGIN_FILESYSTEM_WRITER_HPP_
#define _MEDIAIO_PLUGIN_FILESYSTEM_WRITER_HPP_

#include <mediaio/api/writer/writer.h>

#include <fstream>

class Writer
{
public:
	Writer();
	~Writer();

	MediaioStatus set_filename(const char* filename);
	const char*   get_filename();
	MediaioStatus open();
	MediaioStatus close();
	uint64_t      getPosition();

	uint64_t      write(const char* data, const uint64_t size);
	MediaioStatus seek(int64_t offset);

private:
	std::string   _filename;
	std::ofstream _file;
};

#endif
