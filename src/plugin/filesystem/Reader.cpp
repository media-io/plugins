
#include "Reader.hpp"
#include <iostream>

Reader::Reader()
	: _filename("")
{}

Reader::~Reader()
{}

MediaioStatus Reader::set_filename(const char* filename)
{
	_filename = std::string(filename);
	return kMediaioStatusOK;
}

const char* Reader::get_filename()
{
	return _filename.c_str();
}

MediaioStatus Reader::open()
{
	if(_file.is_open()){
		_file.close();
	}
	_file.open(_filename);
	return kMediaioStatusOK;
}

MediaioStatus Reader::close()
{
	if(_file.is_open()){
		_file.close();
	}
	return kMediaioStatusOK;
}

int64_t Reader::getSize()
{
	std::ifstream file(_filename, std::ios::binary | std::ios::ate);
	if(!file.is_open())
	{
		return -1;
	}
	return file.tellg();
}

MediaioStatus Reader::setCacheSize(uint64_t size)
{
	return kMediaioStatusOK;
}

uint64_t Reader::getCacheSize()
{
	return 0;
}

uint64_t Reader::getPosition()
{
	if(_file.is_open()){
		return _file.tellg();
	}
	return 0;
}

uint64_t Reader::read(char* data, uint64_t size)
{
	_file.read(data, size);
	return size;
}

MediaioStatus Reader::seek(int64_t offset)
{
	if(_file.is_open()){
		_file.seekg(offset, std::ios_base::cur);
	}
	return kMediaioStatusOK;
}
