
#include <wrapper/Plugin.hpp>

#include "Unwrapper.hpp"
#include "Decoder.hpp"
#include "Encoder.hpp"

MediaioStatus ffmpegUnwrapperCreateInstance(void** handle)
{
	Unwrapper* instance = new Unwrapper();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus ffmpegUnwrapperDeleteInstance(void** handle)
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

static MediaioPluginInstance UnwrapperInstance =
{
	ffmpegUnwrapperCreateInstance,
	ffmpegUnwrapperDeleteInstance
};

static MediaioPluginUnwrapper Unwrapper =
{
	unwrapperOpen,
	unwrapperConfigure,
	unwrapperUnwrapNextFrame,
	unwrapperSeekAtFrame,
	unwrapperSeekAtTime
};

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
static void* pluginActionUnwrapper(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance: { return &UnwrapperInstance; }
		case PluginActionUnwrapper:  { return &Unwrapper; }
		default: return nullptr;
	}
}

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

Plugin unwrapperPlugin = Plugin(
	PluginApiUnwrapper,
	"fr.co.mediaio:ffmpegunwrapper",
	"FFmpeg unwrapper",
	"Unwrap streams based on FFmpeg library",
	1,
	0,
	pluginActionUnwrapper
);

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

std::vector<Plugin> plugins = {
	unwrapperPlugin,
	decoderPlugin,
	encoderPlugin
};
