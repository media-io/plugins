
#include <mediaio/api/instance/instance.h>
#include "UnwrapperAsdcp.hpp"
#include "UnwrapperAs02.hpp"

#include <cstring>
#include <iostream>

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

static MediaioPluginInstance AsDcpLibUnwrapperInstance =
{
	asdcpCreateInstance,
	asdcpDeleteInstance
};

static MediaioPluginUnwrapper AsDcpLibUnwrapper =
{
	asdcpOpen,
	asdcpConfigure,
	asdcpUnwrapNextFrame,
	asdcpSeekAtFrame,
	asdcpSeekAtTime
};

static MediaioPluginInstance As02LibUnwrapperInstance =
{
	as02CreateInstance,
	as02DeleteInstance
};

static MediaioPluginUnwrapper As02LibUnwrapper =
{
	as02Open,
	as02Configure,
	as02UnwrapNextFrame,
	as02SeekAtFrame,
	as02SeekAtTime
};

////////////////////////////////////////////////////////////////////////////////
// The main function
static void* pluginAsdcpAction(const char *action)
{
	if(!strcmp(action, kMediaioGetUnwrapperPlugin))
	{
		return &AsDcpLibUnwrapper;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &AsDcpLibUnwrapperInstance;
	}
	return nullptr;
}

static void* pluginAs02Action(const char *action)
{
	if(!strcmp(action, kMediaioGetUnwrapperPlugin))
	{
		return &As02LibUnwrapper;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &As02LibUnwrapperInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct 
static MediaioPlugin AsDcpLibPlugin = 
{
	kMediaioUnwrapperPluginApi,
	1,
	"fr.co.mediaio:asdcplibunwrapper",
	"AsDcpLib Unwrapper",
	"Unwrapper for DCP essences",
	1,
	0,
	pluginAsdcpAction
};

static MediaioPlugin As02LibPlugin = 
{
	kMediaioUnwrapperPluginApi,
	1,
	"fr.co.mediaio:as02libunwrapper",
	"As02Lib Unwrapper",
	"Unwrapper for AS-02 like essences",
	1,
	0,
	pluginAs02Action
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &AsDcpLibPlugin;
	if(nth == 1)
		return &As02LibPlugin;
	return 0;
}

int mediaio_get_number_of_plugins(void)
{
	return 2;
}

}
