
#include "UnwrapperAsdcp.hpp"

#include <AS_DCP.h>

#include <iostream>

using namespace ASDCP::JP2K;

UnwrapperAsdcp::UnwrapperAsdcp()
	: _reader( nullptr )
	, _frameIndex( 0 )
{
}

UnwrapperAsdcp::~UnwrapperAsdcp()
{
}

MediaioStatus UnwrapperAsdcp::open(MediaioPluginReader* reader, void* readerHandle)
{
	// _reader = new MXFReader();
	// Kumu::Result_t result = _reader->OpenRead(filename);

	// if( ! ASDCP_SUCCESS(result) )
	// {
	// 	return kMediaioStatusFailed;
	// }

	return kMediaioStatusOK;
}

MediaioStatus UnwrapperAsdcp::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus UnwrapperAsdcp::unwrapNextFrame(const int streamIndex, CodedData* unwrappedFrame)
{
	FrameBuffer frameBuffer(4 * Kumu::Megabyte);
	Kumu::Result_t res = _reader->ReadFrame(_frameIndex, frameBuffer);

	if( ! ASDCP_SUCCESS( res ) )
	{
		return kMediaioStatusFailed;
	}

	++_frameIndex;

	resize_coded_data(unwrappedFrame, frameBuffer.Size());
	memcpy((void*)unwrappedFrame->data, frameBuffer.Data(), frameBuffer.Size());

	return kMediaioStatusOK;
}

MediaioStatus UnwrapperAsdcp::seekAtFrame(const int frame)
{
	_frameIndex = frame;
	return kMediaioStatusOK;
}

MediaioStatus UnwrapperAsdcp::seekAtTime(const double time)
{
	std::cout << "seek at time " << time << std::endl;
	return kMediaioStatusOK;
}
