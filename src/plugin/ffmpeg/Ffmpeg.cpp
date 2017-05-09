
#include <wrapper/Plugin.hpp>

#include "Decoder.hpp"
#include "Encoder.hpp"

MediaioStatus ffmpegDecoderCreateInstance(void** handle)
{
	Decoder* instance = new Decoder();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus ffmpegDecoderDeleteInstance(void** handle)
{
	delete (Decoder*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus decoderConfigure(void* handle, const Metadata* parameters)
{
	Decoder* instance = (Decoder*) handle;
	return instance->configure(parameters);
}

MediaioStatus decoderDecode(void* handle, CodedData* unwrappedFrame, Frame* decodedFrame)
{
	Decoder* instance = (Decoder*) handle;
	return instance->decode(unwrappedFrame, decodedFrame);
}

Metadata* decoderGetMetadatas(void* handle)
{
	Decoder* instance = (Decoder*) handle;
	return instance->getMetadatas();
}

MediaioStatus ffmpegEncoderCreateInstance(void** handle)
{
	Encoder* instance = new Encoder();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus ffmpegEncoderDeleteInstance(void** handle)
{
	delete (Encoder*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

static MediaioPluginInstance DecoderInstance =
{
	ffmpegDecoderCreateInstance,
	ffmpegDecoderDeleteInstance
};

static MediaioPluginDecoder Decoder =
{
	decoderConfigure,
	decoderDecode,
	decoderGetMetadatas
};

static MediaioPluginInstance EncoderInstance =
{
	ffmpegEncoderCreateInstance,
	ffmpegEncoderDeleteInstance
};

static MediaioPluginEncoder Encoder =
{
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
	"fr.co.mediaio:ffmpegdecoder",
	"FFmpeg decoder",
	"Decoder based on FFmpeg library",
	1,
	0,
	pluginActionDecoder
);

Plugin encoderPlugin = Plugin(
	PluginApiEncoder,
	"fr.co.mediaio:ffmpegencoder",
	"FFmpeg encoder",
	"Encoder based on FFmpeg library",
	1,
	0,
	pluginActionEncoder
);

std::vector<Plugin> plugins = {decoderPlugin, encoderPlugin};
