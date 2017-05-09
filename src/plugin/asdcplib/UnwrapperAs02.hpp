#ifndef _MEDIAIO_PLUGIN_ASDCPLIB_UNWRAPPER_AS02_HPP_
#define _MEDIAIO_PLUGIN_ASDCPLIB_UNWRAPPER_AS02_HPP_

#include <mediaio/api/unwrapper/unwrapper.h>

namespace AS_02
{
namespace JP2K
{
	class MXFReader;
}
}

class UnwrapperAs02
{
public:
	UnwrapperAs02();
	~UnwrapperAs02();

	MediaioStatus open(MediaioPluginReader* reader, void* readerHandle);
	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus unwrapNextFrame(const int streamIndex, CodedData* unwrappedFrame);
	MediaioStatus seekAtFrame(const int frame);
	MediaioStatus seekAtTime(const double time);

private:
	// AS_02::JP2K::MXFReader* _readerHandle;
	AS_02::JP2K::MXFReader* _reader;
	long int                _frameIndex;
};

#endif
