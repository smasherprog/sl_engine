#ifndef SL_TERRAIN_BLOCK_H_123
#define SL_TERRAIN_BLOCK_H_123
#include <vector>
#include "assert.h"

namespace SL_Terrain{
	template<class T> class Terrain_Block{
	private:
		std::vector<T> _Block;
		//blocks are always square so no need to store the dimension
	public:
		Terrain_Block(){}
		template<typename F>Terrain_Block(std::vector<F> src, unsigned int src_beg_row, unsigned int src_beg_col, unsigned int src_col_span, unsigned int src_row_span, unsigned int num_of_rows_to_copy){
			//the blocks are always square, so the num_of_rows_to_copy== the number of cols to copy as well
			auto x = src_beg_row + (num_of_rows_to_copy-1);
			auto y = x*src_col_span;
			assert((y+ src_beg_col + num_of_rows_to_copy) <= src.size());//check for buffer overrun
			_Block.resize(num_of_rows_to_copy*num_of_rows_to_copy);
			unsigned int end_row = num_of_rows_to_copy + src_beg_row;
			unsigned int blockindex = 0;
			for (; src_beg_row < end_row; src_beg_row++){
				for (unsigned int beg_col = src_beg_col; beg_col < num_of_rows_to_copy; beg_col++){
					_Block[blockindex++] = static_cast<T>(src[src_beg_row*src_col_span + beg_col]);//cast to the desired type
				}
			}
		}

	};
};

#endif