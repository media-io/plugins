#ifndef _MEDIAIO_PLUGIN_DEINTERLACE_FILTER_HPP_
#define _MEDIAIO_PLUGIN_DEINTERLACE_FILTER_HPP_

#include <mediaio/api/filter/filter.h>

class Filter
{
public:
	Filter();
	~Filter();

	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus process(const ImageFrame* inputFrame, ImageFrame* outputFrame);

private:
};

#endif
