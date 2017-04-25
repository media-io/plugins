
#include "Generator.hpp"

#include <mediaio/api/instance/instance.h>

#include <vector>

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

MediaioStatus generate(void* handle, Frame* frame)
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
// The main function
static void* pluginActionGenerator( const char *action )
{
	if( ! strcmp( action, kMediaioGetInstancePlugin ) )
	{
		return &GeneratorInstance;
	}
	if( ! strcmp( action, kMediaioGetGeneratorPlugin ) )
	{
		return &Generator;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct 
static MediaioPlugin SeExpr = 
{
	kMediaioGeneratorPluginApi,
	1,
	"fr.co.mediaio:seexpr",
	"SeExpr",
	"Generator based on SeExpr library",
	1,
	0,
	pluginActionGenerator
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &SeExpr;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 1;
}

}