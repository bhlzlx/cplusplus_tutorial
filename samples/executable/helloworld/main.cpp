#include "stb_image.h"
#include "stb_image_write.h"
#include <cstdint>
#include <vector>

struct rect {
	uint32_t x,y;
	uint32_t width,height;
};

class SplitedRegion {
private:
	std::vector<std::vector<rect>> 		main_cells;
	std::vector<rect> 					bottom_sides;
	std::vector<rect> 					right_sides;
	rect 								corner;
	////////////////////////////////
	uint32_t*							pixels;
public:

	SplitedRegion( uint32_t img_width, uint32_t img_height, uint32_t cell_size ) {
		if(cell_size>=img_width || cell_size <= img_height) {
			return;
		}
		uint32_t horizental_count = img_width / cell_size;
		uint32_t horizental_left = img_width % cell_size;
		uint32_t vertical_count = img_height / cell_size;
		uint32_t vertical_left = img_height % cell_size;
		// gen main cells
		for( uint32_t y = 0; y<vertical_count; ++y) {
			main_cells.emplace_back();
			for(uint32_t x = 0; x<horizental_count; ++x) {
				rect rc = {
					x*cell_size, y*cell_size, 
					cell_size, cell_size,
				};
				main_cells.back().push_back(rc);
			}
		}
		if(vertical_left) {
			for( uint32_t i = 0; i<horizental_count; ++i) {
				rect rc = {
					i*cell_size, vertical_count*cell_size,
					cell_size, vertical_left
				};
				bottom_sides.push_back(rc);
			}
		}
		if(horizental_left) {
			for( uint32_t i = 0; i<vertical_count; ++i) {
				rect rc = {
					cell_size * horizental_count, i*cell_size,
					horizental_left, cell_size
				};
				right_sides.push_back(rc);
			}
		}
		if(horizental_left&&vertical_left) {
			corner = { cell_size*horizental_count, cell_size*vertical_count, horizental_left, vertical_left };
		}
	}

	void obfuscate() {
	}

	void swap_cell( rect a, rect b ) {

	}

	bool valid() {
		return !!main_cells.size();
	}

};


int main() {
	uint32_t* bitmap = new uint32_t[480*320];
	return 0;
}