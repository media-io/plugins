
#include <mediaio/api/instance/instance.h>
#include "Analyser.hpp"

#include <iostream>
#include <vector>

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

Metadata* analyse(void* handle, const Frame* decodedFrame, const int numberOfFrames)
{
	Analyser* instance = (Analyser*) handle;
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
// The main function
static void* pluginActionAnalyser(const char *action)
{
	if(!strcmp(action, kMediaioGetAnalyserPlugin))
	{
		return &Analyser;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &AnalyserInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct 
static MediaioPlugin StatisticsAnalyser = 
{
	kMediaioAnalyserPluginApi,
	1,
	"fr.co.mediaio:statisticsanalyser",
	"Statistics Analyser",
	"Process some statistical analysis on image",
	1,
	0,
	pluginActionAnalyser
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &StatisticsAnalyser;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 1;
}

}
