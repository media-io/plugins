#ifndef _MEDIAIO_PLUGIN_SEEXPR_GENERATOR_HPP_
#define _MEDIAIO_PLUGIN_SEEXPR_GENERATOR_HPP_

#include <mediaio/api/generator/generator.h>

#include <vector>

class Generator
{
public:
	Generator();
	~Generator();

	MediaioStatus configure(const Metadata* parameters);

	MediaioStatus generate(Frame* frame);

private:
	size_t _width;
	size_t _height;
	size_t _precision;
	size_t _components;
};

#endif
