
#include "Filter.hpp"

#include <cstring>
#include <iostream>

Filter::Filter()
{
}

Filter::~Filter()
{
}

MediaioStatus Filter::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Filter::process(const ImageFrame* inputFrame, ImageFrame* outputFrame)
{
	create_components(outputFrame, inputFrame->numberOfComponents);

	for(size_t index = 0; index < inputFrame->numberOfComponents; ++index)
	{
		const Component& component_in = inputFrame->components[index];
		Component& component_out = outputFrame->components[index];

		const int new_height = component_in.height * 2;

		resize_component(&component_out, component_in.width * new_height * component_in.sampleSizeInByte);
		component_out.width = component_in.width;
		component_out.height = new_height;
		component_out.precision = component_in.precision;
		component_out.sampleSizeInByte = component_in.sampleSizeInByte;
		component_out.horizontalSubsampling = component_in.horizontalSubsampling;
		component_out.verticalSubsampling = component_in.verticalSubsampling;
		component_out.signedData = component_in.signedData;

		char* ptr_src = (char*)component_in.data;
		char* ptr_dst = (char*)component_out.data;
		size_t line_size = component_in.width * component_in.sampleSizeInByte;

		// std::cout << component_in.width << "x" << component_in.height << " --> " << component_in.width << "x" << new_height << std::endl;
		// if(index == 0){
			for (int i = 0; i < component_in.height; ++i) {
				memcpy(ptr_dst, ptr_src, line_size);
				ptr_dst += line_size;
				memcpy(ptr_dst, ptr_src, line_size);
				ptr_src += line_size;
				ptr_dst += line_size;
			}
		// } else {
		// 	for (int i = 0; i < component_in.height; ++i) {
		// 		memcpy(ptr_dst, ptr_src, line_size / 2);
		// 		ptr_src += line_size;
		// 		ptr_dst += line_size;
		// 	}
		// }
	}

	return kMediaioStatusOK;
}
