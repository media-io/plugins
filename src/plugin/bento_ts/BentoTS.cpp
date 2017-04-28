
#include <wrapper/Plugin.hpp>
#include "Wrapper.hpp"

MediaioStatus createWrapperInstance(void** handle)
{
	Wrapper* instance = new Wrapper();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteWrapperInstance(void** handle)
{
	delete (Wrapper*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus wrapperSetWriter(void* handle, MediaioPluginWriter* writer, void* writerHandle)
{
	Wrapper* instance = (Wrapper*) handle;
	return instance->setWriter(writer, writerHandle);
}

MediaioStatus wrapperConfigure(void* handle, const Metadata* parameters)
{
	Wrapper* instance = (Wrapper*) handle;
	return instance->configure(parameters);
}

MediaioStatus wrapperWrapNextFrame(void* handle, const int streamIndex, CodedData* unwrappedFrame)
{
	Wrapper* instance = (Wrapper*) handle;
	return instance->wrapNextFrame(streamIndex, unwrappedFrame);
}

static MediaioPluginInstance WrapperInstance =
{
	createWrapperInstance,
	deleteWrapperInstance
};

static MediaioPluginWrapper Wrapper =
{
	wrapperSetWriter,
	wrapperConfigure,
	wrapperWrapNextFrame
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginActionWrapper(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:  { return &WrapperInstance; }
		case PluginActionWrapper: { return &Wrapper; }
		default: return nullptr;
	}
}

Plugin wrapperPlugin = Plugin(
	PluginApiWrapper,
	"fr.co.mediaio:bento_ts",
	"Mpeg2 TS muxer",
	"Mux transport streams using Bento library",
	1,
	0,
	pluginActionWrapper
);

std::vector<Plugin> plugins = {wrapperPlugin};
