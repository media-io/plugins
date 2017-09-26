
#include <wrapper/Plugin.hpp>
#include "Filter.hpp"

MediaioStatus createInstance(void** handle)
{
	Filter* instance = new Filter();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteInstance(void** handle)
{
	delete (Filter*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus configure(void* handle, const Metadata* parameters)
{
	Filter* instance = (Filter*) handle;
	return instance->configure(parameters);
}

MediaioStatus process(void* handle, const Frame* inputFrame, Frame* outputFrame)
{
	Filter* instance = (Filter*) handle;
	return instance->process(inputFrame, outputFrame);
}

static MediaioPluginInstance FilterInstance =
{
	createInstance,
	deleteInstance
};

static MediaioPluginFilter Filter =
{
	configure,
	process
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginAction(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance: { return &FilterInstance; }
		case PluginActionFilter:   { return &Filter; }
		default: return nullptr;
	}
}

Plugin plugin = Plugin(
	PluginApiFilter,
	"fr.co.mediaio:interlace",
	"Interlace",
	"Apply interlace filter",
	1,
	0,
	pluginAction
);

std::vector<Plugin> plugins = {plugin};
