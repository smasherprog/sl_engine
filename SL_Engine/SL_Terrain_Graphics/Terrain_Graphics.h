#ifndef TERRAIN_GRAPHICS_H_123
#define TERRAIN_GRAPHICS_H_123
#include "..\SL_Terrain_Service\Terrain.h"
#include <memory>
namespace SL_Graphics{
	class Shader;
	class Camera;
};
namespace SL_Terrain{
	class Terrain_Graphics: public Terrain{
		unsigned int _VB;
		unsigned int _IB;
		unsigned int _VAO;
		unsigned int  matrix;
		unsigned int num_indices;
		std::unique_ptr<SL_Graphics::Shader> _Shader;
	public:
		Terrain_Graphics();
		virtual ~Terrain_Graphics();

		void Draw(double dt, SL_Graphics::Camera* c);

	};

};

#endif