#ifndef _MEDIAIO_PLUGIN_TEXT_FILTER_HPP_
#define _MEDIAIO_PLUGIN_TEXT_FILTER_HPP_

#include <mediaio/api/filter/filter.h>

#include <string>
#include <vector>

class Filter
{
public:
	Filter();
	~Filter();

	MediaioStatus configure( const Metadata* parameters );

	MediaioStatus process( const Frame* inputFrame, Frame* outputFrame );

private:
	void plot_text_utf8(
		Frame* outputFrame,
		std::string face_path,
		int fontsize,
		int x_start,
		int y_start,
		double angle,
		std::string text,
		double red,
		double green,
		double blue
	);

	std::string parse_string(std::string text);

	std::string _text;
	size_t      _size;
	int         _offset_x;
	int         _offset_y;
	float       _red;
	float       _green;
	float       _blue;
	float       _timebase;
	size_t      _current_frame;
};

#endif
