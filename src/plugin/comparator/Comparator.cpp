
#include <wrapper/Plugin.hpp>

#include "Analyser.hpp"

MediaioStatus createInstance(void** handle)
{
	Analyser* instance = new Analyser();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteInstance(void** handle)
{
	delete (Analyser*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

Metadata* analyse(void* handle, const ImageFrame* decodedFrame, const int numberOfFrames)
{
	Analyser* instance = (Analyser*) handle;
	// std::vector<Frame>& decodedFrames( decodedFrame, numberOfFrames );
	return instance->analyse(decodedFrame, numberOfFrames);
}

static MediaioPluginInstance AnalyserInstance =
{
	createInstance,
	deleteInstance
};

static MediaioPluginAnalyser Analyser =
{
	analyse
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginAction(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance: { return &AnalyserInstance; }
		case PluginActionAnalyser: { return &Analyser; }
		default: return nullptr;
	}
}

Plugin plugin = Plugin(
	PluginApiAnalyser,
	"fr.co.mediaio:comparatoranalyser",
	"Comparator",
	"Compare 2 images (mathematically)",
	1,
	0,
	pluginAction
);

std::vector<Plugin> plugins = {plugin};

