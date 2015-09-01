#ifndef TEXTURE_H
#define TEXTURE_H

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

#ifdef D3D_SUPPORT
#include <windows.h>
#include <d3d11.h>
#endif

//Other Project Headers
#include "BrickwareUtils/StringUtils.hpp"

//System Level Headers

//Project Headers

namespace Brickware
{
	namespace Graphics
	{
		enum BRICKWARE_GRAPHICS_API TextureType{
			RGBA
		};

		class BRICKWARE_GRAPHICS_API Texture
		{
			friend class Material;

		public:
			Texture(const char* textureFileName);

			unsigned char* getPixels();

			~Texture();

		private:
			void loadBMP(const char* textureFileName);

			void bufferTexture();
			void bindTexture();
			void freeTexture();

			TextureType textureType;

#ifdef GL_SUPPORT
			GLuint glTexture;
			GLenum glTextureType;

			void bufferGL();
			void bindGL();
			void freeGL();
#endif

#ifdef D3D_SUPPORT
			ID3D11Texture2D* d3dTexture;
			ID3D11ShaderResourceView* d3dTextureSRV;

			void bufferD3D();
			void bindD3D();
			void freeD3D();
#endif

			int width;
			int height;

			short colorDepth; //Bits per pixel
			int compressionType;

			unsigned char* pixels;
		};
	}
}
#endif
