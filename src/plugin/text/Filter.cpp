
#include "Filter.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <regex>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

Filter::Filter()
	: _text          ("")
	, _size          (14)
	, _offset_x      (0)
	, _offset_y      (0)
	, _red           (1.0)
	, _green         (1.0)
	, _blue          (1.0)
	, _timebase      (24.0)
	, _current_frame (0)
{
}

Filter::~Filter()
{
}

MediaioStatus Filter::configure(const Metadata* parameters)
{
	_current_frame = 0;
	size_t index = 0;
	while( 1 )
	{
		const Metadata& m = parameters[index];
		if( m.type == eMetadataTypeEnd )
		{
			break;
		}

		if( ( ! strcmp( m.id, "size" ) ) &&
			( m.type == eMetadataTypeString ) )
		{
			_size = atoi((const char*) m.data);
		}
		if( ( ! strcmp( m.id, "text" ) ) &&
			( m.type == eMetadataTypeString ) )
		{
			_text = std::string((const char*) m.data);
		}
		if( ( ! strcmp( m.id, "offset_x" ) ) &&
			( m.type == eMetadataTypeString ) )
		{
			_offset_x = atoi((const char*) m.data);
		}
		if( ( ! strcmp( m.id, "offset_y" ) ) &&
			( m.type == eMetadataTypeString ) )
		{
			_offset_y = atoi((const char*) m.data);
		}
		if( ( ! strcmp( m.id, "red" ) ) &&
			( m.type == eMetadataTypeString ) )
		{
			_red = atof((const char*) m.data);
		}
		if( ( ! strcmp( m.id, "green" ) ) &&
			( m.type == eMetadataTypeString ) )
		{
			_green = atof((const char*) m.data);
		}
		if( ( ! strcmp( m.id, "blue" ) ) &&
			( m.type == eMetadataTypeString ) )
		{
			_blue = atof((const char*) m.data);
		}
		if( ( ! strcmp( m.id, "timebase" ) ) &&
			( m.type == eMetadataTypeString ) )
		{
			_timebase = atof((const char*) m.data);
		}
		++index;
	}
	return kMediaioStatusOK;
}

MediaioStatus Filter::process(const Frame* inputFrame, Frame* outputFrame)
{
	create_components(outputFrame, inputFrame->numberOfComponents);

	for(size_t index = 0; index < inputFrame->numberOfComponents; ++index)
	{
		const Component& component_in = inputFrame->components[index];
		Component& component_out = outputFrame->components[index];

		resize_component(&component_out, component_in.width * component_in.height * 2);
		component_out.width = component_in.width;
		component_out.height = component_in.height;
		component_out.precision = component_in.precision;
		component_out.sampleSizeInByte = component_in.sampleSizeInByte;
		component_out.horizontalSubsampling = component_in.horizontalSubsampling;
		component_out.verticalSubsampling = component_in.verticalSubsampling;
		component_out.signedData = component_in.signedData;

		memcpy(component_out.data, component_in.data, component_in.width * component_in.height * 2);
	}

	std::string face_path = "/Library/Fonts/Arial.ttf";
	double angle = 0;

	std::string text = parse_string(_text);

	plot_text_utf8(
		outputFrame,
		face_path,
		_size,
		_offset_x,
		_offset_y,
		angle,
		text,
		_red,
		_green,
		_blue
	);

	_current_frame += 1;

	return kMediaioStatusOK;
}

std::string Filter::parse_string(const std::string source_text)
{
	int s = _current_frame / _timebase;
	int m = s / 60;
	int h = m / 60;
	int frames  = _current_frame % (int)_timebase;
	int seconds = s % 60;
	int minutes = m % 60;
	int hours   = h % 24;

	std::ostringstream tc;
	tc << std::setfill('0') << std::setw(2) << hours << ":";
	tc << std::setfill('0') << std::setw(2) << minutes << ":";
	tc << std::setfill('0') << std::setw(2) << seconds << ":";
	tc << std::setfill('0') << std::setw(2) << frames;

	std::regex tc_expr ("\\{timecode\\}");
	std::regex f_expr ("\\{frame\\}");
	std::string text = source_text;
	text = std::regex_replace(text, tc_expr, tc.str());
	text = std::regex_replace(text, f_expr, std::to_string(_current_frame));
	return text;
}

