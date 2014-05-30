#ifndef IMAGING_H_123
#define IMAGING_H_123
#include "..\SOIL\SOIL.h"
#include <vector>
#include "Utils.h"

namespace SL_Image{
	/**
	The format of images that may be loaded (force_channels).
	_LOAD_AUTO leaves the image in whatever format it was found.
	_LOAD_L forces the image to load as Luminous (greyscale)
	_LOAD_LA forces the image to load as Luminous with Alpha
	_LOAD_RGB forces the image to load as Red Green Blue
	_LOAD_RGBA forces the image to load as Red Green Blue Alpha
	**/
	enum
	{
		LOAD_AUTO = 0,
		LOAD_L = 1,
		LOAD_LA = 2,
		LOAD_RGB = 3,
		LOAD_RGBA = 4
	};

	/**
	Passed in as reuse_texture_ID, will cause  to
	register a new texture ID using glGenTextures().
	If the value passed into reuse_texture_ID > 0 then
	will just re-use that texture ID (great for
	reloading image assets in-game!)
	**/
	enum
	{
		CREATE_NEW_ID = 0
	};

	/**
	flags you can pass into _load_OGL_texture()
	and _create_OGL_texture().
	(note that if _FLAG_DDS_LOAD_DIRECT is used
	the rest of the flags with the exception of
	_FLAG_TEXTURE_REPEATS will be ignored while
	loading already-compressed DDS files.)

	_FLAG_POWER_OF_TWO: force the image to be POT
	_FLAG_MIPMAPS: generate mipmaps for the texture
	_FLAG_TEXTURE_REPEATS: otherwise will clamp
	_FLAG_MULTIPLY_ALPHA: for using (GL_ONE,GL_ONE_MINUS_SRC_ALPHA) blending
	_FLAG_INVERT_Y: flip the image vertically
	_FLAG_COMPRESS_TO_DXT: if the card can display them, will convert RGB to DXT1, RGBA to DXT5
	_FLAG_DDS_LOAD_DIRECT: will load DDS files directly without _ANY_ additional processing
	_FLAG_NTSC_SAFE_RGB: clamps RGB components to the range [16,235]
	_FLAG_CoCg_Y: Google YCoCg; RGB=>CoYCg, RGBA=>CoCgAY
	_FLAG_TEXTURE_RECTANGE: uses ARB_texture_rectangle ; pixel indexed & no repeat or MIPmaps or cubemaps
	**/
	enum
	{
		FLAG_POWER_OF_TWO = 1,
		FLAG_MIPMAPS = 2,
		FLAG_TEXTURE_REPEATS = 4,
		FLAG_MULTIPLY_ALPHA = 8,
		FLAG_INVERT_Y = 16,
		FLAG_COMPRESS_TO_DXT = 32,
		FLAG_DDS_LOAD_DIRECT = 64,
		FLAG_NTSC_SAFE_RGB = 128,
		FLAG_CoCg_Y = 256,
		FLAG_TEXTURE_RECTANGLE = 512
	};

	/**
	The types of images that may be saved.
	(TGA supports uncompressed RGB / RGBA)
	(BMP supports uncompressed RGB)
	(DDS supports DXT1 and DXT5)
	**/
	enum
	{
		SAVE_TYPE_TGA = 0,
		SAVE_TYPE_BMP = 1,
		SAVE_TYPE_DDS = 2
	};

	/**
	The types of internal fake HDR representations

	_HDR_RGBE:		RGB * pow( 2.0, A - 128.0 )
	_HDR_RGBdivA:	RGB / A
	_HDR_RGBdivA2:	RGB / (A*A)
	**/
	enum
	{
		HDR_RGBE = 0,
		HDR_RGBdivA = 1,
		HDR_RGBdivA2 = 2
	};

	inline int Save_Image(
		const char *filename,
		int image_type,
		int width, int height, int channels,
		const unsigned char *const data
		) {

		return SOIL_save_image(filename, image_type, width, height, channels, data);
	}





