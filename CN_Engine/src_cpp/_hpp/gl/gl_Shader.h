#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <cn_core.hpp>
#include <cn_pch.hpp>
#include <gl/gl_declarations.hpp>

// Shader
namespace CN
{
	namespace GL
	{
		/// Shader class - handler for shader program OpenGL
		/// --Has m_id, 3 shaders: vertex, fragment, [geometry]
		/// --Can set uniforms via bunch of comands
		class Shader
		{
		private:
			enum class ShaderTypes {
				NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
			};
			struct ShaderSources
			{
				std::string vertexSource;
				std::string fragmentSource;
				std::string geometrySource;
			};
		private:
			ShaderSources m_shaderSources;
			GLuint m_id;
			GLuint m_fragSh, m_vertSh, m_geomSh;
			mutable std::unordered_map<std::string, GLint> m_uniformCashe;

			mutable bool m_isBeeingUsed = false;
		private:
			GLint getUniformLoc(const std::string& name) const;

			static ShaderSources getShaderCode(const std::string& path);
			static GLuint loadShader(const char* shaderPath, GLenum shaderType);
			static void debugShader(unsigned int shader, GLenum shaderType);
		public:
			Shader();
			Shader(const char* vertexPath, const char* fragmentPath,
				const char* geometryPath = nullptr);
			~Shader();

			GLuint getID() const { return m_id; }

			void use() const;
			void stopUse() const;
			void del();

			static void initShader(Shader* shaderPtr, const char* vertexPath, const char* fragmentPath,
				const char* geometryPath = nullptr);
			//Setting uniforms into a shader
			void setBool(const std::string& name, int value) const;
			void setInt(const std::string& name, int value) const;
			void setFloat(const std::string& name, float value) const;
			/*
			void setVec2(const std::string& name, glm::vec2& value) const;
			void setVec2(const std::string& name,
				float x, float y) const;

			void setVec3(const std::string& name, glm::vec3& value) const;
			void setVec3(const std::string& name,
				float x, float y, float z) const;

			void setVec4(const std::string& name, glm::vec4& value) const;
			void setVec4(const std::string& name,
				float x, float y, float z, float w) const;

			void setMat2(const std::string& name, glm::mat2& value) const;
			void setMat3(const std::string& name, glm::mat3& value) const;
			void setMat4(const std::string& name, glm::mat4& value) const;*/
		};
	}
}
#endif