void plot(Frame* outputFrame, int x, int y, int red, int green, int blue)
{
	red = std::min(red, 65535);
	green = std::min(green, 65535);
	blue = std::min(blue, 65535);

	red = std::max(red, 0);
	green = std::max(green, 0);
	blue = std::max(blue, 0);

	Component& component = outputFrame->components[0];
	size_t height        = component.height;
	size_t width         = component.width;
	
	if(y < 0 || y >= height){
		return;
	}

	if(x < 0 || x >= width){
		return;
	}

	// std::cout << "plot (" << x << "," << y << ")" << std::endl;
	for( size_t componentIndex = 0; componentIndex < outputFrame->numberOfComponents; ++componentIndex )
	{
		Component& component = outputFrame->components[componentIndex];
		unsigned short* data = (unsigned short*)component.data;
		
		size_t pixel_index = ((height - y) * width) + x;

		switch(componentIndex) {
			case 0: {
				data[pixel_index] = red;
				break;
			}
			case 1: {
				data[pixel_index] = green;
				break;
			}
			case 2:{
				data[pixel_index] = blue;
				break;
			}
			default: {
				std::cout << "Text: plot out of components" << std::endl;
			}
		}
	}
};

void plot(Frame* outputFrame, int x, int y, double red, double green, double blue)
{
	plot(
		outputFrame,
		x, y,
		int(red * 65535),
		int(green * 65535),
		int(blue * 65535)
	);
};

uint16_t get_pixel_value(const Frame* frame, size_t componentIndex, size_t x, size_t y)
{
	Component& component = frame->components[componentIndex];
	unsigned short* data = (unsigned short*)component.data;
	
	size_t index = ((frame->components[0].height - y) * frame->components[0].width) + x;
	return data[index];
}

void render_bitmap(
	Frame* outputFrame,
	FT_Bitmap* bitmap,
	int x,
	int y,
	double red,
	double green,
	double blue)
{
	double temp;
	for(unsigned int j = 1u; j < bitmap->rows + 1u; j++)
	{
		for(unsigned int i = 1u; i < bitmap->width + 1u; i++)
		{
			temp = (double)(bitmap->buffer[(j-1u)*bitmap->width + (i-1u)] )/255.0;

			if(temp)
			{
				plot(
					outputFrame,
					x + i,
					y - j,
					temp * red   + (1 - temp) * get_pixel_value(outputFrame, 0, x + i, y - j),
					temp * green + (1 - temp) * get_pixel_value(outputFrame, 1, x + i, y - j),
					temp * blue  + (1 - temp) * get_pixel_value(outputFrame, 2, x + i, y - j)
				);
			}
		}
	}
}