	template<typename T>struct vec2{
		T x, y;
	};
	template<typename T>struct vec3{
		T x, y, z;
	};
	template<typename T>struct vec4{
		T x, y, z, w;
	};

	template<typename T>std::vector<T> Load_X(const std::string& str){
		int width(0), height(0), channels(0);
		if (!FileExists(str)){
			DEBUG_MSG("File: '%' Does not exist!", str);
			return std::vector<T>();
		}
		auto c = SOIL_load_image(str.c_str(), &width, &height, &channels, LOAD_L);
		if (c == NULL){
			DEBUG_MSG("SOIL Error loading file '%'. Error: '%'", str, SOIL_last_result());
			return std::vector<T>();
		}
		std::vector<T> grid;
		grid.resize(width*height);
		for (auto i = 0; i < width*height; i++) grid[i] = static_cast<T>(c[i]);
		SOIL_free_image_data(c);
		return grid;
	}

	template<typename T>std::vector<vec2<T>> Load_XY(const std::string& str){
		int width(0), height(0), channels(0);
		if (!FileExists(str)){
			DEBUG_MSG("File: '%' Does not exist!", str);
			return std::vector<T>();
		}
		auto c = SOIL_load_image(str.c_str(), &width, &height, &channels, LOAD_LA);
		if (c == NULL){
			DEBUG_MSG("SOIL Error loading file '%'. Error: '%'", str, SOIL_last_result());
			return std::vector<T>();
		}
		std::vector<vec2<T>> grid;
		grid.resize(width*height);
		unsigned int j = 0;
		for (auto i = 0; i < width*height; i++){
			grid[i].x = static_cast<T>(c[j++]);
			grid[i].y = static_cast<T>(c[j++]);
		}
		SOIL_free_image_data(c);
		return grid;
	}
	template<typename T>std::vector<vec3<T>> Load_XYZ(const std::string& str){
		int width(0), height(0), channels(0);
		if (!FileExists(str)){
			DEBUG_MSG("File: '%' Does not exist!", str);
			return std::vector<T>();
		}
		auto c = SOIL_load_image(str.c_str(), &width, &height, &channels, LOAD_RGB);
		std::vector<vec3<T>> grid;
		grid.resize(width*height);
		unsigned int j = 0;
		for (auto i = 0; i < width*height; i++){
			grid[i].x = static_cast<T>(c[j++]);
			grid[i].y = static_cast<T>(c[j++]);
			grid[i].z = static_cast<T>(c[j++]);
		}
		SOIL_free_image_data(c);
		return grid;
	}
	template<typename T>std::vector<vec4<T>> Load_XYZW(const std::string& str){
		int width(0), height(0), channels(0);
		if (!FileExists(str)){
			DEBUG_MSG("File: '%' Does not exist!", str);
			return std::vector<T>();
		}
		auto c = SOIL_load_image(str.c_str(), &width, &height, &channels, LOAD_RGBA);
		if (c == NULL){
			DEBUG_MSG("SOIL Error loading file '%'. Error: '%'", str, SOIL_last_result());
			return std::vector<T>();
		}
		std::vector<vec4<T>> grid;
		grid.resize(width*height);
		unsigned int j = 0;
		for (auto i = 0; i < width*height; i++){
			grid[i].x = static_cast<T>(c[j++]);
			grid[i].y = static_cast<T>(c[j++]);
			grid[i].z = static_cast<T>(c[j++]);
			grid[i].w = static_cast<T>(c[j++]);
		}
		SOIL_free_image_data(c);
		return grid;
	}

