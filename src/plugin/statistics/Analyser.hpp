#ifndef _MEDIAIO_PLUGIN_STATISTICS_ANALYSER_HPP_
#define _MEDIAIO_PLUGIN_STATISTICS_ANALYSER_HPP_

#include <mediaio/api/analyser/analyser.h>

#include <vector>

class Analyser
{
public:
	Analyser();
	~Analyser();

	Metadata* analyse(const ImageFrame* decodedFrame, const int numberOfFrames);

protected:
	void analyseFrame(
		const ImageFrame& frame,
		std::vector<int>& mins,
		std::vector<int>& maxs,
		std::vector<double>& means
	);

private:
	std::vector<Metadata>                 _results;
	std::vector< std::vector<Metadata> >  _analysis;
	std::vector< std::vector<int > >      _min;
	std::vector< std::vector<int > >      _max;
	std::vector< std::vector<double > >   _mean;
};

#endif
