#ifndef SL_TEXTURE_FACTORY_H_123
#define SL_TEXTURE_FACTORY_H_123
#include <map>
#include <memory>
#include <string>

namespace SL_Graphics{
	class ITexture;
	namespace Texture_Factory{
		std::shared_ptr<ITexture> Create(const std::string& str);
		std::shared_ptr<ITexture> Create(unsigned int size_x, unsigned int size_y, int filter = GL_LINEAR, int repeat = GL_CLAMP_TO_EDGE, int type1 = GL_RGBA, int type2 = GL_RGBA, unsigned char* data = 0, int type3 = GL_UNSIGNED_BYTE);
		
		namespace _INTERNAL{
			extern std::map<std::string, ITexture> _Texture_Cache;
			void UnLoad(ITexture* tex);
		}
	};
};

#endif