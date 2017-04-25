#ifndef _MEDIAIO_PLUGIN_BENTO_TS_WRAPPER_HPP_
#define _MEDIAIO_PLUGIN_BENTO_TS_WRAPPER_HPP_

#include <mediaio/api/wrapper/wrapper.h>

#include <Ap4.h>
#include <string>

class Buffer;

class Wrapper
{
public:
	Wrapper();
	~Wrapper();

	MediaioStatus setWriter(MediaioPluginWriter* writer, void* writerHandle);

	MediaioStatus configure(const Metadata* parameters);

	MediaioStatus wrapNextFrame(const int streamIndex, CodedData* wrappedFrame);

private:

	MediaioStatus init(Buffer& buffer);

	MediaioPluginWriter* _writer;
	void*                _writerHandle;
	AP4_Mpeg2TsWriter    _wrapper;
	AP4_Mpeg2TsWriter::SampleStream* _video_stream;
	uint64_t             _dts;
	bool                 _init;
};

#endif