	/**
	Loads an image from disk into an OpenGL texture.
	\param filename the name of the file to upload as a texture
	\param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Load_OGL_texture
		(
		const char *filename,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_load_OGL_texture(filename, force_channels, reuse_texture_ID, flags);
	}

	/**
	Loads 6 images from disk into an OpenGL cubemap texture.
	\param x_pos_file the name of the file to upload as the +x cube face
	\param x_neg_file the name of the file to upload as the -x cube face
	\param y_pos_file the name of the file to upload as the +y cube face
	\param y_neg_file the name of the file to upload as the -y cube face
	\param z_pos_file the name of the file to upload as the +z cube face
	\param z_neg_file the name of the file to upload as the -z cube face
	\param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Load_OGL_cubemap
		(
		const char *x_pos_file,
		const char *x_neg_file,
		const char *y_pos_file,
		const char *y_neg_file,
		const char *z_pos_file,
		const char *z_neg_file,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_load_OGL_cubemap(x_pos_file, x_neg_file, y_pos_file, y_neg_file, z_pos_file, z_neg_file, force_channels, reuse_texture_ID, flags);
	}

	/**
	Loads 1 image from disk and splits it into an OpenGL cubemap texture.
	\param filename the name of the file to upload as a texture
	\param face_order the order of the faces in the file, any combination of NSWEUD, for North, South, Up, etc.
	\param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Load_OGL_single_cubemap
		(
		const char *filename,
		const char face_order[6],
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_load_OGL_single_cubemap(filename,
				face_order,
				force_channels,
				reuse_texture_ID,
				flags);
	}

	/**
	Loads an HDR image from disk into an OpenGL texture.
	\param filename the name of the file to upload as a texture
	\param fake_HDR_format SOIL_HDR_RGBE, SOIL_HDR_RGBdivA, SOIL_HDR_RGBdivA2
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Load_OGL_HDR_texture
		(
		const char *filename,
		int fake_HDR_format,
		int rescale_to_max,
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_load_OGL_HDR_texture(filename,
				fake_HDR_format,
				rescale_to_max,
				reuse_texture_ID,
				flags);
	}

	/**
	Loads an image from RAM into an OpenGL texture.
	\param buffer the image data in RAM just as if it were still in a file
	\param buffer_length the size of the buffer in bytes
	\param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Load_OGL_texture_from_memory
		(
		const unsigned char *const buffer,
		int buffer_length,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_load_OGL_texture_from_memory(buffer,
				buffer_length,
				force_channels,
				reuse_texture_ID,
				flags);
	}

	/**
	Loads 6 images from memory into an OpenGL cubemap texture.
	\param x_pos_buffer the image data in RAM to upload as the +x cube face
	\param x_pos_buffer_length the size of the above buffer
	\param x_neg_buffer the image data in RAM to upload as the +x cube face
	\param x_neg_buffer_length the size of the above buffer
	\param y_pos_buffer the image data in RAM to upload as the +x cube face
	\param y_pos_buffer_length the size of the above buffer
	\param y_neg_buffer the image data in RAM to upload as the +x cube face
	\param y_neg_buffer_length the size of the above buffer
	\param z_pos_buffer the image data in RAM to upload as the +x cube face
	\param z_pos_buffer_length the size of the above buffer
	\param z_neg_buffer the image data in RAM to upload as the +x cube face
	\param z_neg_buffer_length the size of the above buffer
	\param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Load_OGL_cubemap_from_memory
		(
		const unsigned char *const x_pos_buffer,
		int x_pos_buffer_length,
		const unsigned char *const x_neg_buffer,
		int x_neg_buffer_length,
		const unsigned char *const y_pos_buffer,
		int y_pos_buffer_length,
		const unsigned char *const y_neg_buffer,
		int y_neg_buffer_length,
		const unsigned char *const z_pos_buffer,
		int z_pos_buffer_length,
		const unsigned char *const z_neg_buffer,
		int z_neg_buffer_length,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_load_OGL_cubemap_from_memory(x_pos_buffer,
				x_pos_buffer_length,
				x_neg_buffer,
				x_neg_buffer_length,
				y_pos_buffer,
				y_pos_buffer_length,
				y_neg_buffer,
				y_neg_buffer_length,
				z_pos_buffer,
				z_pos_buffer_length,
				z_neg_buffer,
				z_neg_buffer_length,
				force_channels,
				reuse_texture_ID,
				flags);
	}

	/**
	Loads 1 image from RAM and splits it into an OpenGL cubemap texture.
	\param buffer the image data in RAM just as if it were still in a file
	\param buffer_length the size of the buffer in bytes
	\param face_order the order of the faces in the file, any combination of NSWEUD, for North, South, Up, etc.
	\param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Load_OGL_single_cubemap_from_memory
		(
		const unsigned char *const buffer,
		int buffer_length,
		const char face_order[6],
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_load_OGL_single_cubemap_from_memory(buffer,
				buffer_length,
				face_order,
				force_channels, reuse_texture_ID,
				flags);
	}

	/**
	Creates a 2D OpenGL texture from raw image data.  Note that the raw data is
	_NOT_ freed after the upload (so the user can load various versions).
	\param data the raw data to be uploaded as an OpenGL texture
	\param width the width of the image in pixels
	\param height the height of the image in pixels
	\param channels the number of channels: 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Create_OGL_texture
		(
		const unsigned char *const data,
		int width, int height, int channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_create_OGL_texture(data,
				width, height, channels,
				reuse_texture_ID,
				flags);
	}

	/**
	Creates an OpenGL cubemap texture by splitting up 1 image into 6 parts.
	\param data the raw data to be uploaded as an OpenGL texture
	\param width the width of the image in pixels
	\param height the height of the image in pixels
	\param channels the number of channels: 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
	\param face_order the order of the faces in the file, and combination of NSWEUD, for North, South, Up, etc.
	\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
	\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
	\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	inline unsigned int
		Create_OGL_single_cubemap
		(
		const unsigned char *const data,
		int width, int height, int channels,
		const char face_order[6],
		unsigned int reuse_texture_ID,
		unsigned int flags
		){
			return SOIL_create_OGL_single_cubemap(data,
				width, height, channels, face_order,
				reuse_texture_ID,
				flags);
	}

	/**
	Captures the OpenGL window (RGB) and saves it to disk
	\return 0 if it failed, otherwise returns 1
	**/
	inline int
		Save_screenshot
		(
		const char *filename,
		int image_type,
		int x, int y,
		int width, int height
		){
			return SOIL_save_screenshot(filename,
				image_type, x, y, width, height);
	}

