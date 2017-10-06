
#include <wrapper/Plugin.hpp>

#include "Wrapper.hpp"

MediaioStatus wrapperCreateInstance(void** handle)
{
	Wrapper* instance = new Wrapper();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus wrapperDeleteInstance(void** handle)
{
	delete (Wrapper*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus wrapperOpen(void* handle, MediaioPluginWriter* writer, void* writerHandle)
{
	Wrapper* instance = (Wrapper*) handle;
	return instance->open(writer, writerHandle);
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
	wrapperCreateInstance,
	wrapperDeleteInstance
};

static MediaioPluginWrapper Wrapper =
{
	wrapperOpen,
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
	"fr.co.mediaio:gpacwrapper",
	"GPAC Wrapper",
	"Write wrapping using GPAC library",
	1,
	0,
	pluginActionWrapper
);

std::vector<Plugin> plugins = {wrapperPlugin};
