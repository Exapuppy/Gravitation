////////////////////////////////////////////////
// Структуры и классы необходимые для текстур //
////////////////////////////////////////////////


# include "Main.h"

//# include "devil\include\IL\il.h"

# include "devil\include\IL\ilu.h"
typedef struct _AUX_RGBImageRec
{
   GLint sizeX;
   GLint sizeY;
   unsigned char *data;
} AUX_RGBImageRec;

typedef struct TextureImage
{
	GLubyte *imageData;
	GLuint bpp;
	GLuint width;
	GLuint height;
	GLuint TexID;
} TextureImage;

class CTexture 
{
	public:
		CTexture ( );
		~ CTexture ( );

		void LoadTexture ( ILenum FileType, char *filename, TextureImage *texture );
		void FreeTexture ( TextureImage *texture );
};