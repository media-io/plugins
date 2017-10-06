
#include "Filter.hpp"

#include <cstring>
#include <iostream>

Filter::Filter()
	: _init(false)
{
}

Filter::~Filter()
{
}

MediaioStatus Filter::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Filter::process(const Frame* inputFrame, Frame* outputFrame)
{
	// if(!_init)
	// {
	// 	int mode = 1;
	// 	void* eclip = nullptr;

	// 	int byte_per_sample_in = inputFrame->components[0].sampleSizeInByte;
	// 	_interp = get_interp(byte_per_sample_in);


	// 	_mainProc = get_main_proc(byte_per_sample_in, mode < 2, eclip != nullptr, get_arch(0));

	// 	_init = true;
	// }

	// _frames.push_back(inputFrame);

	// if(_frames.size() < 3)
	// {
	// 	return kMediaioStatusOK;
	// }

	create_components(outputFrame, inputFrame->numberOfComponents);

	for(size_t index = 0; index < inputFrame->numberOfComponents; ++index)
	{
		const Component& component_in = inputFrame->components[index];
		Component& component_out = outputFrame->components[index];

		resize_component(&component_out, component_in.width * component_in.height * component_in.sampleSizeInByte);
		component_out.width = component_in.width;
		component_out.height = component_in.height;
		component_out.precision = component_in.precision;
		component_out.sampleSizeInByte = component_in.sampleSizeInByte;
		component_out.horizontalSubsampling = component_in.horizontalSubsampling;
		component_out.verticalSubsampling = component_in.verticalSubsampling;
		component_out.signedData = component_in.signedData;

		memcpy(component_out.data, component_in.data, component_in.width * component_in.height * component_in.sampleSizeInByte);
	}
	std::cout << "END" << std::endl;
	// mainProc(
	// 	currp + begin * cstride,
	// 	prevp + begin * pstride,
	// 	nextp + begin * nstride,
	// 	fm_prev,
	// 	fm_next,
	// 	edeintp + begin * estride,
	// 	dstp + begin * dstride,
	// 	width,
	// 	cstride,
	// 	pstride,
	// 	nstride,
	// 	fm_pstride,
	// 	fm_nstride,
	// 	estride * 2,
	// 	dstride * 2,
	// 	count
	// );

	return kMediaioStatusOK;
}

