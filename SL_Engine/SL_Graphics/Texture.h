#ifndef SL_TEXTURE_H_123
#define SL_TEXTURE_H_123
#include <functional>

namespace SL_Graphics{

	class Texture{
		std::function<void(Texture*)> _OnDestruction;
		unsigned int _TextureID;
	public: 
		Texture() = delete;
		Texture& operator=(const Texture& rhs) = delete;
		Texture(std::function<void(Texture*)> on_destruction, unsigned int texid) : _OnDestruction(on_destruction), _TextureID(texid) {}
		virtual ~Texture(){ _OnDestruction(this); }
	};

};

#endif