	/**
	Loads an image from disk into an array of unsigned chars.
	Note that *channels return the original channel count of the
	image.  If force_channels was other than SOIL_LOAD_AUTO,
	the resulting image has force_channels, but *channels may be
	different (if the original image had a different channel
	count).
	\return 0 if failed, otherwise returns 1
	**/
	inline unsigned char*
		Load_image
		(
		const char *filename,
		int *width, int *height, int *channels,
		int force_channels
		){
			return SOIL_load_image(filename,
				width, height, channels, force_channels);
	}

	/**
	Loads an image from memory into an array of unsigned chars.
	Note that *channels return the original channel count of the
	image.  If force_channels was other than SOIL_LOAD_AUTO,
	the resulting image has force_channels, but *channels may be
	different (if the original image had a different channel
	count).
	\return 0 if failed, otherwise returns 1
	**/
	inline unsigned char*
		Load_image_from_memory
		(
		const unsigned char *const buffer,
		int buffer_length,
		int *width, int *height, int *channels,
		int force_channels
		){
			return SOIL_load_image_from_memory(buffer,
				buffer_length, width, height, channels, force_channels);
	}

	/**
	Saves an image from an array of unsigned chars (RGBA) to disk
	\return 0 if failed, otherwise returns 1
	**/
	inline int
		Save_image
		(
		const char *filename,
		int image_type,
		int width, int height, int channels,
		const unsigned char *const data
		){
			return SOIL_save_image(filename,
				image_type, width, height, channels, data);
	}

	/**
	Frees the image data (note, this is just C's "free()"...this function is
	present mostly so C++ programmers don't forget to use "free()" and call
	"delete []" instead [8^)
	**/
	inline void
		Free_image_data
		(
		unsigned char *img_data
		){
			return SOIL_free_image_data(img_data);
	}


	/**
	This function resturn a pointer to a string describing the last thing
	that happened inside SOIL.  It can be used to determine why an image
	failed to load.
	**/
	inline const char*
		Last_result
		(
		void
		){
			return SOIL_last_result();
	}



};

#endif