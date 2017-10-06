#ifndef _MEDIAIO_PLUGIN_YADIF_FILTER_HPP_
#define _MEDIAIO_PLUGIN_YADIF_FILTER_HPP_

#include <mediaio/api/filter/filter.h>
#include <vector>
#include "yadifmod2.h"

class Filter
{
public:
	Filter();
	~Filter();

	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus process(const Frame* inputFrame, Frame* outputFrame);

private:
	interpolate_t _interp;
	proc_filter_t _mainProc;
	std::vector<const Frame*> _frames;
	bool          _init;
};

#endif
