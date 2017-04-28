
#include <wrapper/Plugin.hpp>

#include "Unwrapper.hpp"
#include "Wrapper.hpp"

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

static MediaioPluginInstance UnwrapperInstance =
{
	unwrapperCreateInstance,
	unwrapperDeleteInstance
};

static MediaioPluginUnwrapper Unwrapper =
{
	unwrapperOpen,
	unwrapperConfigure,
	unwrapperUnwrapNextFrame,
	unwrapperSeekAtFrame,
	unwrapperSeekAtTime
};

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
static void* pluginActionUnwrapper(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:  { return &UnwrapperInstance; }
		case PluginActionUnwrapper: { return &Unwrapper; }
		default: return nullptr;
	}
}

static void* pluginActionWrapper(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:  { return &WrapperInstance; }
		case PluginActionWrapper: { return &Wrapper; }
		default: return nullptr;
	}
}

Plugin unwrapperPlugin = Plugin(
	PluginApiUnwrapper,
	"fr.co.mediaio:sequenceunwrapper",
	"Sequence Unwrapper",
	"Read image sequences of any type",
	1,
	0,
	pluginActionUnwrapper
);


Plugin wrapperPlugin = Plugin(
	PluginApiWrapper,
	"fr.co.mediaio:sequencewrapper",
	"Sequence Wrapper",
	"Write image sequences of any type",
	1,
	0,
	pluginActionWrapper
);

std::vector<Plugin> plugins = {wrapperPlugin, unwrapperPlugin};
