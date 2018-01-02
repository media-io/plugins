#include "Analyser.hpp"

#include <iostream>
#include <algorithm>

Analyser::Analyser()
{

}

Analyser::~Analyser()
{

}

Metadata* Analyser::analyse(const ImageFrame* decodedFrame, const int numberOfFrames)
{
	_results.clear();
	_sums.clear();
	_mins.clear();
	_maxs.clear();
	_meansOfSum.clear();


	Metadata mEnd;
	init_metadata(&mEnd);
	mEnd.type = eMetadataTypeEnd;

	for(int imageIndex = 1; imageIndex < numberOfFrames; ++imageIndex)
	{
		computeSumOfDiff(
			decodedFrame[0],
			decodedFrame[imageIndex],
			_sums,
			_mins,
			_maxs,
			_meansOfSum
		);
	}

	_compareResult.resize( numberOfFrames - 1 );

	for( int imageIndex = 0; imageIndex < numberOfFrames - 1; ++imageIndex )
	{
		_compareResult.at(imageIndex).resize( _sums[imageIndex].size() * 4 );
		for( size_t resultIndex = 0; resultIndex < _sums[imageIndex].size(); ++resultIndex )
		{
			Metadata resultSumAbsDiff;
			init_metadata(&resultSumAbsDiff);
			resultSumAbsDiff.id = "sumofabsdiff";
			resultSumAbsDiff.label = "Sum of absolute difference between images";
			resultSumAbsDiff.type = eMetadataTypeDouble;
			resultSumAbsDiff.data = &_sums[imageIndex][resultIndex];
			_compareResult.at(imageIndex).at( resultIndex * 4 + 0 ) = resultSumAbsDiff;

			Metadata resultMeanSumAbsDiff;
			init_metadata(&resultMeanSumAbsDiff);
			resultMeanSumAbsDiff.id = "meansumofabsdiff";
			resultMeanSumAbsDiff.label = "Mean of the sum of absolute difference between images";
			resultMeanSumAbsDiff.type = eMetadataTypeDouble;
			resultMeanSumAbsDiff.data = &_meansOfSum[imageIndex][resultIndex];
			_compareResult.at(imageIndex).at( resultIndex * 4 + 1 ) = resultMeanSumAbsDiff;

			Metadata resultMinSumAbsDiff;
			init_metadata(&resultMinSumAbsDiff);
			resultMinSumAbsDiff.id = "minsumofabsdiff";
			resultMinSumAbsDiff.label = "Minimum of the sum of absolute difference between images";
			resultMinSumAbsDiff.type = eMetadataTypeDouble;
			resultMinSumAbsDiff.data = &_mins[imageIndex][resultIndex];
			_compareResult.at(imageIndex).at( resultIndex * 4 + 2 ) = resultMinSumAbsDiff;

			Metadata resultMaxSumAbsDiff;
			init_metadata(&resultMaxSumAbsDiff);
			resultMaxSumAbsDiff.id = "maxsumofabsdiff";
			resultMaxSumAbsDiff.label = "Maximum of the sum of absolute difference between images";
			resultMaxSumAbsDiff.type = eMetadataTypeDouble;
			resultMaxSumAbsDiff.data = &_maxs[imageIndex][resultIndex];
			_compareResult.at(imageIndex).at( resultIndex * 4 + 3 ) = resultMaxSumAbsDiff;
		}
		_compareResult.at(imageIndex).push_back( mEnd );

		Metadata resultGroup;
		init_metadata(&resultGroup);
		resultGroup.id = "comparator";
		resultGroup.label = "Comparison between stream 0 and stream ";
		resultGroup.type = eMetadataTypeGroup;
		resultGroup.data = &_compareResult.at(imageIndex)[0];
		_results.push_back( resultGroup );
	}

	_results.push_back(mEnd);
	return &_results[0];
}

void Analyser::computeSumOfDiff(
		const ImageFrame& refFrame,
		const ImageFrame& frame,
		std::vector< std::vector<double> >& sums,
		std::vector< std::vector<double> >& mins,
		std::vector< std::vector<double> >& maxs,
		std::vector< std::vector<double> >& meansOfSum )
{
	size_t components = frame.numberOfComponents;
	std::vector<double> resultSums;
	std::vector<double> resultMins;
	std::vector<double> resultMaxs;
	std::vector<double> resultMeansOfSum;

	for( size_t componentIndex = 0; componentIndex < components; ++componentIndex )
	{
		Component& refComponent = refFrame.components[componentIndex];
		Component& component = frame.components[componentIndex];
		size_t height     = component.height;
		size_t width      = component.width;
		// size_t sampleSize = component.precision / 8;

		const unsigned short* refData = (const unsigned short*)refComponent.data;
		const unsigned short* data    = (const unsigned short*)component.data;
		double sum = 0;
		double minDiff = 99999999;
		double maxDiff = -99999999;

		for( size_t ypos = 0; ypos < height; ++ypos )
		{
			for( size_t xpos = 0; xpos < width; ++xpos )
			{
				size_t offset = ( ypos * width + xpos );
				unsigned short refPixel = refData[offset];
				unsigned short pixel = data[offset];

				uint32_t diff = std::abs( refPixel - pixel );
				sum += diff;
				minDiff = std::min( (double)diff, minDiff );
				maxDiff = std::max( (double)diff, maxDiff );
			}
		}

		resultSums.push_back( sum );
		resultMins.push_back( minDiff );
		resultMaxs.push_back( maxDiff );
		resultMeansOfSum.push_back( sum / ( height * width ) );
	}
	sums.push_back(resultSums);
	mins.push_back(resultMins);
	maxs.push_back(resultMaxs);
	meansOfSum.push_back(resultMeansOfSum);
}
