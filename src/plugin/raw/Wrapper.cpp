
#include "Wrapper.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

Wrapper::Wrapper()
	: _writer       (NULL)
	, _writerHandle (NULL)
	, _init         (false)
{}

Wrapper::~Wrapper()
{
	// if(_writer->is_open())
	{
		_writer->close(_writerHandle);
	}
}

MediaioStatus Wrapper::open(MediaioPluginWriter* writer, void* writerHandle)
{
	_writer = writer;
	_writerHandle = writerHandle;
	return kMediaioStatusOK;
}

MediaioStatus Wrapper::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Wrapper::wrapNextFrame(const int streamIndex, CodedData* wrappedFrame)
{
	if(!_init)
	{
		MediaioStatus status = _writer->open(_writerHandle);

		if(status != kMediaioStatusOK)
		{
			return kMediaioStatusFailed;
		}
		_init = true;
	}

	_writer->write(_writerHandle, (char*)wrappedFrame->data, wrappedFrame->size);

	return kMediaioStatusOK;
}
