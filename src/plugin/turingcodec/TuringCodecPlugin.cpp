
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
// Plugin definition
static void* pluginActionDecoder(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance: { return &DecoderInstance; }
		case PluginActionDecoder:  { return &Decoder; }
		default: return nullptr;
	}
}

static void* pluginActionEncoder(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance: { return &EncoderInstance; }
		case PluginActionEncoder:  { return &Encoder; }
		default: return nullptr;
	}
}

Plugin decoderPlugin = Plugin(
	PluginApiDecoder,
	"fr.co.mediaio:turingdecoder",
	"HEVC Decoder",
	"Decode HEVC using Turing Codec library",
	1,
	0,
	pluginActionDecoder
);

Plugin encoderPlugin = Plugin(
	PluginApiEncoder,
	"fr.co.mediaio:turingencoder",
	"HEVC Encoder",
	"Encode HEVC using Turing Codec library",
	1,
	0,
	pluginActionEncoder
);

std::vector<Plugin> plugins = {decoderPlugin, encoderPlugin};
