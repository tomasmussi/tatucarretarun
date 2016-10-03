// Based on "Sol's 2d gl basecode 2.0" (http://www.iki.fi/sol/)
// Modified by Mariano M. Chouza (http://www.chouza.com.ar)

#ifndef ANGELCODEFONT_H
#define ANGELCODEFONT_H
// Angelcode bitmap font generator font loader, as of 1.9c (format version 2)

class BinFile;

class ACFontInfoBlock
{
public:
    int fontSize;
    int bold;
    int italic;
    int unicode;
    int smooth;
    int charSet;
    int stretchH;
    int aa;
    int paddingUp;
    int paddingRight;
    int paddingDown;
    int paddingLeft;
    int spacingHoriz;
    int spacingVert;
    int outline;
    char * fontName;
    void load(BinFile * f);
    ~ACFontInfoBlock();
    ACFontInfoBlock();
};

class ACFontCommonBlock
{
public:
    int lineHeight;
    int base;
    int scaleW;
    int scaleH;
    int pages;
    int packed;
    int encoded;
    void load(BinFile * f);
    ACFontCommonBlock();
};

class ACFontPagesBlock
{
public:
    int pages;
    int *glhandle;
    char **name;
    void load(BinFile * f);
    ~ACFontPagesBlock();
    ACFontPagesBlock();
};


class ACFontCharBlock
{
public:
    int id;
    int x;
    int y;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int xadvance;
    int page;
    int channel;
    void load(BinFile * f);
    ACFontCharBlock();
};

class ACFontCharsBlock
{
public:
    int charcount;
    ACFontCharBlock *chars;
    void load(BinFile * f);
    ACFontCharsBlock();
    ~ACFontCharsBlock();
};


class ACFontKerningPair
{
public:
    int first;
    int second;
    int amount;
    void load(BinFile * f);
    ACFontKerningPair();
};

class ACFontKerningPairsBlock
{
public:
    int kerningPairs;
    ACFontKerningPair * pair;
    void load(BinFile * f);
    ~ACFontKerningPairsBlock();
    ACFontKerningPairsBlock();
};

class ACFont
{
public:
    ACFontInfoBlock info;
    ACFontCommonBlock common;
    ACFontPagesBlock pages;
    ACFontCharsBlock chars;
    ACFontKerningPairsBlock kerning;
    void load(const char *filename);
    void load(BinFile * f);
    void drawstring(const char * string, float x, float y, int color=0xffffffff,float desired_ht = 0.0f);
    void stringmetrics(const char * string, float &w, float &h, float &lastlinew, float desired_ht = 0.0f);
protected:
    ACFontCharBlock *findcharblock(int ch);
    int findkern(int id1, int id2);
};

#endif
