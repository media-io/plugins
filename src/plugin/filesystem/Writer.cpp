
#include "Writer.hpp"
#include <iostream>

Writer::Writer()
	: _filename("")
{}

Writer::~Writer()
{}

MediaioStatus Writer::set_filename(const char* filename)
{
	_filename = std::string(filename);
	return kMediaioStatusOK;
}

const char* Writer::get_filename()
{
	return _filename.c_str();
}

MediaioStatus Writer::open()
{
	if(_file.is_open()){
		_file.close();
	}
	_file.open(_filename);
	return kMediaioStatusOK;
}

MediaioStatus Writer::close()
{
	if(_file.is_open()){
		_file.close();
	}
	return kMediaioStatusOK;
}

uint64_t Writer::getPosition()
{
	if(_file.is_open()){
		return _file.tellp();
	}
	return 0;
}

uint64_t Writer::write(const char* data, const uint64_t size)
{
	_file.write(data, size);
	return size;
}

MediaioStatus Writer::seek(int64_t offset)
{
	if(_file.is_open()){
		_file.seekp(offset);
	}
	return kMediaioStatusOK;
}
