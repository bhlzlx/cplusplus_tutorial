#include <string>
#include <regex>
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
private:	
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
        if(main_cells.size()){
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
        if(bottom_sides.size()) {
            for(uint32_t x = 0; x<x_max; ++x) {
                if(row_size-1-x != x) {
                    rect rc_1 = bottom_sides[x];
                    rect rc_2 = bottom_sides[row_size-2-x];
                    swap_cell(rc_1, rc_2);
                }
            }
        }
        if(right_sides.size()) {
            for(uint32_t y = 0; y<y_max; ++y) {
                if(row_count-1-y != y) {
                    rect rc_1 = right_sides[y];
                    rect rc_2 = right_sides[row_count-2-y];
                    swap_cell(rc_1, rc_2);
                }
            }
        }
	}

	bool valid() {
		return !!main_cells.size();
	}

};

bool is_jpeg( FILE* file ) {
	constexpr uint8_t JPEG_HEAD[] = {0xFF,0xD8}; 
	constexpr uint8_t JPEG_TAIL[] = {0xFF,0xD9}; 
	uint8_t bytes[2];
	fseek(file, 0, SEEK_SET);
	fread(bytes, 1, 2, file);
	if(memcmp(JPEG_HEAD, bytes, 2)!=0) {
		return false; 
	}
	fseek(file, -2, SEEK_END);
	fread(bytes, 1, 2, file);
	if(memcmp(JPEG_TAIL, bytes, 2)!=0) {
		return false; 
	}
	return true;
}

bool is_png( FILE* file) {
	const uint8_t PNG_HEADER[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};
	uint8_t bytes[sizeof(PNG_HEADER)];
	fseek(file, 0, SEEK_SET);
	fread(bytes, 1, sizeof(PNG_HEADER), file);
	if(memcmp(PNG_HEADER, bytes, sizeof(PNG_HEADER))!=0) {
		return false; 
	}
	return true;
}


enum class ImageType {
	PNG,
	JPEG,
	INVALID,
};

int main( int argc, char const ** argv ) {
	std::string input_path;
	std::string output_path;
	uint32_t cell_size = 32;
	#ifndef NDEBUG
	system("pause");
	#endif
	if(argc == 2) {
		input_path = argv[1];
		auto pos = input_path.rfind('.');
		if( pos == std::string::npos) {
			return -1;
		}
		output_path = std::string(input_path.c_str(), input_path.c_str()+pos);
		output_path += "obfus_";
		output_path += std::string(input_path.c_str()+pos, input_path.c_str()+input_path.length());
	} else if(argc == 4) {
		uint32_t cell_size = atoi(argv[3]);
		std::string input_path = argv[1];
		std::string output_path = argv[2];
	} else {
		printf("argument error!");
		printf("usage: imgobfus input_filepath output_filepath 32");
		return -1;
	}
	auto file = fopen(input_path.c_str(), "rb");
	if(!file) { 
		printf("file not found!");
		return -1;
	}
	ImageType type = ImageType::INVALID;
	if( is_jpeg(file)) {
		type = ImageType::JPEG;
	} else if( is_png(file)) {
		type = ImageType::PNG;
	}
	if(type == ImageType::INVALID) {
		printf("unsupported format!");
		return -1;
	}
    int w,h,n;
    auto data = stbi_load(input_path.c_str(), &w, &h, &n, 4);
    ImageObfuscater obfs( (uint32_t*)data, w, h, cell_size);
    obfs.obfuscate();
	switch(type) {
		case ImageType::JPEG:
			stbi_write_jpg(output_path.c_str(), w, h, 4, data, w*4);
			break;
		case ImageType::PNG:
			stbi_write_png(output_path.c_str(), w, h, 4, data, w*4);
	}
	return 0;
}