void Filter::plot_text_utf8(
	Frame* outputFrame,
	std::string face_path,
	int fontsize,
	int x_start,
	int y_start,
	double angle,
	std::string text,
	double red,
	double green,
	double blue)
{
	FT_Library  library;
	FT_Face     face;
	FT_Matrix   matrix;      // transformation matrix
	FT_Vector   pen;

	FT_UInt glyph_index;
	FT_Error error;

	FT_Bool use_kerning;
	FT_UInt previous = 0;

	/* Set up transformation Matrix */
	matrix.xx = (FT_Fixed)( cos(angle) * 0x10000);   /* It would make more sense to do this (below), but, bizzarely, */
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000);   /* if one does, FT_Load_Glyph fails consistently.               */
	matrix.yx = (FT_Fixed)( sin(angle) * 0x10000);  //   matrix.yx = - matrix.xy;
	matrix.yy = (FT_Fixed)( cos(angle) * 0x10000);  //   matrix.yy = matrix.xx;

	/* Place starting coordinates in adequate form. */
	pen.x = x_start * 64;
	pen.y = (int)(y_start/64.0);

	/* Get length of the string */
	int num_bytes = text.size();

	// std::cout << "Num bytes is: "<< num_bytes << std::endl;

	//The array of ucs4 glyph indexes, which will by at most the number of bytes in the utf-8 file.
	long * ucs4text;
	ucs4text = new long[num_bytes+1];

	unsigned char u,v,w,x,y,z;

	int num_chars=0;

	long iii=0;

	while(iii<num_bytes)
	{
		z = text[iii];

		if(z<=127)
		{
			ucs4text[num_chars] = z;
		}

		if((192<=z)&&(z<=223))
		{
			iii++; y = text[iii];
			ucs4text[num_chars] = (z-192)*64 + (y -128);
		}

		if((224<=z)&&(z<=239))
		{
			iii++; y = text[iii];
			iii++; x = text[iii];
			ucs4text[num_chars] = (z-224)*4096 + (y -128)*64 + (x-128);
		}

		if((240<=z)&&(z<=247))
		{
			iii++; y = text[iii];
			iii++; x = text[iii];
			iii++; w = text[iii];
			ucs4text[num_chars] = (z-240)*262144 + (y -128)*4096 + (x-128)*64 + (w-128);
		}

		if((248<=z)&&(z<=251))
		{
			iii++; y = text[iii];
			iii++; x = text[iii];
			iii++; w = text[iii];
			iii++; v = text[iii];
			ucs4text[num_chars] = (z-248)*16777216 + (y -128)*262144 + (x-128)*4096 + (w-128)*64 +(v-128);
		}

		if((252==z)||(z==253))
		{
			iii++; y = text[iii];
			iii++; x = text[iii];
			iii++; w = text[iii];
			iii++; v = text[iii];
			u = text[iii];
			ucs4text[num_chars] = (z-252)*1073741824 + (y -128)*16777216   + (x-128)*262144 + (w-128)*4096 +(v-128)*64 + (u-128);
		}

		if((z==254)||(z==255))
		{
			std::cerr << " Text::plot_text_utf8 - ERROR **: Problem with character: invalid UTF-8 data."<< std::endl;
		}
		// std::cerr << "\nProblem at " << iii << ".\n";
		//
		iii++;
		num_chars++;
	}

	// num_chars now contains the number of characters in the string.
	// std::cout << "Num chars is: "<< num_chars << std::endl;

	/* Initialize FT Library object */
	error = FT_Init_FreeType( &library );
	if (error) {
		std::cerr << " Text::plot_text_utf8 - ERROR **: FreeType: Could not init Library." << std::endl;
		delete[] ucs4text;
		return;
	}

	/* Initialize FT face object */
	error = FT_New_Face(library, face_path.c_str(), 0, &face);
	if ( error == FT_Err_Unknown_File_Format ) {
		std::cerr << " Text::plot_text_utf8 - ERROR **: FreeType: Font was opened, but type not supported." << std::endl;
		delete[] ucs4text;
		return;
	} else if (error) {
		std::cerr << " Text::plot_text - ERROR **: FreeType: Could not find or load font file." << std::endl;
		delete[] ucs4text;
		return;
	}

	/* Set the Char size */
	error = FT_Set_Char_Size(
		face,          /* handle to face object           */
		0,             /* char_width in 1/64th of points  */
		fontsize * 64, /* char_height in 1/64th of points */
		100,           /* horizontal device resolution    */
		100            /* vertical device resolution      */
	);

	/* A way of accesing the glyph directly */
	FT_GlyphSlot  slot = face->glyph;  // a small shortcut

	/* Does the font file support kerning? */
	use_kerning = FT_HAS_KERNING( face );

	int n;
	for ( n = 0; n < num_chars; n++ )
	{
		/* Convert character code to glyph index */
		glyph_index = FT_Get_Char_Index( face, ucs4text[n] );

		/* Retrieve kerning distance and move pen position */
		if ( use_kerning && previous&& glyph_index )
		{
			FT_Vector  delta;
			FT_Get_Kerning(
				face,
				previous,
				glyph_index,
				ft_kerning_default, //FT_KERNING_DEFAULT,
				&delta
			);

			/* Transform this kerning distance into rotated space */
			pen.x += (int) (((double) delta.x)*cos(angle));
			pen.y +=  (int) (((double) delta.x)*( sin(angle)));
		}

		/* Set transform */
		FT_Set_Transform( face, &matrix, &pen );

		/*set char size*/

		if (error) {
			std::cerr << " Text::plot_text_utf8 - ERROR **: FreeType: Set char size error." << std::endl;
			delete[] ucs4text;
			return;
		}

		/* Retrieve glyph index from character code */
		glyph_index = FT_Get_Char_Index( face, ucs4text[n] );

		/* Load glyph image into the slot (erase previous one) */
		error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
		if (error) {
			std::cerr << " Text::plot_text_utf8 - ERROR **: FreeType: Could not load glyph (in loop). (FreeType error " << std::hex << error <<")." << std::endl;
			std::cerr.copyfmt(std::ios(NULL));
			delete[] ucs4text;
			return;
		}

		/* Convert to an anti-aliased bitmap */
		error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
		if (error) {
			std::cerr << " Text::plot_text_utf8 - ERROR **: FreeType: Render glyph error." << std::endl;
			delete[] ucs4text;
			return;
		}

		/* Now, draw to our target surface */
		render_bitmap(
			outputFrame,
			&slot->bitmap,
			slot->bitmap_left,
			y_start + slot->bitmap_top,
			red,
			green,
			blue
		);

		/* Advance to the next position */
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;

		/* record current glyph index */
		previous = glyph_index;
	}

	/* Free the face and the library objects */
	FT_Done_Face    ( face );
	FT_Done_FreeType( library );

	delete[] ucs4text;
}
