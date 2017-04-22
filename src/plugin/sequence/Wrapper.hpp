#ifndef _MEDIAIO_PLUGIN_SEQUENCE_WRAPPER_HPP_
#define _MEDIAIO_PLUGIN_SEQUENCE_WRAPPER_HPP_

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
	std::string          _pattern;
	std::string          _prefix;
	std::string          _suffix;
	size_t               _padding;
	size_t               _currentFrame;
};

#endif
