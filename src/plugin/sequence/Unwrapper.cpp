
#include "Unwrapper.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

Unwrapper::Unwrapper()
	: _reader       (NULL)
	, _readerHandle (NULL)
	, _pattern      ("")
	, _prefix       ("")
	, _suffix       ("")
	, _padding      (0)
	, _currentFrame (0)
{
}

Unwrapper::~Unwrapper()
{
}

MediaioStatus Unwrapper::open(MediaioPluginReader* reader, void* readerHandle)
{
	_reader = reader;
	_readerHandle = readerHandle;
	const char* str_filename = _reader->get_filename(_readerHandle);
	_pattern = std::string(str_filename);
	_currentFrame = 0;

	int prefixIndex = -1;
	int suffixIndex = -1;
	bool patternFound = false;

	for(size_t index = _pattern.size(); index > 0; --index)
	{
		switch(_pattern[index])
		{
			case '#':
			{
				if(suffixIndex == -1)
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
				if((suffixIndex != -1) &&
					(prefixIndex != -1))
				{
					patternFound = true;
				}
				break;
			}
		}
		if(patternFound)
		{
			break;
		}
	}

	_prefix = _pattern.substr(0, prefixIndex);
	_suffix = _pattern.substr(suffixIndex, _pattern.size());

	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::getDescription(struct MediaioFileDescriptor* descriptor)
{
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::getStreamDescription(const int streamIndex, struct MediaioStreamDescriptor* descriptor)
{
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::unwrapNextFrame(const int streamIndex, CodedData* unwrappedFrame)
{
	std::ostringstream filenameAtFrame;

	filenameAtFrame << std::setfill('0');

	filenameAtFrame << _prefix;
	filenameAtFrame << std::setw(_padding) << _currentFrame;
	filenameAtFrame << _suffix;

	// std::cout << filenameAtFrame.str() << std::endl;
	++_currentFrame;

	_reader->set_filename(_readerHandle, filenameAtFrame.str().c_str());
	_reader->open(_readerHandle);
	int size = _reader->get_size(_readerHandle);

	if(size <= 0)
	{
		return kMediaioStatusFailed;
	}

	resize_coded_data(unwrappedFrame, size);

	_reader->read(_readerHandle, (char*)unwrappedFrame->data, size);
	_reader->close(_readerHandle);

	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::seekAtFrame(const int frame)
{
	std::cout << "seek at frame " << frame << std::endl;
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::seekAtTime(const double time)
{
	std::cout << "seek at time " << time << std::endl;
	return kMediaioStatusOK;
}
