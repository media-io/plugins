#ifndef _MEDIAIO_PLUGIN_COMPARATOR_ANALYSER_HPP_
#define _MEDIAIO_PLUGIN_COMPARATOR_ANALYSER_HPP_

#include <mediaio/api/analyser/analyser.h>
#include <vector>

class Analyser
{
public:
	Analyser();
	~Analyser();

	Metadata* analyse(const ImageFrame* decodedFrame, const int numberOfFrames);

protected:
	void computeSumOfDiff(
		const ImageFrame& refFrame,
		const ImageFrame& frame,
		std::vector< std::vector<double> >& sums,
		std::vector< std::vector<double> >& mins,
		std::vector< std::vector<double> >& maxs,
		std::vector< std::vector<double> >& meansOfSum);

private:
	std::vector<Metadata>                _results;
	std::vector< std::vector<Metadata> > _compareResult;
	std::vector< std::vector<double> >   _sums;
	std::vector< std::vector<double> >   _mins;
	std::vector< std::vector<double> >   _maxs;
	std::vector< std::vector<double> >   _meansOfSum;
};

#endif
