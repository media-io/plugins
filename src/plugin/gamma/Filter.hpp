#ifndef _MEDIAIO_PLUGIN_GAMMA_FILTER_HPP_
#define _MEDIAIO_PLUGIN_GAMMA_FILTER_HPP_

#include <mediaio/api/filter/filter.h>

#include <Halide.h>

#include <vector>

class Filter
{
public:
	Filter();
	~Filter();

	MediaioStatus configure( const Metadata* parameters );

	MediaioStatus process( const Frame* inputFrame, Frame* outputFrame );

private:
	Halide::Target _target;
	Halide::Var    _i;
	Halide::Var    _x;
	Halide::Var    _y;
	Halide::Var    _c;
};

#endif
