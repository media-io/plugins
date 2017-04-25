
#include "Filter.hpp"

#include <mediaio/api/instance/instance.h>

// #include <iostream>
// #include <vector>

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
	return instance->configure( parameters );
}

MediaioStatus process(void* handle, const Frame* inputFrame, Frame* outputFrame )
{
	Filter* instance = (Filter*) handle;
	return instance->process( inputFrame, outputFrame );
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
// The main function
static void* pluginActionFilter( const char *action )
{
	if( ! strcmp( action, kMediaioGetFilterPlugin ) )
	{
		return &Filter;
	}
	if( ! strcmp( action, kMediaioGetInstancePlugin ) )
	{
		return &FilterInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct 
static MediaioPlugin Math = 
{
	kMediaioFilterPluginApi,
	1,
	"fr.co.mediaio:math",
	"Math",
	"Apply math operator on a image (plus, minus, multiply, division)",
	1,
	0,
	pluginActionFilter
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &Math;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 1;
}

}
