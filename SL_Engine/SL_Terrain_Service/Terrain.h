#ifndef SL_TERRAIN_H_123
#define SL_TERRAIN_H_123
#include "Terrain_Block.h"

namespace SL_Terrain{
	class Terrain{
	private :
		Terrain_Block<unsigned int> Block;
	public:
		Terrain(){}
		virtual ~Terrain(){}


		template<class T>void Add_Block(std::vector<T> src, unsigned int src_beg_row, unsigned int src_beg_col, unsigned int src_col_span, unsigned int src_row_span, unsigned int num_of_rows_to_copy){
			Block = Terrain_Block<unsigned int>(src, src_beg_row, src_beg_col, src_col_span, src_row_span, num_of_rows_to_copy);
		}

	};
};

#endif