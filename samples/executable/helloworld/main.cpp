#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdint>
#include <vector>

struct rect {
	uint32_t x,y;
	uint32_t width,height;
};

class ImageObfuscater {
private:
	std::vector<std::vector<rect>> 		main_cells;
	std::vector<rect> 					bottom_sides;
	std::vector<rect> 					right_sides;
	rect 								corner;
	////////////////////////////////
	uint32_t*							bitmap_data;
    uint32_t                            bitmap_width; 
    uint32_t                            bitmap_height; 
public:

	ImageObfuscater( uint32_t * data, uint32_t img_width, uint32_t img_height, uint32_t cell_size )
        : main_cells()
        , bottom_sides()
        , right_sides()
        , corner()
        , bitmap_data(data)
        , bitmap_width(img_width)
        , bitmap_height(img_height)
    {
		if(cell_size>=img_width || cell_size >= img_height) {
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
        auto row_count = main_cells.size();
        if(!row_count) {
            return;
        }
        auto row_size = main_cells[0].size();
        if(!row_size) {
            return;
        }
        auto x_max = ((row_size + 1)&~(1)) >> 1;
        auto y_max = ((row_count+ 1)&~(1)) >> 1;
        {
            for( uint32_t y = 0; y<row_count; ++y) {
                for( uint32_t x = 0; x<row_size; ++x) {
                    if(((float)row_count - y)/((float)x+1) > (float)row_count/(float)row_size) {
                        continue;
                    }
                    rect rc_1 = main_cells[y][x];
                    rect rc_2 = main_cells[row_count-1-y][row_size-1-x];
                    swap_cell(rc_1, rc_2);
                }
            }
        }
        {
            for(uint32_t x = 0; x<x_max; ++x) {
                if(row_size-1-x != x) {
                    rect rc_1 = bottom_sides[x];
                    rect rc_2 = bottom_sides[row_size-2-x];
                    swap_cell(rc_1, rc_2);
                }
            }
        }
        {
            for(uint32_t y = 0; y<y_max; ++y) {
                if(row_count-1-y != y) {
                    rect rc_1 =right_sides[y];
                    rect rc_2 =right_sides[row_size-2-y];
                    swap_cell(rc_1, rc_2);
                }
            }
        }
	}

    void swap_pixel( uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2 ) {
        uint32_t index1 = x1 + y1 * bitmap_width;
        uint32_t index2 = x2 + y2 * bitmap_width;
        uint32_t t = bitmap_data[index1];
        bitmap_data[index1] = bitmap_data[index2];
        bitmap_data[index2] = t;
    }

	void swap_cell( rect a, rect b ) {
        uint32_t ax = a.x;
        uint32_t ay = a.y;
        uint32_t bx = b.x;
        uint32_t by = b.y;
        // 
        for(uint32_t y = 0; y<a.height; ++y) {
            for(uint32_t x = 0; x<a.width; ++x) {
                swap_pixel(ax+x, ay+y, bx+x, by+y);
            }
        }
	}

	bool valid() {
		return !!main_cells.size();
	}

};


int main() {
    int w,h,n;
    auto data = stbi_load("d:/sasara.png", &w, &h, &n, 4);
    ImageObfuscater obfs( (uint32_t*)data, w, h, 32 );
    obfs.obfuscate();
    stbi_write_png("d:/output.png", w,h,4, data, w*4);
	return 0;
}