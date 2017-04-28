
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

static MediaioPluginInstance DecoderInstance =
{
	createInstance,
	deleteInstance
};

static MediaioPluginDecoder Decoder =
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
		case PluginActionInstance: { return &DecoderInstance; }
		case PluginActionDecoder:  { return &Decoder; }
		default: return nullptr;
	}
}

Plugin decoderPlugin = Plugin(
	PluginApiDecoder,
	"fr.co.mediaio:openjpegdecoder",
	"OpenJpeg Decoder",
	"Decoder Jpeg2000 using OpenJpeg library",
	1,
	0,
	pluginActionDecoder
);

std::vector<Plugin> plugins = {decoderPlugin};
