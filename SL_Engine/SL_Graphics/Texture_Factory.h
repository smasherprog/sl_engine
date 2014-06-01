#ifndef SL_TEXTURE_FACTORY_H_123
#define SL_TEXTURE_FACTORY_H_123
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "..\SL_Utilities\SOIL.h"

namespace SL_Graphics{
	class Texture;


	namespace Texture_Factory{
		void Init();
		void DeInit();
		std::shared_ptr<Texture> Load(const std::string& str);
		
		namespace _INTERNAL{
			extern std::map<std::string, Texture> _Texture_Cache;
			void UnLoad(Texture* tex);
		}
	};
};

#endif