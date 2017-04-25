
#include <mediaio/api/instance/instance.h>
#include "Decoder.hpp"
#include "Encoder.hpp"

#include <iostream>

MediaioStatus createDecoderInstance(void** handle)
{
	Decoder* instance = new Decoder();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteDecoderInstance(void** handle)
{
	delete (Decoder*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus configureDecoder(void* handle, const Metadata* parameters)
{
	Decoder* instance = (Decoder*) handle;
	return instance->configure( parameters );
}

MediaioStatus decode(void* handle, CodedData* unwrappedFrame, Frame* decodedFrame)
{
	Decoder* instance = (Decoder*) handle;
	return instance->decode(unwrappedFrame, decodedFrame);
}

Metadata* getDecoderMetadatas(void* handle)
{
	Decoder* instance = (Decoder*) handle;
	return instance->getMetadatas();
}

MediaioStatus createEncoderInstance(void** handle)
{
	Encoder* instance = new Encoder();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteEncoderInstance(void** handle)
{
	delete (Encoder*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus configureEncoder(void* handle, const Metadata* parameters)
{
	Encoder* instance = (Encoder*) handle;
	return instance->configure( parameters );
}

MediaioStatus encode(void* handle, Frame* decodedFrame, CodedData* unwrappedFrame)
{
	Encoder* instance = (Encoder*) handle;
	return instance->encode(decodedFrame, unwrappedFrame);
}

Metadata* getEncoderMetadatas(void* handle)
{
	Encoder* instance = (Encoder*) handle;
	return instance->getMetadatas();
}

static MediaioPluginInstance TiffDecoderInstance =
{
	createDecoderInstance,
	deleteDecoderInstance
};

static MediaioPluginDecoder TiffDecoder =
{
	configureDecoder,
	decode,
	getDecoderMetadatas
};

static MediaioPluginInstance TiffEncoderInstance =
{
	createEncoderInstance,
	deleteEncoderInstance
};

static MediaioPluginEncoder TiffEncoder =
{
	configureEncoder,
	encode,
	getEncoderMetadatas
};

////////////////////////////////////////////////////////////////////////////////
// The main function
static void* pluginActionDecoder(const char *action)
{
	if(!strcmp(action, kMediaioGetDecoderPlugin))
	{
		return &TiffDecoder;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &TiffDecoderInstance;
	}
	return nullptr;
}

static void* pluginActionEncoder(const char *action)
{
	if(!strcmp(action, kMediaioGetEncoderPlugin))
	{
		return &TiffEncoder;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &TiffEncoderInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct

static MediaioPlugin TiffDecoderPlugin = 
{
	kMediaioDecoderPluginApi,
	1,
	"fr.co.mediaio:tiffdecoder",
	"Tiff Decoder",
	"Decode TIFF image format",
	1,
	0,
	pluginActionDecoder
};

static MediaioPlugin TiffEncoderPlugin = 
{
	kMediaioEncoderPluginApi,
	1,
	"fr.co.mediaio:tiffencoder",
	"Tiff Encoder",
	"Encode TIFF image format",
	1,
	0,
	pluginActionEncoder
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &TiffDecoderPlugin;
	if(nth == 1)
		return &TiffEncoderPlugin;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 2;
}

}
