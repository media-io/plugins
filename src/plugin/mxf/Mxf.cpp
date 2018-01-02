
#include <wrapper/Plugin.hpp>
#include "Unwrapper.hpp"

MediaioStatus unwrapperCreateInstance(void** handle)
{
	Unwrapper* instance = new Unwrapper();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus unwrapperDeleteInstance(void** handle)
{
	delete (Unwrapper*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus unwrapperOpen(void* handle, MediaioPluginReader* reader, void* readerHandle)
{
	Unwrapper* instance = (Unwrapper*) handle;
	return instance->open(reader, readerHandle);
}

MediaioStatus unwrapperConfigure(void* handle, const Metadata* parameters)
{
	Unwrapper* instance = (Unwrapper*) handle;
	return instance->configure(parameters);
}

MediaioStatus unwrapperGetFileDescription(void* handle, struct MediaioFileDescriptor* descriptor)
{
	Unwrapper* instance = (Unwrapper*) handle;
	return instance->getDescription(descriptor);
}

MediaioStatus unwrapperGetStreamDescription(void* handle, const int streamIndex, struct MediaioStreamDescriptor* descriptor)
{
	Unwrapper* instance = (Unwrapper*) handle;
	return instance->getStreamDescription(streamIndex, descriptor);
}

MediaioStatus unwrapperUnwrapNextFrame(void* handle, const int streamIndex, CodedData* unwrappedFrame)
{
	Unwrapper* instance = (Unwrapper*) handle;
	return instance->unwrapNextFrame(streamIndex, unwrappedFrame);
}

MediaioStatus unwrapperSeekAtFrame(void* handle, const int frame)
{
	Unwrapper* instance = (Unwrapper*) handle;
	return instance->seekAtFrame(frame);
}

MediaioStatus unwrapperSeekAtTime(void* handle, const double time)
{
	Unwrapper* instance = (Unwrapper*) handle;
	return instance->seekAtFrame(time);
}

static MediaioPluginInstance UnwrapperInstance =
{
	unwrapperCreateInstance,
	unwrapperDeleteInstance
};

static MediaioPluginUnwrapper Unwrapper =
{
	unwrapperOpen,
	unwrapperConfigure,
	unwrapperGetFileDescription,
	unwrapperGetStreamDescription,
	unwrapperUnwrapNextFrame,
	unwrapperSeekAtFrame,
	unwrapperSeekAtTime
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginActionUnwrapper(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:  { return &UnwrapperInstance; }
		case PluginActionUnwrapper: { return &Unwrapper; }
		default: return nullptr;
	}
}

Plugin unwrapperPlugin = Plugin(
	PluginApiUnwrapper,
	"fr.co.mediaio:mxfunwrapper",
	"MXF Unwrapper",
	"Read video stream into MXF file",
	1,
	0,
	pluginActionUnwrapper
);

std::vector<Plugin> plugins = {unwrapperPlugin};
