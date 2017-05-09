#ifndef _MEDIAIO_PLUGIN_MXF_UNWRAPPER_HPP_
#define _MEDIAIO_PLUGIN_MXF_UNWRAPPER_HPP_

#include <mediaio/api/unwrapper/unwrapper.h>

#include <string>

class Unwrapper
{
public:
	Unwrapper();
	~Unwrapper();

	MediaioStatus open(MediaioPluginReader* reader, void* readerHandle);
	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus unwrapNextFrame(const int streamIndex, CodedData* unwrappedFrame);
	MediaioStatus seekAtFrame(const int frame);
	MediaioStatus seekAtTime(const double time);

private:
	bool is_video_essence_key(const std::vector<unsigned char>& key);

	MediaioPluginReader* _reader;
	void*                _readerHandle;
	bool                 _init;
};

#endif
