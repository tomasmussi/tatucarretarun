// Based on "Sol's 2d gl basecode 2.0" (http://www.iki.fi/sol/)
// Modified by Mariano M. Chouza (http://www.chouza.com.ar)

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include "angelcodefont.h"
#include "fileutils.h"
#include "texture_store.h"

void ACFontInfoBlock::load(BinFile * f)
{
    int blocksize = f->readdword() - 4;
    fontSize = f->readword();
    int db = f->readbyte();
    smooth  = !!(db & (1 << 7));
    unicode = !!(db & (1 << 6));
    italic  = !!(db & (1 << 5));
    bold    = !!(db & (1 << 4));
    charSet = f->readbyte();
    stretchH = f->readword();
    aa = f->readbyte();
    paddingUp = f->readbyte();
    paddingRight = f->readbyte();
    paddingDown = f->readbyte();
    paddingLeft = f->readbyte();
    spacingHoriz = f->readbyte();
    spacingVert = f->readbyte();
    outline = f->readbyte();
    int fontnamelen = blocksize - (2 + 1 + 1 + 2 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1);
    fontName = new char[fontnamelen];
    f->readbytes(fontName, fontnamelen);
}
ACFontInfoBlock::~ACFontInfoBlock()
{
    delete[] fontName;
}
ACFontInfoBlock::ACFontInfoBlock()
{
    fontSize = bold = italic = unicode = smooth = charSet = stretchH = aa = paddingUp = 
    paddingRight = paddingDown = spacingHoriz = spacingVert = outline = 0;
    fontName = 0;
}    

void ACFontCommonBlock::load(BinFile * f)
{
    f->readdword();
    lineHeight = f->readword();
    base = f->readword();
    scaleW = f->readword();
    scaleH = f->readword();
    pages = f->readword();
    int db = f->readbyte();
    packed  = !!(db & (1 << 0));
    encoded = !!(db & (1 << 1));
}
ACFontCommonBlock::ACFontCommonBlock()
{
    lineHeight = base = scaleW = scaleH = pages = packed = encoded = 0;
}

void ACFontPagesBlock::load(BinFile * f)
{
    int blocksize = f->readdword() - 4;
    int p = f->tell();
    int l = 1;
    while (f->readbyte()) l++;
    f->seek(p);
    pages = blocksize / l;
    int i;
    name = new char*[pages];
    glhandle = new int[pages];
    for (i = 0; i < pages; i++)
    {
        name[i] = new char[l];
        f->readbytes(name[i], l);
        // should actually take the directory from the .fnt filename string..
		glhandle[i] = TextureStore::addTexture(name[i]);
    }
}
ACFontPagesBlock::~ACFontPagesBlock()
{
    int i;
    for (i = 0; i < pages; i++)
        delete[] name[i];
    delete[] name;
    delete[] glhandle;
}
ACFontPagesBlock::ACFontPagesBlock()
{
    pages = 0;
    name = 0;
	glhandle = 0;
}

void ACFontCharBlock::load(BinFile * f)
{
    id = f->readword();
    x = f->readword();
    y = f->readword();
    width = f->readword();
    height = f->readword();
    xoffset = f->readword();
    yoffset = f->readword();
    xadvance = f->readword();
    page = f->readbyte();
    channel = f->readbyte();
}

ACFontCharBlock::ACFontCharBlock()
{
    id = x = y = width = height = xoffset = yoffset = xadvance = page = channel = 0;
}

void ACFontCharsBlock::load(BinFile * f)
{
    int blocksize = f->readdword() - 4;
    charcount = blocksize / (2 * 8 + 1 + 1);
    chars = new ACFontCharBlock[charcount];
    int i;
    for (i = 0; i < charcount; i++)
        chars[i].load(f);
}

ACFontCharsBlock::ACFontCharsBlock()
{
    charcount = 0;
    chars = 0;
}

ACFontCharsBlock::~ACFontCharsBlock()
{
    delete[] chars;
}


void ACFontKerningPair::load(BinFile * f)
{
    first = f->readword();
    second = f->readword();
    amount = f->readword();
}

ACFontKerningPair::ACFontKerningPair()
{
    first = second = amount = 0;
}    

void ACFontKerningPairsBlock::load(BinFile * f)
{
    int blocksize = f->readdword() - 4;
    kerningPairs = blocksize / (2 + 2 + 2);
    pair = new ACFontKerningPair[kerningPairs];
    int i;
    for (i = 0; i < kerningPairs; i++)
        pair[i].load(f);
}

ACFontKerningPairsBlock::~ACFontKerningPairsBlock()
{
    delete[] pair;
}

