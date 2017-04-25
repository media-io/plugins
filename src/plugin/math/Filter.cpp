
#include "Filter.hpp"

#include <iostream>


Filter::Filter()
{
	_target = Halide::get_host_target();

	// std::string platform = "";

	// switch( _target.os )
	// {
	// 	case Halide::Target::OSUnknown:
	// 	{
	// 		platform = "unknown";
	// 		break;
	// 	}
	// 	case Halide::Target::Linux:
	// 	{
	// 		platform = "Linux";
	// 		break;
	// 	}
	// 	case Halide::Target::Windows:
	// 	{
	// 		platform = "Windows";
	// 		break;
	// 	}
	// 	case Halide::Target::OSX:
	// 	{
	// 		platform = "OSX";
	// 		break;
	// 	}
	// 	case Halide::Target::Android:
	// 	{
	// 		platform = "Android";
	// 		break;
	// 	}
	// 	case Halide::Target::IOS:
	// 	{
	// 		platform = "IOS";
	// 		break;
	// 	}
	// 	case Halide::Target::NaCl:
	// 	{
	// 		platform = "NaCl";
	// 		break;
	// 	}
	// }

	// _target.set_feature( Halide::Target::OpenGL, true );
	// _target.set_feature( Halide::Target::CUDA, true );
	// _target.set_feature( Halide::Target::OpenCL, true );
	// _target.set_feature( Halide::Target::Matlab, true );

	// std::cout << "Platform        : " << platform << std::endl;

	// std::cout << "JIT             : " << ( _target.has_feature( Halide::Target::JIT           ) ? "yes" : " no" ) << std::endl;
	// std::cout << "SSE 4.1         : " << ( _target.has_feature( Halide::Target::SSE41         ) ? "yes" : " no" ) << std::endl;
	// std::cout << "OpenGL          : " << ( _target.has_feature( Halide::Target::OpenGL        ) ? "yes" : " no" ) << std::endl;
	// std::cout << "OpenGL Compute  : " << ( _target.has_feature( Halide::Target::OpenGLCompute ) ? "yes" : " no" ) << std::endl;
	// std::cout << "Cuda            : " << ( _target.has_feature( Halide::Target::CUDA          ) ? "yes" : " no" ) << std::endl;
	// std::cout << "OpenCL          : " << ( _target.has_feature( Halide::Target::OpenCL        ) ? "yes" : " no" ) << std::endl;
	// std::cout << "Matlab          : " << ( _target.has_feature( Halide::Target::Matlab        ) ? "yes" : " no" ) << std::endl;


	// std::cout << _target.to_string() << std::endl;

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
	create_components(outputFrame, inputFrame->numberOfComponents);

	for(size_t index = 0; index < inputFrame->numberOfComponents; ++index)
	{
		Component& component = inputFrame->components[index];

		Halide::Buffer buffer(
			Halide::UInt( 16 ),
			component.width,
			component.height,
			1, // number of components
			0,
			(uint8_t*)component.data
		);

		Halide::Image<uint16_t> input( buffer );

		Halide::Expr value = input(_x, _y, _c);

		value = Halide::cast<float>(value);
		value = value * 4.0f;
		value = Halide::min(value, 65535.0f);
		value = Halide::cast<uint16_t>(value);
		
		Halide::Func mathOperator;
		mathOperator(_x, _y, _c) = value;

		Halide::Image<uint16_t> output = mathOperator.realize(input.width(), input.height(), input.channels());

		resize_component(&outputFrame->components[index], component.width * component.height * 2);
		outputFrame->components[index].width = component.width;
		outputFrame->components[index].height = component.height;
		outputFrame->components[index].precision = component.precision;
		outputFrame->components[index].sampleSizeInByte = component.sampleSizeInByte;
		outputFrame->components[index].horizontalSubsampling = component.horizontalSubsampling;
		outputFrame->components[index].verticalSubsampling = component.verticalSubsampling;
		outputFrame->components[index].signedData = component.signedData;

		memcpy( outputFrame->components[index].data, output.data(), component.width * component.height * 2 );
	}

	return kMediaioStatusOK;
}
