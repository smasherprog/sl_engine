#ifndef SHAPES_H_123
#define SHAPES_H_123
#include <vector>
#include "..\glm\glm.hpp"

namespace SL_Graphics{
	namespace Shapes{
		template<typename T>std::vector<T> Generate_Grid_Indices(T width, T height){
			auto n = (long long)((T)((width-1)*(height-1)*6));
			auto m = ((long long)width - 1)*((long long)height - 1) * 6;
			assert(m == n);//if they are are not equal it is because T was truncated
			std::vector<T> grid;
			T index = 0;
			grid.resize((width-1)*(height-1)*6);
			for (T row = 0; row < height-1; row++){
				for (T col = 0; col < width-1; col++){
					T start = row * width + col;
					grid[index++] = start;
					grid[index++] = start+1;
					grid[index++] = start + width;
					grid[index++] = start + 1;
					grid[index++] = start + 1 + width;
					grid[index++] = start + width;
				}
			}
			return grid;
		}
		template<typename T> std::vector<T> Generate_Grid_Vertices(unsigned int width, unsigned int height){ }
		template<> std::vector<glm::vec2> Generate_Grid_Vertices<glm::vec2>(unsigned int width, unsigned int height){
			std::vector<glm::vec2> grid;
			grid.resize(width*height);
			unsigned int index = 0;
			for (unsigned int row = 0; row < height; row++){
				for (unsigned int col = 0; col < width; col++){
					grid[index++] = glm::vec2(col, row);
				}
			}
			return grid;
		}
		template<> std::vector<glm::vec3> Generate_Grid_Vertices<glm::vec3>(unsigned int width, unsigned int height){
			std::vector<glm::vec3> grid;
			grid.resize(width*height);
			unsigned int index = 0;
			for (unsigned int row = 0; row < height; row++){
				for (unsigned int col = 0; col < width; col++){
					grid[index++] = glm::vec3(col, 0.0f, row);
				}
			}
			return grid;
		}
		template<typename T> std::vector<glm::vec3> Generate_Grid_Vertices(unsigned int width, unsigned int height, std::vector<T> heights){
			assert(heights.size() > width*height);
			std::vector<glm::vec3> grid;
			grid.resize(width*height);
			unsigned int index = 0;
			for (unsigned int row = 0; row < height; row++){
				for (unsigned int col = 0; col < width; col++){
					grid[index] = glm::vec3(col, heights[index], row);
					index += 1;
				}
			}
			return grid;
		}
	};
};

#endif