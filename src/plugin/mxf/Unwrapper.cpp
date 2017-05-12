
#include "Unwrapper.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

Unwrapper::Unwrapper()
	: _reader       (NULL)
	, _readerHandle (NULL)
	, _init         (false)
{
}

Unwrapper::~Unwrapper()
{
	if(_init)
	{
		_reader->close(_readerHandle);
	}
}

MediaioStatus Unwrapper::open(MediaioPluginReader* reader, void* readerHandle)
{
	_reader = reader;
	_readerHandle = readerHandle;

	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::unwrapNextFrame(const int streamIndex, CodedData* codedData)
{
	if(!_init)
	{
		_reader->open(_readerHandle);
		_init = true;
	}

	size_t value_length = 0;
	while(1)
	{
		std::vector<unsigned char> key(16, 0);

		size_t size = _reader->read(_readerHandle, (char*)&key[0], 16);
		if(size != 16)
		{
			return kMediaioStatusFailed;
		}

		value_length = 0;

		unsigned char length = 0;
		_reader->read(_readerHandle, (char*)&length, 1);

		if((length & 0x80) == 0)
		{
			value_length = length;
		}
		else
		{
			std::vector<unsigned char> long_length(length - 0x80, 0);
			_reader->read(_readerHandle, (char*)&long_length[0], long_length.size());

			for(size_t i = 0; i < long_length.size(); ++i)
			{
				int shift = long_length.size() - 1 - i;
				value_length += (long_length.at(i) << 8 * shift);
			}
		}

		if(is_video_essence_key(key))
		{
			break;
		}

		_reader->seek(_readerHandle, value_length);
	}

	resize_coded_data(codedData, value_length);
	_reader->read(_readerHandle, (char*)codedData->data, value_length);
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

bool Unwrapper::is_video_essence_key(const std::vector<unsigned char>& key)
{
	std::vector<char> ref = {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01, 0x0d, 0x01, 0x03, 0x01, 0x15, 0x01, 0x05, 0x00};
	if(std::equal(ref.begin(), ref.end(), key.begin())){
		return true;
	}
	return false;
}
