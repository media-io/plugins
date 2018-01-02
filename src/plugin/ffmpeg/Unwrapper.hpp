#ifndef _MEDIAIO_PLUGIN_FFMPEG_UNWRAPPER_HPP_
#define _MEDIAIO_PLUGIN_FFMPEG_UNWRAPPER_HPP_

#include <mediaio/api/unwrapper/unwrapper.h>

#include <string>

extern "C" {
#include <libavformat/avformat.h>
}

class Unwrapper
{
public:
	Unwrapper();
	~Unwrapper();

	MediaioStatus open(MediaioPluginReader* reader, void* readerHandle);
	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus getDescription(struct MediaioFileDescriptor* descriptor);
	MediaioStatus getStreamDescription(const int streamIndex, struct MediaioStreamDescriptor* descriptor);
	MediaioStatus unwrapNextFrame(const int streamIndex, CodedData* unwrappedFrame);
	MediaioStatus seekAtFrame(const int frame);
	MediaioStatus seekAtTime(const double time);

private:

	static int read_packet(void *opaque, uint8_t *buf, int buf_size);

	AVFormatContext*     _avFormatContext;

	// AVIOContext*         _avio_ctx;
	MediaioPluginReader* _reader;
	void*                _readerHandle;

	bool _init;
};

#endif
