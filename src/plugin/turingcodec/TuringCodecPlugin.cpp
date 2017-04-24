
#include <mediaio/api/instance/instance.h>
#include "Decoder.hpp"
#include "Encoder.hpp"

#include <cstring>
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

static MediaioPluginInstance TuringCodecDecoderInstance =
{
	createDecoderInstance,
	deleteDecoderInstance
};

static MediaioPluginDecoder TuringCodecDecoder =
{
	configureDecoder,
	decode,
	getDecoderMetadatas
};

static MediaioPluginInstance TuringCodecEncoderInstance =
{
	createEncoderInstance,
	deleteEncoderInstance
};

static MediaioPluginEncoder TuringCodecEncoder =
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
		return &TuringCodecDecoder;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &TuringCodecDecoderInstance;
	}
	return nullptr;
}

static void* pluginActionEncoder(const char *action)
{
	if(!strcmp(action, kMediaioGetEncoderPlugin))
	{
		return &TuringCodecEncoder;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &TuringCodecEncoderInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct

static MediaioPlugin TuringCodecDecoderPlugin = 
{
	kMediaioDecoderPluginApi,
	1,
	"fr.co.mediaio:turingdecoder",
	"HEVC Decoder",
	"Decode HEVC using Turing Codec library",
	1,
	0,
	pluginActionDecoder
};

static MediaioPlugin TuringCodecEncoderPlugin = 
{
	kMediaioEncoderPluginApi,
	1,
	"fr.co.mediaio:turingencoder",
	"HEVC Encoder",
	"Encode HEVC using Turing Codec library",
	1,
	0,
	pluginActionEncoder
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &TuringCodecDecoderPlugin;
	if(nth == 1)
		return &TuringCodecEncoderPlugin;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 2;
}

}
