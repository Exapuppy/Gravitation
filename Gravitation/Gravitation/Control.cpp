# include "stdafx.h"
# include "Control.h"


void DrawBackground(HWND hWndBGR, int ID_BGR, HINSTANCE hInst)
{
	// картинка
		HDC dc, bits;
		RECT rt;
		GetClientRect(hWndBGR, &rt);
		int maxX = rt.right, maxY = rt.bottom;
		dc = GetDC(hWndBGR);
		bits = CreateCompatibleDC(dc);
		HBITMAP BGR = LoadBitmap(hInst, MAKEINTRESOURCE(ID_BGR));
		SelectObject(bits, BGR);
		StretchBlt(dc, 0, 0, maxX, maxY, bits, 0, 0, maxX, maxY, SRCCOPY);
		DeleteDC(bits);
		ReleaseDC(hWndBGR, dc);
}

bool control::keys_summ()
{
	for(int i = 255; i != 0; i--)
		if(keys[i]) return false;
	return true;
}

void TextureCtrl::UpgateTexture()
{
	Texture = new CTexture( );
};

void TextureCtrl::LoadFirstTexture()
{
	Texture -> LoadTexture ( IL_JPG, "datafiles\\Cosmos.jpg", &textures [ COSMOS_ID ] );
	TextureLoad[ COSMOS_ID ] = true;
	Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetMercury.jpg", &textures [ PLANET_MERCURY_ID] );
	TextureLoad[ PLANET_MERCURY_ID ] = true;
	Texture -> LoadTexture ( IL_JPG, "datafiles\\AsteroidCeres.jpg", &textures [ ASTEROID_CERES_ID ] );
	TextureLoad[ ASTEROID_CERES_ID ] = true;
}

void TextureCtrl::set_Planet_texture(char lpch_0, char lpch_1, char lpch_2)
{
	switch(lpch_0)
	{
	case 'M':
		switch(lpch_1)
		{
		case 'e':
				planet_texture_nummer = MercuryID;
			break;
		case 'a':
			if(lpch_2 == 'r')
			{
				if(!TextureLoad[PLANET_MARS_ID])
				{
					Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetMars.jpg", &textures [ PLANET_MARS_ID ] );
					TextureLoad[PLANET_MARS_ID] = true;
				}
				planet_texture_nummer = MarsID;
			}
			else
			{
				if(!TextureLoad[PLANET_MAKEMAKE_ID])
				{
					Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetMakeMake.jpg", &textures [ PLANET_MAKEMAKE_ID ] );
					TextureLoad[PLANET_MAKEMAKE_ID] = true;
				}
				planet_texture_nummer = MakemakeID;
			}
			break;
		default: 
			if(!TextureLoad[PLANET_ABSTRACT_ID])
			{
				Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetAbstract.jpg", &textures [ PLANET_ABSTRACT_ID ] );
				TextureLoad[PLANET_ABSTRACT_ID] = true;	 
			}
			planet_texture_nummer = AbstractID;
			break;
		}
		break;
	case 'V':
			if(!TextureLoad[PLANET_VENUS_ID])
			{
				Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetVenus.jpg", &textures [ PLANET_VENUS_ID ] );
				TextureLoad[PLANET_VENUS_ID] = true;
			}
			planet_texture_nummer = VenusID;
			break;
	case 'E':
		if(lpch_1 == 'r')
		{
			if(!TextureLoad[PLANET_ERIS_ID])
			{
				Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetEris.jpg", &textures [ PLANET_ERIS_ID ] );
				TextureLoad[PLANET_ERIS_ID] = true;
			}
			planet_texture_nummer = ErisID;
		}
		else
		{
			if(!TextureLoad[PLANET_EARTH_ID])
			{
				Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetEarth.jpg", &textures [ PLANET_EARTH_ID ] );
				TextureLoad[PLANET_EARTH_ID] = true;
			}
			planet_texture_nummer = EarthID;
		}
		break;
	case 'U':
		if(!TextureLoad[PLANET_URANUS_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetUranus.jpg", &textures [ PLANET_URANUS_ID ] );
			TextureLoad[PLANET_URANUS_ID] = true;	 
		}
		planet_texture_nummer = UranusID;
		break;
	case 'N':
		if(!TextureLoad[PLANET_NUPTUNE_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetNeptune.jpg", &textures [ PLANET_NUPTUNE_ID ] );
			TextureLoad[PLANET_NUPTUNE_ID] = true;
		}
		planet_texture_nummer = NeptuneID;
		break;
	case 'P':
		if(!TextureLoad[PLANET_PLUTO_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetPluto.jpg", &textures [ PLANET_PLUTO_ID ] );
			TextureLoad[PLANET_PLUTO_ID] = true;	 
		}
		planet_texture_nummer = PlutoID;
		break;
	case 'H':
		if(!TextureLoad[PLANET_HAUMEA_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetHaumea.jpg", &textures [ PLANET_HAUMEA_ID ] );
			TextureLoad[PLANET_HAUMEA_ID] = true;	 
		}
		planet_texture_nummer = HaumeaID;
		break;
	default: 
		if(!TextureLoad[PLANET_ABSTRACT_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\PlanetAbstract.jpg", &textures [ PLANET_ABSTRACT_ID ] );
			TextureLoad[PLANET_ABSTRACT_ID] = true;	 
		}
		planet_texture_nummer = AbstractID;
		break;
	}
}

void TextureCtrl::set_Asteroid_texture(char lpch_0)
{
	switch(lpch_0)
	{
	case 'C':
		asteroid_texture_nummer = CeresID;
	case 'P':
		if(!TextureLoad[ASTEROID_PALLAS_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\AsteroidPallas.jpg", &textures [ ASTEROID_PALLAS_ID ] );
			TextureLoad[ASTEROID_PALLAS_ID] = true;
		}
		asteroid_texture_nummer = PallasID;
		break;
	case 'V':
		if(!TextureLoad[ASTEROID_VESTA_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\AsteroidVesta.jpg", &textures [ ASTEROID_VESTA_ID ] );
			TextureLoad[ASTEROID_VESTA_ID] = true;
		}
		asteroid_texture_nummer = VestaID;
		break;
	case 'G':
		if(!TextureLoad[ASTEROID_GIGEA_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\AsteroidGigea.jpg", &textures [ ASTEROID_GIGEA_ID ] );
			TextureLoad[ASTEROID_GIGEA_ID] = true;
		}
		asteroid_texture_nummer = GigeaID;
		break;
	case 'E':
		if(!TextureLoad[ASTEROID_EUPHROSYNE_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\AsteroidEuphrosyne.jpg", &textures [ ASTEROID_EUPHROSYNE_ID ] );
			TextureLoad[ASTEROID_EUPHROSYNE_ID] = true;
		}
		asteroid_texture_nummer = EuphrosyneID;
		break;
	default: 
		if(!TextureLoad[ASTEROID_ID])
		{
			Texture -> LoadTexture ( IL_JPG, "datafiles\\Asteroid.jpg", &textures [ ASTEROID_ID ] );
			TextureLoad[ASTEROID_ID] = true;
		}
		asteroid_texture_nummer = AsteroidID;
		break;
	}
}