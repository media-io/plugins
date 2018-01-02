
#include <wrapper/Plugin.hpp>

#include "Reader.hpp"

MediaioStatus createInstanceReader(void** handle)
{
	Reader* instance = new Reader();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteInstanceReader(void** handle)
{
	delete (Reader*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus setFilenameReader(void* handle, const char* filename)
{
	Reader* instance = (Reader*)handle;
	return instance->set_filename(filename);
}

const char* getFilenameReader(void* handle)
{
	Reader* instance = (Reader*) handle;
	return instance->get_filename();
}

MediaioStatus openReader(void* handle)
{
	Reader* instance = (Reader*) handle;
	return instance->open();
}

MediaioStatus closeReader(void* handle)
{
	Reader* instance = (Reader*) handle;
	return instance->close();
}

int64_t getSizeReader(void* handle)
{
	Reader* instance = (Reader*) handle;
	return instance->getSize();
}

MediaioStatus setCacheSizeReader(void* handle, const uint64_t size)
{
	Reader* instance = (Reader*) handle;
	return instance->setCacheSize(size);
}

uint64_t getCacheSizeReader(void* handle)
{
	Reader* instance = (Reader*) handle;
	return instance->getCacheSize();
}

uint64_t getPositionReader(void* handle)
{
	Reader* instance = (Reader*) handle;
	return instance->getPosition();
}

uint64_t readReader(void* handle, char* data, const uint64_t size)
{
	Reader* instance = (Reader*) handle;
	return instance->read(data, size);
}

MediaioStatus seekReader(void* handle, const int64_t size)
{
	Reader* instance = (Reader*) handle;
	return instance->seek(size);
}

static MediaioPluginInstance ReaderInstance =
{
	createInstanceReader,
	deleteInstanceReader
};

static MediaioPluginReader Reader =
{
	setFilenameReader,
	getFilenameReader,
	openReader,
	closeReader,
	getSizeReader,
	setCacheSizeReader,
	getCacheSizeReader,
	getPositionReader,
	readReader,
	seekReader,
};

////////////////////////////////////////////////////////////////////////////////
// Plugin definition
static void* pluginActionReader(const char *action)
{
	switch(get_action(action))
	{
		case PluginActionInstance: { return &ReaderInstance; }
		case PluginActionReader:   { return &Reader; }
		default: return nullptr;
	}
}

Plugin readerPlugin = Plugin(
	PluginApiReader,
	"fr.co.mediaio:httpreader",
	"HTTP Reader",
	"Read data from HTTP access",
	1,
	0,
	pluginActionReader
);

std::vector<Plugin> plugins = {readerPlugin};
