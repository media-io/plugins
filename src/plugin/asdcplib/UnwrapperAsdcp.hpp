#ifndef _MEDIAIO_PLUGIN_ASDCPLIB_UNWRAPPER_HPP_
#define _MEDIAIO_PLUGIN_ASDCPLIB_UNWRAPPER_HPP_

#include <mediaio/api/unwrapper/unwrapper.h>

namespace ASDCP
{
namespace JP2K
{
	class MXFReader;
}
}

class UnwrapperAsdcp
{
public:
	UnwrapperAsdcp();
	~UnwrapperAsdcp();

	MediaioStatus open(MediaioPluginReader* reader, void* readerHandle);

	MediaioStatus configure(const Metadata* parameters);

	MediaioStatus unwrapNextFrame(const int streamIndex, CodedData* unwrappedFrame);

	MediaioStatus seekAtFrame(const int frame);

	MediaioStatus seekAtTime(const double time);

private:
	ASDCP::JP2K::MXFReader* _reader;
	long int                _frameIndex;
};

#endif
