
#include <wrapper/Plugin.hpp>
#include "Decoder.hpp"
#include "Encoder.hpp"

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

MediaioStatus decode(void* handle, CodedData* unwrappedFrame, ImageFrame* decodedFrame)
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

MediaioStatus encode(void* handle, ImageFrame* decodedFrame, CodedData* unwrappedFrame)
{
	Encoder* instance = (Encoder*) handle;
	return instance->encode(decodedFrame, unwrappedFrame);
}

Metadata* getEncoderMetadatas(void* handle)
{
	Encoder* instance = (Encoder*) handle;
	return instance->getMetadatas();
}

static MediaioPluginInstance DecoderInstance =
{
	createDecoderInstance,
	deleteDecoderInstance
};

static MediaioPluginImageDecoder Decoder =
{
	configureDecoder,
	decode,
	getDecoderMetadatas
};

static MediaioPluginInstance EncoderInstance =
{
	createEncoderInstance,
	deleteEncoderInstance
};

static MediaioPluginImageEncoder Encoder =
{
	configureEncoder,
	encode,
	getEncoderMetadatas
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginActionDecoder(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:     { return &DecoderInstance; }
		case PluginActionImageDecoder: { return &Decoder; }
		default: return nullptr;
	}
}

static void* pluginActionEncoder(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:     { return &EncoderInstance; }
		case PluginActionImageEncoder: { return &Encoder; }
		default: return nullptr;
	}
}

Plugin decoderPlugin = Plugin(
	PluginApiImageDecoder,
	"fr.co.mediaio:tiffdecoder",
	"Tiff Decoder",
	"Decode TIFF image format",
	1,
	0,
	pluginActionDecoder
);

Plugin encoderPlugin = Plugin(
	PluginApiImageEncoder,
	"fr.co.mediaio:tiffencoder",
	"Tiff Encoder",
	"Encode TIFF image format",
	1,
	0,
	pluginActionEncoder
);

std::vector<Plugin> plugins = {decoderPlugin, encoderPlugin};
