
#include <mediaio/api/instance/instance.h>
#include "Decoder.hpp"

#include <cstring>

MediaioStatus createInstance(void** handle)
{
	Decoder* instance = new Decoder();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteInstance(void** handle)
{
	delete (Decoder*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus configure(void* handle, const Metadata* parameters)
{
	Decoder* instance = (Decoder*) handle;
	return instance->configure(parameters);
}

MediaioStatus decode(void* handle, CodedData* unwrappedFrame, Frame* decodedFrame)
{
	Decoder* instance = (Decoder*) handle;
	return instance->decode(unwrappedFrame, decodedFrame);
}

Metadata* getMetadatas(void* handle)
{
	Decoder* instance = (Decoder*) handle;
	return instance->getMetadatas();
}

static MediaioPluginInstance OpenJpegDecoderInstance =
{
	createInstance,
	deleteInstance
};

static MediaioPluginDecoder OpenJpegDecoder =
{
	configure,
	decode,
	getMetadatas
};

////////////////////////////////////////////////////////////////////////////////
// The main function
static void* pluginActionDecoder(const char *action)
{
	if(!strcmp(action, kMediaioGetDecoderPlugin))
	{
		return &OpenJpegDecoder;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &OpenJpegDecoderInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct

static MediaioPlugin OpenJpegDecoderPlugin = 
{
	kMediaioDecoderPluginApi,
	1,
	"fr.co.mediaio:openjpegdecoder",
	"OpenJpeg Decoder",
	"Decoder Jpeg2000 using OpenJpeg library",
	1,
	0,
	pluginActionDecoder
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &OpenJpegDecoderPlugin;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 1;
}

}
