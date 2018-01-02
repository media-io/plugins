
#include <wrapper/Plugin.hpp>

#include "Generator.hpp"

MediaioStatus createInstance(void** handle)
{
	Generator* instance = new Generator();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteInstance(void** handle)
{
	delete (Generator*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus configure(void* handle, const Metadata* parameters)
{
	Generator* instance = (Generator*) handle;
	return instance->configure(parameters);
}

MediaioStatus generate(void* handle, ImageFrame* frame)
{
	Generator* instance = (Generator*) handle;
	instance->generate(frame);
	return kMediaioStatusOK;
}

static MediaioPluginInstance GeneratorInstance =
{
	createInstance,
	deleteInstance
};

static MediaioPluginGenerator Generator =
{
	configure,
	generate
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginActionGenerator(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance:  { return &GeneratorInstance; }
		case PluginActionGenerator: { return &Generator; }
		default: return nullptr;
	}
}

Plugin plugin = Plugin(
	PluginApiGenerator,
	"fr.co.mediaio:seexpr",
	"SeExpr",
	"Generator based on SeExpr library",
	1,
	0,
	pluginActionGenerator
);

std::vector<Plugin> plugins = {plugin};
