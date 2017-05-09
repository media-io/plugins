#ifndef _MEDIAIO_PLUGIN_RAW_WRAPPER_HPP_
#define _MEDIAIO_PLUGIN_RAW_WRAPPER_HPP_

#include <mediaio/api/wrapper/wrapper.h>

#include <string>

class Wrapper
{
public:
	Wrapper();
	~Wrapper();

	MediaioStatus open(MediaioPluginWriter* writer, void* writerHandle);

	MediaioStatus configure(const Metadata* parameters);

	MediaioStatus wrapNextFrame(const int streamIndex, CodedData* wrappedFrame);

private:
	MediaioPluginWriter* _writer;
	void*                _writerHandle;

	bool                 _init;
};

#endif
