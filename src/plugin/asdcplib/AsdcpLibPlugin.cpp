
#include <wrapper/Plugin.hpp>

#include "UnwrapperAsdcp.hpp"
#include "UnwrapperAs02.hpp"

MediaioStatus asdcpCreateInstance(void** handle)
{
	UnwrapperAsdcp* instance = new UnwrapperAsdcp();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus asdcpDeleteInstance(void** handle)
{
	delete (UnwrapperAsdcp*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus asdcpOpen(void* handle, MediaioPluginReader* reader, void* readerHandle)
{
	UnwrapperAsdcp* instance = (UnwrapperAsdcp*) handle;
	return instance->open(reader, readerHandle);
}

MediaioStatus asdcpConfigure(void* handle, const Metadata* parameters)
{
	UnwrapperAsdcp* instance = (UnwrapperAsdcp*) handle;
	return instance->configure(parameters);
}

MediaioStatus asdcpUnwrapNextFrame(void* handle, const int streamIndex, CodedData* unwrappedFrame)
{
	UnwrapperAsdcp* instance = (UnwrapperAsdcp*) handle;
	return instance->unwrapNextFrame(streamIndex, unwrappedFrame);
}

MediaioStatus asdcpSeekAtFrame(void* handle, const int frame)
{
	UnwrapperAsdcp* instance = (UnwrapperAsdcp*) handle;
	return instance->seekAtFrame(frame);
}

MediaioStatus asdcpSeekAtTime(void* handle, const double time)
{
	UnwrapperAsdcp* instance = (UnwrapperAsdcp*) handle;
	return instance->seekAtFrame(time);
}

MediaioStatus as02CreateInstance(void** handle)
{
	UnwrapperAs02* instance = new UnwrapperAs02();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus as02DeleteInstance(void** handle)
{
	delete (UnwrapperAs02*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus as02Open(void* handle, MediaioPluginReader* reader, void* writerHandle)
{
	UnwrapperAs02* instance = (UnwrapperAs02*) handle;
	return instance->open(reader, writerHandle);
}

MediaioStatus as02Configure(void* handle, const Metadata* parameters)
{
	UnwrapperAs02* instance = (UnwrapperAs02*) handle;
	return instance->configure(parameters);
}

MediaioStatus as02UnwrapNextFrame(void* handle, const int streamIndex, CodedData* unwrappedFrame)
{
	UnwrapperAs02* instance = (UnwrapperAs02*) handle;
	return instance->unwrapNextFrame(streamIndex, unwrappedFrame);
}

MediaioStatus as02SeekAtFrame(void* handle, const int frame)
{
	UnwrapperAs02* instance = (UnwrapperAs02*) handle;
	return instance->seekAtFrame(frame);
}

MediaioStatus as02SeekAtTime(void* handle, const double time)
{
	UnwrapperAs02* instance = (UnwrapperAs02*) handle;
	return instance->seekAtFrame(time);
}

static MediaioPluginInstance AsdcpUnwrapperInstance =
{
	asdcpCreateInstance,
	asdcpDeleteInstance
};

static MediaioPluginUnwrapper AsdcpUnwrapper =
{
	asdcpOpen,
	asdcpConfigure,
	asdcpUnwrapNextFrame,
	asdcpSeekAtFrame,
	asdcpSeekAtTime
};

static MediaioPluginInstance As02UnwrapperInstance =
{
	as02CreateInstance,
	as02DeleteInstance
};

static MediaioPluginUnwrapper As02Unwrapper =
{
	as02Open,
	as02Configure,
	as02UnwrapNextFrame,
	as02SeekAtFrame,
	as02SeekAtTime
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginActionAsdcp(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:  { return &AsdcpUnwrapperInstance; }
		case PluginActionUnwrapper: { return &AsdcpUnwrapper; }
		default: return nullptr;
	}
}

static void* pluginActionAs02(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:  { return &As02UnwrapperInstance; }
		case PluginActionUnwrapper: { return &As02Unwrapper; }
		default: return nullptr;
	}
}

Plugin asdcpPlugin = Plugin(
	PluginApiUnwrapper,
	"fr.co.mediaio:asdcplibunwrapper",
	"AsDcpLib Unwrapper",
	"Unwrapper for DCP essences",
	1,
	0,
	pluginActionGenerator
);


Plugin as02Plugin = Plugin(
	PluginApiUnwrapper,
	"fr.co.mediaio:as02libunwrapper",
	"As02Lib Unwrapper",
	"Unwrapper for AS-02 like essences",
	1,
	0,
	pluginActionGenerator
);

std::vector<Plugin> plugins = {asdcpPlugin, as02Plugin};
