#include "Generator.hpp"
#include "ImageSynthExpr.hpp"

#include <SeExpression.h>
#include <iostream>

Generator::Generator()
	: _width(1920)
	, _height(1080)
	, _precision(16)
	, _components(3)
{
}

Generator::~Generator()
{
}

MediaioStatus Generator::configure(const Metadata* parameters)
{
	size_t index = 0;
	while( 1 )
	{
		const Metadata& m = parameters[index];
		if(m.type == eMetadataTypeEnd)
		{
			break;
		}
		std::cout << m.id << std::endl;
		if((!strcmp( m.id, "width")) &&
			(m.type == eMetadataTypeString))
		{
			_width = atoi((const char*) m.data);
		}
		if((!strcmp( m.id, "height")) &&
			(m.type == eMetadataTypeString))
		{
			_height = atoi((const char*) m.data);
		}
		++index;
	}

	return kMediaioStatusOK;
}

double clamp(double x) {
	return std::max(0., std::min(65535., x));
}

MediaioStatus Generator::generate(ImageFrame* frame)
{
	// ImageSynthExpr expr("fbm([10*$u,10*$v,.5])");
	ImageSynthExpr expr("\
		a=0.5+0.5 * sin(u*3000);\
		a\
		");
	expr.vars["u"] = ImageSynthExpr::Var(0.0);
	expr.vars["v"] = ImageSynthExpr::Var(0.0);
	expr.vars["w"] = ImageSynthExpr::Var(_width);
	expr.vars["h"] = ImageSynthExpr::Var(_height);
	// expr.vars["frame"] = ImageSynthExpr::Var(_time);

	bool valid = expr.isValid();
	if (!valid) {
		std::cerr << "Invalid expression " << std::endl;
		std::cerr << expr.parseError() << std::endl;
		return kMediaioStatusFailed;
	}
	// if (!expr.returnType().isFP(3)) {
	// 	std::cerr << "Expected color FP[3] got type " << expr.returnType().toString() << std::endl;
	// 	return kMediaioStatusFailed;
	// }

	init_image_frame(frame);
	create_components(frame, _components);

	double one_over_width  = 1.0 / _width;
	double one_over_height = 1.0 / _height;
	double& u = expr.vars["u"].val;
	double& v = expr.vars["v"].val;

	for (int c = 0; c < _components; ++c)
	{
		Component& component = frame->components[c];
		init_component(&component);
		component.width = _width;
		component.height = _height;
		component.precision = _precision;

		resize_component(&component, _width * _height * _precision / 8);
		memset((uint8_t*)component.data, 0x00, component.size);
	}

	uint16_t* ptr_r = (uint16_t*)frame->components[0].data;
	uint16_t* ptr_g = (uint16_t*)frame->components[1].data;
	uint16_t* ptr_b = (uint16_t*)frame->components[2].data;

	for (int row = 0; row < _height; ++row) {
		// std::cout << "row " << row << std::endl;
		for (int col = 0; col < _width; ++col) {
			u = one_over_width * (col + .5);
			v = one_over_height * (row + .5);

			// const double* result = expr.evalFP();
			const double* result = expr.evaluate();

			// expr._interpreter->print();
			*ptr_r = clamp(result[0] * 65535.);
			*ptr_g = clamp(result[1] * 65535.);
			*ptr_b = clamp(result[2] * 65535.);
			++ptr_r;
			++ptr_g;
			++ptr_b;
		}
	}

	return kMediaioStatusOK;
}
