
#include "Wrapper.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

Wrapper::Wrapper()
	: _writer       (NULL)
	, _writerHandle (NULL)
	, _pattern      ("")
	, _prefix       ("")
	, _suffix       ("")
	, _padding      (0)
	, _currentFrame (0)
{}

Wrapper::~Wrapper()
{}

MediaioStatus Wrapper::open(MediaioPluginWriter* writer, void* writerHandle)
{
	_writer = writer;
	_writerHandle = writerHandle;

	const char* str_filename = _writer->get_filename(_writerHandle);
	_pattern = std::string(str_filename);
	_currentFrame = 0;

	int prefixIndex = -1;
	int suffixIndex = -1;
	bool patternFound = false;

	for( size_t index = _pattern.size(); index > 0; --index )
	{
		switch( _pattern[index] )
		{
			case '#':
			{
				if( suffixIndex == -1 )
				{
					suffixIndex = index + 1;
					++_padding;
				}
				else
				{
					prefixIndex = index;
					++_padding;
				}
				break;
			}
			default:
			{
				if( ( suffixIndex != -1 ) &&
					( prefixIndex != -1 ) )
				{
					patternFound = true;
				}
				break;
			}
		}
		if( patternFound )
		{
			break;
		}
	}

	_prefix = _pattern.substr( 0, prefixIndex );
	_suffix = _pattern.substr( suffixIndex, _pattern.size() );

	return kMediaioStatusOK;
}

MediaioStatus Wrapper::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Wrapper::wrapNextFrame(const int streamIndex, CodedData* wrappedFrame)
{
	std::ostringstream filenameAtFrame;

	filenameAtFrame << std::setfill('0');

	filenameAtFrame << _prefix;
	filenameAtFrame << std::setw(_padding) << _currentFrame;
	filenameAtFrame << _suffix;

	// std::cout << filenameAtFrame.str() << std::endl;
	++_currentFrame;

	_writer->set_filename(_writerHandle, filenameAtFrame.str().c_str());
	MediaioStatus status = _writer->open(_writerHandle);

	if(status != kMediaioStatusOK)
	{
		return kMediaioStatusFailed;
	}

	_writer->write(_writerHandle, (char*)wrappedFrame->data, wrappedFrame->size);
	_writer->close(_writerHandle);

	return kMediaioStatusOK;
}
