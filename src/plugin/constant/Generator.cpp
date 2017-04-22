#include "Generator.hpp"

#include <iostream>

Generator::Generator()
	: _width(1920)
	, _height(1080)
	, _precision(16)
	, _components(3)
{
}

Generator::~Generator()
{
}

MediaioStatus Generator::configure(const Metadata* parameters)
{
	size_t index = 0;
	while( 1 )
	{
		const Metadata& m = parameters[index];
		if(m.type == eMetadataTypeEnd)
		{
			break;
		}
		// std::cout << m.id << std::endl;
		if((!strcmp(m.id, "width")) &&
			(m.type == eMetadataTypeString))
		{
			_width = atoi((const char*) m.data);
		}
		if((!strcmp( m.id, "height")) &&
			(m.type == eMetadataTypeString))
		{
			_height = atoi((const char*) m.data);
		}
		++index;
	}
	return kMediaioStatusOK;
}

MediaioStatus Generator::generate(Frame* frame)
{
	init_frame(frame);
	create_components(frame, _components);

	for (int c = 0; c < _components; ++c)
	{
		Component& component = frame->components[c];
		init_component(&component);
		component.width = _width;
		component.height = _height;
		component.precision = _precision;
		component.sampleSizeInByte = _precision / 8;

		resize_component(&component, _width * _height * _precision / 8);

		uint16_t* ptr = (uint16_t*)component.data;


		// memset((uint8_t*)component.data, 0xFF, component.size);

		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x)
			{
				// *ptr = 0xFFFF;
				*ptr = c * 0;
				++ptr;
			}
		}
	}
	return kMediaioStatusOK;
}
