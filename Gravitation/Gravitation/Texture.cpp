//////////////////////////////////
// Работа с текстурами в OpenGL	//
//////////////////////////////////

# include "Texture.h"

CTexture :: CTexture ( )
{
	ilInit ( );
	iluInit ( );
}

CTexture :: ~ CTexture ( )
{
}

void CTexture :: LoadTexture ( ILenum FileType, char *filename, TextureImage *texture )
{
	ilLoad ( FileType, ( LPCWSTR ) filename );

	int error = ilGetError ( );

	if ( error != IL_NO_ERROR )
	{
		wchar_t * strError = iluErrorString ( error );
		MessageBox ( NULL, strError, L"Ошибка при загрузке текстуры", MB_OK );
		exit ( 1 );
	}

	texture -> width	 = ilGetInteger ( IL_IMAGE_WIDTH );
	texture -> height	 = ilGetInteger ( IL_IMAGE_HEIGHT );
	texture -> bpp		 = ilGetInteger ( IL_IMAGE_BYTES_PER_PIXEL );
	texture -> imageData = ilGetData ( );

	ilEnable ( IL_CONV_PAL );

	unsigned int type = ilGetInteger ( IL_IMAGE_FORMAT );

	glGenTextures ( 1, &texture -> TexID );
	glBindTexture ( GL_TEXTURE_2D, texture -> TexID );

	gluBuild2DMipmaps ( GL_TEXTURE_2D, texture -> bpp, 
						texture -> width, texture -> height, 
						type, GL_UNSIGNED_BYTE, texture -> imageData );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void CTexture :: FreeTexture ( TextureImage *texture )
{
}