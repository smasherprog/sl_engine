#ifndef SL_TEXTURE_H_123
#define SL_TEXTURE_H_123
#include <functional>

namespace SL_Graphics{

	class ITexture{
		std::function<void(ITexture*)> _OnDestruction;
		unsigned int _TextureID;
	public: 
		ITexture() = delete;
		ITexture& operator=(const ITexture& rhs) = delete;

		ITexture(std::function<void(ITexture*)> on_destruction, unsigned int texid) : _OnDestruction(on_destruction), _TextureID(texid) {}
		virtual ~ITexture(){ _OnDestruction(this); }
		virtual void Bind(){}
	};

};

#endif