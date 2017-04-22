
#include <mediaio/api/instance/instance.h>
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

static MediaioPluginInstance SequenceUnwrapperInstance =
{
	unwrapperCreateInstance,
	unwrapperDeleteInstance
};

static MediaioPluginUnwrapper SequenceUnwrapper =
{
	unwrapperOpen,
	unwrapperConfigure,
	unwrapperUnwrapNextFrame,
	unwrapperSeekAtFrame,
	unwrapperSeekAtTime
};

static MediaioPluginInstance SequenceWrapperInstance =
{
	wrapperCreateInstance,
	wrapperDeleteInstance
};

static MediaioPluginWrapper SequenceWrapper =
{
	wrapperOpen,
	wrapperConfigure,
	wrapperWrapNextFrame
};

////////////////////////////////////////////////////////////////////////////////
// The main function
static void* pluginActionUnwrapper( const char *action )
{
	if( ! strcmp( action, kMediaioGetUnwrapperPlugin ) )
	{
		return &SequenceUnwrapper;
	}
	if( ! strcmp( action, kMediaioGetInstancePlugin ) )
	{
		return &SequenceUnwrapperInstance;
	}
	return nullptr;
}

static void* pluginActionWrapper( const char *action )
{
	if( ! strcmp( action, kMediaioGetWrapperPlugin ) )
	{
		return &SequenceWrapper;
	}
	if( ! strcmp( action, kMediaioGetInstancePlugin ) )
	{
		return &SequenceWrapperInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct

static MediaioPlugin SequenceUnwrapperPlugin = 
{
	kMediaioUnwrapperPluginApi,
	1,
	"fr.co.mediaio:sequenceunwrapper",
	"Sequence Unwrapper",
	"Read image sequences of any type",
	1,
	0,
	pluginActionUnwrapper
};

static MediaioPlugin SequenceWrapperPlugin = 
{
	kMediaioWrapperPluginApi,
	1,
	"fr.co.mediaio:sequencewrapper",
	"Sequence Wrapper",
	"Write image sequences of any type",
	1,
	0,
	pluginActionWrapper
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &SequenceUnwrapperPlugin;
	if(nth == 1)
		return &SequenceWrapperPlugin;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 2;
}

}
