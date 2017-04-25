
#include "UnwrapperAs02.hpp"

#include <AS_02.h>

#include <iostream>

using namespace AS_02::JP2K;

UnwrapperAs02::UnwrapperAs02()
	: _reader( nullptr )
	, _frameIndex( 0 )
{
}

UnwrapperAs02::~UnwrapperAs02()
{
}

MediaioStatus UnwrapperAs02::open(MediaioPluginReader* reader, void* readerHandle)
{
	// _reader = new MXFReader();
	// Kumu::Result_t result = _reader->OpenRead(filename);

	// if( ! ASDCP_SUCCESS(result) )
	// {
	// 	return kMediaioStatusFailed;
	// }

	return kMediaioStatusOK;
}

MediaioStatus UnwrapperAs02::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus UnwrapperAs02::unwrapNextFrame(const int streamIndex, CodedData* unwrappedFrame)
{
	ASDCP::JP2K::FrameBuffer frameBuffer( 4 * Kumu::Megabyte );
	Kumu::Result_t res = _reader->ReadFrame(_frameIndex, frameBuffer);

	if( ! ASDCP_SUCCESS( res ) )
	{
		return kMediaioStatusFailed;
	}

	++_frameIndex;

	resize_coded_data(unwrappedFrame, frameBuffer.Size());
	memcpy( (void*)unwrappedFrame->data, frameBuffer.Data(), frameBuffer.Size());
	return kMediaioStatusOK;
}

MediaioStatus UnwrapperAs02::seekAtFrame(const int frame)
{
	_frameIndex = frame;
	return kMediaioStatusOK;
}

MediaioStatus UnwrapperAs02::seekAtTime(const double time)
{
	std::cout << "seek at time " << time << std::endl;
	return kMediaioStatusOK;
}
