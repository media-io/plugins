
#include <mediaio/api/instance/instance.h>
#include "Reader.hpp"
#include "Writer.hpp"

#include <iostream>
#include <vector>

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


MediaioStatus createInstanceWriter(void** handle)
{
	Writer* instance = new Writer();
	*handle = instance;
	return kMediaioStatusOK;
}

MediaioStatus deleteInstanceWriter(void** handle)
{
	delete (Writer*)*handle;
	*handle = nullptr;
	return kMediaioStatusOK;
}

MediaioStatus setFilenameWriter(void* handle, const char* filename)
{
	Writer* instance = (Writer*)handle;
	return instance->set_filename(filename);
}

const char* getFilenameWriter(void* handle)
{
	Writer* instance = (Writer*) handle;
	return instance->get_filename();
}

MediaioStatus openWriter(void* handle)
{
	Writer* instance = (Writer*) handle;
	return instance->open();
}

MediaioStatus closeWriter(void* handle)
{
	Writer* instance = (Writer*) handle;
	return instance->close();
}

uint64_t getPositionWriter(void* handle)
{
	Writer* instance = (Writer*) handle;
	return instance->getPosition();
}

uint64_t writeWriter(void* handle, const char* data, const uint64_t size)
{
	Writer* instance = (Writer*) handle;
	return instance->write(data, size);
}

MediaioStatus seekWriter(void* handle, const int64_t size)
{
	Writer* instance = (Writer*) handle;
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

static MediaioPluginInstance WriterInstance =
{
	createInstanceWriter,
	deleteInstanceWriter
};

static MediaioPluginWriter Writer =
{
	setFilenameWriter,
	getFilenameWriter,
	openWriter,
	closeWriter,
	getPositionWriter,
	writeWriter,
	seekWriter,
};

////////////////////////////////////////////////////////////////////////////////
// The main function
static void* pluginActionReader(const char *action)
{
	if(!strcmp(action, kMediaioGetReaderPlugin))
	{
		return &Reader;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &ReaderInstance;
	}
	return nullptr;
}

static void* pluginActionWriter(const char *action)
{
	if(!strcmp(action, kMediaioGetWriterPlugin))
	{
		return &Writer;
	}
	if(!strcmp(action, kMediaioGetInstancePlugin))
	{
		return &WriterInstance;
	}
	return nullptr;
}

extern "C"
{

////////////////////////////////////////////////////////////////////////////////
// the plugin struct 
static MediaioPlugin FileSystemReader = 
{
	kMediaioReaderPluginApi,
	1,
	"fr.co.mediaio:filesystemreader",
	"File System Reader",
	"Read data from local filesystem",
	1,
	0,
	pluginActionReader
};

static MediaioPlugin FileSystemWriter = 
{
	kMediaioWriterPluginApi,
	1,
	"fr.co.mediaio:filesystemwriter",
	"File System Writer",
	"Write data to local filesystem",
	1,
	0,
	pluginActionWriter
};

MediaioPlugin* mediaio_get_plugin(int nth)
{
	if(nth == 0)
		return &FileSystemReader;
	if(nth == 1)
		return &FileSystemWriter;
	return 0;
}
 
int mediaio_get_number_of_plugins(void)
{
	return 2;
}

}
