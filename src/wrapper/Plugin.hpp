
#include <mediaio/api/instance/instance.h>
#include <mediaio/api/generator/generator.h>
#include <mediaio/api/reader/reader.h>
#include <mediaio/api/writer/writer.h>
#include <mediaio/api/unwrapper/unwrapper.h>
#include <mediaio/api/wrapper/wrapper.h>
#include <mediaio/api/decoder/decoder.h>
#include <mediaio/api/encoder/encoder.h>
#include <mediaio/api/filter/filter.h>
#include <mediaio/api/analyser/analyser.h>

#include <cstring>
#include <vector>

class Plugin
{
	int get_plugin_api_version(const MediaioPluginApi api)
	{
		return 1;
	}
public:

	Plugin(
		const MediaioPluginApi api,
		const char* identifier,
		const char* label,
		const char* description,
		const unsigned int major_version,
		const unsigned int minor_version,
		PluginActionPoint* action)
		: _definition({
			get_api_str(api),
			get_plugin_api_version(api),
			identifier,
			label,
			description,
			major_version,
			minor_version,
			action
		})
	{}

	MediaioPlugin* get_mediaio_plugin(){
		return &_definition;
	}

private:
	MediaioPlugin           _definition;
};

extern std::vector<Plugin> plugins;

extern "C"
{

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth > plugins.size())
	{
		return 0;
	}
	return plugins.at(nth).get_mediaio_plugin();
}

int mediaio_get_number_of_plugins(void)
{
	return plugins.size();
}

}