ACFontKerningPairsBlock::ACFontKerningPairsBlock()
{
    kerningPairs = 0;
    pair = 0;
}

void ACFont::load(const char *filename)
{
	BinFile * f = new BinFile(filename);
    load(f);
    delete f;
}

void ACFont::load(BinFile * f)
{
    if (f == NULL) return;
    if (f->readbyte() != 'B') return; // B
    if (f->readbyte() != 'M') return; // M
    if (f->readbyte() != 'F') return; // F
    if (f->readbyte() != 2) return;    // ver 2
    if (f->readbyte() != 1) return;
    info.load(f);
    if (f->readbyte() != 2) return;
    common.load(f);
    if (f->readbyte() != 3) return;
    pages.load(f);
    if (f->readbyte() != 4) return;
    chars.load(f);
    if (f->readbyte() != 5) return; // this is ok to fail
    kerning.load(f);         
}

ACFontCharBlock * ACFont::findcharblock(int ch)
{
    int i;
    for (i = 0; i < chars.charcount; i++)
    {
        if (chars.chars[i].id == ch)
            return &(chars.chars[i]);
    }
    return chars.chars;
}

int ACFont::findkern(int id1, int id2)
{
    int i;
    for (i = 0; i < kerning.kerningPairs; i++)
        if (kerning.pair[i].first == id1 && kerning.pair[i].second == id2)
            return kerning.pair[i].amount;
    return 0;
}


void ACFont::drawstring(const char * string, float x, float y, int color, float desired_ht)
{
    float scalefactor;
    if (desired_ht == 0.0f)
        scalefactor = 1.0f;
    else
        scalefactor = desired_ht / common.lineHeight;
    
    int currentpage = 0;
    if (pages.pages == 0)
        return;

    float xofs, yofs;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pages.glhandle[0]);
    glColor4f(((color >> 16) & 0xff) / 256.0f,
              ((color >> 8) & 0xff) / 256.0f,
              ((color >> 0) & 0xff) / 256.0f,
              ((color >> 24) & 0xff) / 256.0f);
    xofs = x;
    yofs = y;
    int lastid = 0;
    while (*string)
    {
        xofs += findkern(lastid, *string) * scalefactor;
        lastid = *string;
        if (*string == '\n')
        {
            xofs = x;
            yofs += common.lineHeight * scalefactor;
        }
        else
        {
            ACFontCharBlock *curr = findcharblock(*string);
            if (curr->page != currentpage)
            {
                currentpage = curr->page;
                glBindTexture(GL_TEXTURE_2D, pages.glhandle[currentpage]);
            }
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(curr->x / (float)common.scaleW, curr->y / (float)common.scaleH);
            glVertex2f(xofs + curr->xoffset * scalefactor, yofs - curr->yoffset * scalefactor);

            glTexCoord2f(curr->x / (float)common.scaleW, (curr->y + curr->height) / (float)common.scaleH);
            glVertex2f(xofs + curr->xoffset * scalefactor, yofs - (curr->yoffset + curr->height) * scalefactor);

            glTexCoord2f((curr->x + curr->width) / (float)common.scaleW, curr->y / (float)common.scaleH);
            glVertex2f(xofs + (curr->xoffset + curr->width) * scalefactor, yofs - curr->yoffset * scalefactor);

            glTexCoord2f((curr->x + curr->width) / (float)common.scaleW, (curr->y + curr->height) / (float)common.scaleH);
            glVertex2f(xofs + (curr->xoffset + curr->width) * scalefactor, yofs - (curr->yoffset + curr->height) * scalefactor);
        glEnd();
            xofs += curr->xadvance * scalefactor;
        }
        string++;
    }
    glDisable(GL_TEXTURE_2D);
}

void ACFont::stringmetrics(const char * string, float &w, float &h, float &lastlinew, float desired_ht)
{
    float scalefactor;
    if (desired_ht == 0.0f)
        scalefactor = 1.0f;
    else
        scalefactor = desired_ht / common.lineHeight;

    if (pages.pages == 0)
    {
        w = h = 0;
        return;
    }
    float maxx = 0;

    float xofs, yofs;
    xofs = 0;
    yofs = 0;
    int lastid = 0;
    while (*string)
    {
        xofs += findkern(lastid, *string) * scalefactor;
        lastid = *string;
        if (*string == '\n')
        {
            xofs = 0;
            yofs += common.lineHeight * scalefactor;
        }
        else
        {
            ACFontCharBlock *curr = findcharblock(*string);
            xofs += curr->xadvance * scalefactor;
        }
        if (maxx < xofs) maxx = xofs;
        string++;
    }
    lastlinew = xofs;
    w = maxx;
    h = yofs + common.lineHeight * scalefactor;
}
