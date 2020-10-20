#ifndef CN_LOAD_MASTER_H
#define CN_LOAD_MASTER_H

#include <cn_core.hpp>

// Predeclarations
namespace CN
{
	namespace GL
	{
		class CN_API Texture;
		class CN_API Mesh;
		class CN_API Sprite;
	}
}

namespace CN
{
	/// LoadMaster Singleton class
	/// --Deals with loading of
	/// -->textures and meshes(obj/dae)
	/// -->shaders, text files and file streams
	/// -->binary objects
	class CN_API LoadMaster
	{
	public : // Constants&Macroses
#define MAX_LOAD_SIZE 1024 * 100
	public: // Enums
		enum class MeshFormats {
			DEFAULT = 0, OBJ = 1, DAE
		};
		enum class ImgFormats {
			DEFAULT = 0, PNG = 1, JPG, DDS
		};
	public: // Methods
		// Destructor
		~LoadMaster();

		// Accessors
		LoadMaster& get() const { static LoadMaster s_instance; return s_instance; }
		
		UInt getTexCount() const { return texturesCount; }
		UInt getMeshesCount() const { return meshesCount; }

		// Saving
		/// Save the data with the given size into the file by given path
		bool saveIt_bin(const std::string& path,
			void* data, size_t bytes);
		// Loading
		/// Load bytes of the given size from the file into the given data
		bool loadIt_bin(const std::string& path,
			void* data, size_t bytes);
		/// Load image bytes from the file and initialize GL texture struct
		bool loadIt_tex2d(const std::string& path,
			GL::Texture* texPtr);
		/// Load vertex and material data from the file and initialize GL mesh class
		bool loadIt_mesh(const std::string& path,
			GL::Mesh* meshPtr);
		/// Load material and image data from the file and initialize GL sprite class
		bool loadIt_sprite(const std::string& path,
			GL::Sprite* spritePtr);
	private: // Members
		size_t bytesCount;
		UInt texturesCount;
		UInt meshesCount;
	private: // Implementation Functions
		// Private singleton Constructor
		LoadMaster();
		/// -Parse mesh file depending on it's format
		/// --Write vertex and material data into the mesh struct
		void load_mesh(const std::string& path,
			GL::Mesh* meshPtr, MeshFormats format);
		/// -Load binary information from the file and parse it depending on file format
		/// --Return byte array with written pixel data
		UChar* load_img(const std::string& path,
			MeshFormats format);
	};
}

#endif