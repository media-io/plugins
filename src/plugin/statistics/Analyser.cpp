#include "Analyser.hpp"

#include <iostream>
#include <algorithm>
#include <limits>

Analyser::Analyser()
{

}

Analyser::~Analyser()
{

}

Metadata* Analyser::analyse(const ImageFrame* decodedFrame, const int numberOfFrames)
{
	_results.clear();

	_min.resize(numberOfFrames);
	_max.resize(numberOfFrames);
	_mean.resize(numberOfFrames);

	for(int index = 0; index < numberOfFrames; ++ index)
	{
		analyseFrame(decodedFrame[ index ], _min.at(index), _max.at(index), _mean.at(index));
	}

	Metadata mEnd;
	init_metadata(&mEnd);
	mEnd.type = eMetadataTypeEnd;


	_analysis.resize( numberOfFrames );
	for(int imageIndex = 0; imageIndex < numberOfFrames; ++imageIndex )
	{
		_analysis.at(imageIndex).resize( _min[imageIndex].size() * 3 + 1 );
		for( size_t resultIndex = 0; resultIndex < _min[imageIndex].size(); ++resultIndex )
		{
			Metadata resultMin;
			init_metadata(&resultMin);
			resultMin.id = "min";
			resultMin.label = "Minimum sample value";
			resultMin.type = eMetadataTypeInt32;
			resultMin.data = &_min[imageIndex][resultIndex];
			_analysis.at(imageIndex).at( resultIndex * 3 + 0 ) = resultMin;

			Metadata resultMax;
			init_metadata(&resultMax);
			resultMax.id = "max";
			resultMax.label = "Maximum sample value";
			resultMax.type = eMetadataTypeInt32;
			resultMax.data = &_max[imageIndex][resultIndex];
			_analysis.at(imageIndex).at( resultIndex * 3 + 1 ) = resultMax;

			Metadata resultMean;
			init_metadata(&resultMean);
			resultMean.id = "mean";
			resultMean.label = "Mean value of image";
			resultMean.type = eMetadataTypeDouble;
			resultMean.data = &_mean[imageIndex][resultIndex];
			_analysis.at(imageIndex).at( resultIndex * 3 + 2 ) = resultMean;
		}
		_analysis.at(imageIndex).back() = mEnd;

		Metadata resultGroup;
		init_metadata(&resultGroup);
		resultGroup.id = "analysis";
		resultGroup.label = "Analysis of stream X";
		resultGroup.type = eMetadataTypeGroup;
		resultGroup.data = &_analysis.at(imageIndex)[0];
		_results.push_back( resultGroup );
	}

	_results.push_back(mEnd);

	return &_results[0];
}

void Analyser::analyseFrame(const ImageFrame& frame, std::vector<int>& mins, std::vector<int>& maxs, std::vector<double>& means)
{
	size_t components = frame.numberOfComponents;

	mins.resize( components, 0 );
	maxs.resize( components, 0 );
	means.resize( components, 0.0 );

	for( size_t componentIndex = 0; componentIndex < components; ++ componentIndex )
	{
		Component& component = frame.components[componentIndex];
		const char* data = (const char*) component.data;
		size_t width = component.width;
		size_t height = component.height;
		size_t sampleSize = component.precision / 8;

		int min = std::numeric_limits<int>::max();
		int max = std::numeric_limits<int>::min();
		double sum = 0.0;
		for( size_t sampleIndex = 0; sampleIndex < width * height; sampleIndex += sampleSize )
		{
			int sample = (unsigned short)
				( ( data[sampleIndex * sampleSize + 0] << 8 ) +
				    data[sampleIndex * sampleSize + 1] );

			min = std::min( sample, min );
			max = std::max( sample, max );
			sum += sample;
		}
		mins.at( componentIndex ) = min;
		maxs.at( componentIndex ) = max;
		means.at( componentIndex ) = sum / ( width * height );
	}
}
