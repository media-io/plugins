#ifndef _MEDIAIO_PLUGIN_SEQUENCE_UNWRAPPER_HPP_
#define _MEDIAIO_PLUGIN_SEQUENCE_UNWRAPPER_HPP_

#include <mediaio/api/unwrapper/unwrapper.h>

#include <string>

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
	MediaioPluginReader* _reader;
	void*                _readerHandle;
	std::string          _pattern;
	std::string          _prefix;
	std::string          _suffix;
	size_t               _padding;
	size_t               _currentFrame;
};

#endif
