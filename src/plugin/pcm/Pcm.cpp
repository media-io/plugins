
#include <wrapper/Plugin.hpp>

#include "Decoder.hpp"

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

MediaioStatus decode(void* handle, CodedData* unwrappedFrame, AudioFrame* decodedFrame)
{
	Decoder* instance = (Decoder*) handle;
	return instance->decode(unwrappedFrame, decodedFrame);
}

Metadata* getMetadatas(void* handle)
{
	Decoder* instance = (Decoder*) handle;
	return instance->getMetadatas();
}

static MediaioPluginInstance DecoderInstance =
{
	createInstance,
	deleteInstance
};

static MediaioPluginAudioDecoder Decoder =
{
	configure,
	decode,
	getMetadatas
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginActionDecoder(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:     { return &DecoderInstance; }
		case PluginActionAudioDecoder: { return &Decoder; }
		default: return nullptr;
	}
}

Plugin decoderPlugin = Plugin(
	PluginApiAudioDecoder,
	"fr.co.mediaio:pcmdecoder",
	"PCM Decoder",
	"Decode PCM audio",
	1,
	0,
	pluginActionDecoder
);

std::vector<Plugin> plugins = {decoderPlugin};
