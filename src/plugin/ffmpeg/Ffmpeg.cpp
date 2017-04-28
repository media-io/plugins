
#include <mediaio/api/instance/instance.h>
#include "Decoder.hpp"
#include "Encoder.hpp"

#include <cstring>

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

static MediaioPluginInstance FfmpegDecoderInstance =
{
	ffmpegDecoderCreateInstance,
	ffmpegDecoderDeleteInstance
};

static MediaioPluginDecoder FfmpegDecoder =
{
};

static MediaioPluginInstance FfmpegEncoderInstance =
{
	ffmpegEncoderCreateInstance,
	ffmpegEncoderDeleteInstance
};

static MediaioPluginEncoder FfmpegEncoder =
{
};

////////////////////////////////////////////////////////////////////////////////
// The main function
static void* pluginFfmpegDecoderAction(const char *action)
{
	if(!strcmp(action, kMediaioGetDecoderPlugin))
	{
		return &FfmpegDecoder;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &FfmpegDecoderInstance;
	}
	return nullptr;
}

static void* pluginFfmpegEncoderAction(const char *action)
{
	if(!strcmp(action, kMediaioGetEncoderPlugin))
	{
		return &FfmpegEncoder;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &FfmpegEncoderInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct 
static MediaioPlugin FfmpegDecoderPlugin = 
{
	kMediaioDecoderPluginApi,
	1,
	"fr.co.mediaio:ffmpegdecoder",
	"FFmpeg decoder",
	"Decoder based on FFmpeg library",
	1,
	0,
	pluginFfmpegDecoderAction
};

static MediaioPlugin FfmpegEncoderPlugin = 
{
	kMediaioEncoderPluginApi,
	1,
	"fr.co.mediaio:ffmpegencoder",
	"FFmpeg encoder",
	"Encoder based on FFmpeg library",
	1,
	0,
	pluginFfmpegEncoderAction
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &FfmpegDecoderPlugin;
	if(nth == 1)
		return &FfmpegEncoderPlugin;
	return 0;
}

int mediaio_get_number_of_plugins(void)
{
	return 2;
}

}
