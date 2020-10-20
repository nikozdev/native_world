#include <cn_pch.hpp>
#include <cn\cn_LoadMaster.h>
#include <extern/stbiLoad/stb_image.h>
#include <extern/GLEW/glew.h>

// LoadMaster
namespace CN
{
    // Constructor&Destructor
	LoadMaster::LoadMaster() :
        bytesCount(0), texturesCount(0), meshesCount(0)
	{
	}
    LoadMaster::~LoadMaster()
    {
    }

    bool LoadMaster::saveIt_bin(const std::string& path,
        void* data, size_t bytes)
    {
        std::ofstream saver(path, std::ios::out);
        if (!saver.is_open())
        {
            printf("CN::LOAD_MASTER::SAVE_IT_BIN: Failed to save the data by path\n%s\n",
                path.c_str());
            return false;
        }

        saver.write((char*)data, bytes);

        saver.close();
        return true;
    }

    bool LoadMaster::loadIt_bin(const std::string& path,
        void* data, size_t bytes)
    {
        std::ifstream loader(path, std::ios::in);
        if (!loader.is_open())
        {
            printf("CN::LOAD_MASTER::LOAD_IT_BIN: Failed to load the data by path\n%s\n",
                path.c_str());
            return false;
        }

        while (loader.read((char*)data, bytes));
        bytesCount += bytes;

        loader.close();
        return true;
    }

    bool LoadMaster::loadIt_tex2d(const std::string& path, GL::Texture* texPtr)
    {
        return false;
    }

    bool LoadMaster::loadIt_mesh(const std::string& path, GL::Mesh* meshPtr)
    {
        return false;
    }

    bool LoadMaster::loadIt_sprite(const std::string& path, GL::Sprite* spritePtr)
    {
        return false;
    }

    // 
    /*
    int _png_load(const char* file, int* width, int* height) {
        FILE* f;
        int is_png, bit_depth, color_type, row_bytes;
        png_infop info_ptr, end_info;
        uint32_t t_width, t_height;
        byte header[8], * image_data;
        byte* row_pointers;
        png_structp png_ptr;
        GLuint texture;
        int alpha;

        if (!(f = fopen(file, "r"))) {
            return 0;
        }
        fread(header, 1, 8, f);
        is_png = !png_sig_cmp(header, 0, 8);
        if (!is_png) {
            fclose(f);
            return 0;
        }
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
            NULL, NULL);
        if (!png_ptr) {
            fclose(f);
            return 0;
        }
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, (png_infopp)NULL,
                (png_infopp)NULL);
            fclose(f);
            return 0;
        }
        end_info = png_create_info_struct(png_ptr);
        if (!end_info) {
            png_destroy_read_struct(&png_ptr, (png_infopp)NULL,
                (png_infopp)NULL);
            fclose(f);
            return 0;
        }
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            fclose(f);
            return 0;
        }
        png_init_io(png_ptr, f);
        png_set_sig_bytes(png_ptr, 8);
        png_read_info(png_ptr, info_ptr);
        png_get_IHDR(png_ptr, info_ptr, &t_width, &t_height, &bit_depth,
            &color_type, NULL, NULL, NULL);
        *width = t_width;
        *height = t_height;
        png_read_update_info(png_ptr, info_ptr);
        row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        image_data = (png_bytep)malloc(row_bytes * t_height * sizeof(png_byte));
        if (!image_data) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            fclose(f);
            return 0;
        }
        row_pointers = (png_bytepp)malloc(t_height * sizeof(png_bytep));
        if (!row_pointers) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            free(image_data);
            fclose(f);
            return 0;
        }
        for (unsigned int i = 0; i < t_height; ++i) {
            row_pointers[t_height - 1 - i] = image_data + i * row_bytes;
        }
        png_read_image(png_ptr, row_pointers);
        switch (png_get_color_type(png_ptr, info_ptr)) {
        case PNG_COLOR_TYPE_RGBA:
            alpha = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = GL_RGB;
            break;
        default:
            printf("Color type %d not supported!\n",
                png_get_color_type(png_ptr, info_ptr));
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            return 0;
        }
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0,
            alpha, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        free(row_pointers);
        fclose(f);
        return texture;
    }*/
}