#include "texture_store.h"
#include <SDL.h>
#include <SDL_image.h>

TextureStore::TTexStore TextureStore::texStore_;

namespace
{
	static void do_loadtexture(const char * aFilename, int clamp = 1)
	{
		int i, j;

		// Load texture using SDL_Image
		SDL_Surface *temp = IMG_Load(aFilename);

		if (temp == NULL)
			return;

		// Set up opengl-compatible pixel format
		SDL_PixelFormat pf;
		pf.palette = NULL;
		pf.BitsPerPixel = 32;
		pf.BytesPerPixel = 4;
		pf.alpha = 0;
		pf.colorkey = 0;
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		pf.Rmask = 0x000000ff;
		pf.Rshift = 0;
		pf.Rloss = 0;
		pf.Gmask = 0x0000ff00;
		pf.Gshift = 8;
		pf.Gloss = 0;
		pf.Bmask = 0x00ff0000;
		pf.Bshift = 16;
		pf.Bloss = 0;
		pf.Amask = 0xff000000;
		pf.Ashift = 24;
		pf.Aloss = 0;
	#else    
		pf.Amask = 0x000000ff;
		pf.Ashift = 0;
		pf.Aloss = 0;
		pf.Bmask = 0x0000ff00;
		pf.Bshift = 8;
		pf.Bloss = 0;
		pf.Gmask = 0x00ff0000;
		pf.Gshift = 16;
		pf.Gloss = 0;
		pf.Rmask = 0xff000000;
		pf.Rshift = 24;
		pf.Rloss = 0;
	#endif
		// Convert texture to said format
		SDL_Surface *tm = SDL_ConvertSurface(temp, &pf,SDL_SWSURFACE);

		// Cleanup
		SDL_FreeSurface(temp);

		// Lock the converted surface for reading
		SDL_LockSurface(tm);

		int w,h,l;
		w = tm->w;
		h = tm->h;
		l = 0;
		unsigned int * mip = new unsigned int[w * h * 5];
		unsigned int * src = (unsigned int *)tm->pixels;

		memset(mip,0,tm->w*tm->h*4);

		// mark all pixels with alpha = 0 to black
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				if ((src[i*w+j] & pf.Amask) == 0)
					src[i*w+j] = 0;
			}
		}


		// Tell OpenGL to read the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tm->w, tm->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)src);

		if (mip)
		{
			// precalculate summed area tables
			// it's a box filter, which isn't very good, but at least it's fast =)
			int ra = 0, ga = 0, ba = 0, aa = 0;
			int i, j, c;
			unsigned int * rbuf = mip + (tm->w * tm->h * 1);
			unsigned int * gbuf = mip + (tm->w * tm->h * 2);
			unsigned int * bbuf = mip + (tm->w * tm->h * 3);
			unsigned int * abuf = mip + (tm->w * tm->h * 4);
	        
			for (j = 0, c = 0; j < tm->h; j++)
			{
				ra = ga = ba = aa = 0;
				for (i = 0; i < tm->w; i++, c++)
				{
					ra += (src[c] >>  0) & 0xff;
					ga += (src[c] >>  8) & 0xff;
					ba += (src[c] >> 16) & 0xff;
					aa += (src[c] >> 24) & 0xff;
					if (j == 0)
					{
						rbuf[c] = ra;
						gbuf[c] = ga;
						bbuf[c] = ba;
						abuf[c] = aa;
					}
					else
					{
						rbuf[c] = ra + rbuf[c - tm->w];
						gbuf[c] = ga + gbuf[c - tm->w];
						bbuf[c] = ba + bbuf[c - tm->w];
						abuf[c] = aa + abuf[c - tm->w];
					}
				}
			}

			while (w > 1 || h > 1)
			{
				l++;
				w /= 2;
				h /= 2;
				if (w == 0) w = 1;
				if (h == 0) h = 1;

				int dw = tm->w / w;
				int dh = tm->h / h;

				for (j = 0, c = 0; j < h; j++)
				{
					for (i = 0; i < w; i++, c++)
					{
						int x1 = i * dw;
						int y1 = j * dh;
						int x2 = x1 + dw - 1;
						int y2 = y1 + dh - 1;
						int div = (x2 - x1) * (y2 - y1);
						y1 *= tm->w;
						y2 *= tm->w;
						int r = rbuf[y2 + x2] - rbuf[y1 + x2] - rbuf[y2 + x1] + rbuf[y1 + x1];
						int g = gbuf[y2 + x2] - gbuf[y1 + x2] - gbuf[y2 + x1] + gbuf[y1 + x1];
						int b = bbuf[y2 + x2] - bbuf[y1 + x2] - bbuf[y2 + x1] + bbuf[y1 + x1];
						int a = abuf[y2 + x2] - abuf[y1 + x2] - abuf[y2 + x1] + abuf[y1 + x1];

						r /= div;
						g /= div;
						b /= div;
						a /= div;

						if (a == 0)
							mip[c] = 0;
						else
							mip[c] = ((r & 0xff) <<  0) | 
									 ((g & 0xff) <<  8) | 
									 ((b & 0xff) << 16) | 
									 ((a & 0xff) << 24); 
					}
				}
				glTexImage2D(GL_TEXTURE_2D, l, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)mip);
			}
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR); // Linear Filtering
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
			delete[] mip;
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
		}

		// Unlock..
		SDL_UnlockSurface(tm);

		// and cleanup.
		SDL_FreeSurface(tm);


		if (clamp)
		{
			// Set up texture parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		else
		{
			// Set up texture parameters
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	}

	GLuint load_texture(const char* aFilename, int clamp = 1)
	{
		// Create OpenGL texture handle and bind it to use

		GLuint texname;
		glGenTextures(1,&texname);
		glBindTexture(GL_TEXTURE_2D,texname);

		do_loadtexture(aFilename);

		return texname;
	}
}

GLuint TextureStore::addTexture(const std::string& filename)
{
	TTexStore::iterator it = texStore_.find(filename);
	
	// checks if it's already stored
	if (it != texStore_.end())
		return it->second;

	// creates the texture
	GLuint texID = load_texture(filename.c_str());

	// stores the id
	texStore_[filename] = texID;

	// returns the ID
	return texID;
}

