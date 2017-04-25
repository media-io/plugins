
#include <mediaio/api/instance/instance.h>
#include "Wrapper.hpp"

#include <cstring>
#include <iostream>

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

static MediaioPluginInstance BentoTSWrapperInstance =
{
	createWrapperInstance,
	deleteWrapperInstance
};

static MediaioPluginWrapper BentoTSWrapper =
{
	wrapperSetWriter,
	wrapperConfigure,
	wrapperWrapNextFrame
};

////////////////////////////////////////////////////////////////////////////////
// The main function
static void* pluginActionWrapper(const char *action)
{
	if(!strcmp(action, kMediaioGetWrapperPlugin))
	{
		return &BentoTSWrapper;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &BentoTSWrapperInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct

static MediaioPlugin BentoTSWrapperPlugin = 
{
	kMediaioWrapperPluginApi,
	1,
	"fr.co.mediaio:bento_ts",
	"Mpeg2 TS muxer",
	"Mux transport streams using Bento library",
	1,
	0,
	pluginActionWrapper
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &BentoTSWrapperPlugin;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 1;
}

}
