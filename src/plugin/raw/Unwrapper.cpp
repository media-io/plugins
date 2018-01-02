
#include "Unwrapper.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

Unwrapper::Unwrapper()
	: _reader       (NULL)
	, _readerHandle (NULL)
{
}

Unwrapper::~Unwrapper()
{
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
	// _reader->open(_readerHandle);
	// int size = _reader->get_size(_readerHandle);

	// if(size <= 0)
	// {
	// 	return kMediaioStatusFailed;
	// }

	// resize_coded_data(unwrappedFrame, size);

	// _reader->read(_readerHandle, (char*)unwrappedFrame->data, size);
	// _reader->close(_readerHandle);

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
