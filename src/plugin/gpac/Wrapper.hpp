#ifndef _MEDIAIO_PLUGIN_SEQUENCE_WRAPPER_HPP_
#define _MEDIAIO_PLUGIN_SEQUENCE_WRAPPER_HPP_

#include <mediaio/api/wrapper/wrapper.h>

#include <string>
#include <gpac/media_tools.h>
#include <gpac/constants.h>
#include <gpac/base_coding.h>
#include <gpac/mpegts.h>

class Wrapper
{
public:
	Wrapper();
	~Wrapper();

	MediaioStatus open(MediaioPluginWriter* writer, void* writerHandle);
	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus wrapNextFrame(const int streamIndex, CodedData* wrappedFrame);

private:
	MediaioStatus init();

	MediaioPluginWriter* _writer;
	void*                _writerHandle;
	GF_M2TS_Mux*         _muxer;
	bool                 _init;
};

#endif
