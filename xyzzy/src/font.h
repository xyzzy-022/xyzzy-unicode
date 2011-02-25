#ifndef _font_h_
#define _font_h_

class FontObject
{
protected:
  HFONT fo_hfont;
  int fo_need_pad;
  POINT fo_offset;
  SIZE fo_size;
  int fo_ascent;
  LOGFONT fo_logfont;
public:
  FontObject () : fo_hfont (0) {}
  ~FontObject () {if (fo_hfont) DeleteObject (fo_hfont);}
  int create (const LOGFONT &);
  int create (const TCHAR *, int, int);
  operator HFONT () const {return fo_hfont;}
  int need_pad_p () const {return fo_need_pad;}
  void require_pad () {fo_need_pad = 1;}
  void get_metrics (HDC, SIZE &, SIZE &);
  void calc_offset (const SIZE &);
  const SIZE &size () const {return fo_size;}
  const POINT &offset () const {return fo_offset;}
  int ascent () const {return fo_ascent;}
  const LOGFONT &logfont () const {return fo_logfont;}
};

#define FONT_ASCII          0
#define FONT_JP             1
#define FONT_LATIN          2
#define FONT_CYRILLIC       3
#define FONT_GREEK          4
#define FONT_CN_SIMPLIFIED  5
#define FONT_CN_TRADITIONAL 6
#define FONT_HANGUL         7
#define FONT_GEORGIAN       8
#define FONT_MAX            9

#ifdef UNICODE

struct glyph_info
{
  int8_t   font_index;
  uint8_t  cell_width;
  uint16_t pixel_width;
  WORD     glyph_index;

  static const int8_t FONT_INDEX_DEFCHAR = -1;
  static const int8_t FONT_INDEX_UNBOUND = -2;

  glyph_info (int8_t fi = FONT_INDEX_UNBOUND,
              uint8_t cw = 1,
              int16_t pw = 1,
              WORD gi = 0xffff) : font_index (fi),
                                  cell_width (cw),
                                  pixel_width (pw),
                                  glyph_index (gi) { }

  bool is_defchar() const { return font_index == FONT_INDEX_DEFCHAR; }
  bool is_unbound() const { return font_index == FONT_INDEX_UNBOUND; }

  static glyph_info defchar () { return glyph_info (FONT_INDEX_DEFCHAR); }
  static glyph_info unbound () { return glyph_info (FONT_INDEX_UNBOUND); }
};

class glyph_info_array
{
private:
  struct HDC_deleter {
    typedef HDC pointer;
    void operator () (HDC hDc) { ::DeleteDC(hDc); }
  };

  mutable std::array<glyph_info, 0x10000> cache;

  std::unique_ptr<HDC, HDC_deleter> hdc;
  std::array<HFONT, FONT_MAX> fonts;
  SIZE cell_size;
  glyph_info get_impl (WCHAR) const;

public:
  glyph_info_array ();
  void reset (HDC, const std::array<HFONT, FONT_MAX> &, const SIZE &);
  void reset_dc (HDC);
  const glyph_info &get (WCHAR) const;
};

#endif /* UNICODE */

struct FontSetParam
{
  LOGFONT fs_logfont[FONT_MAX];
  int fs_use_backsl;
  int fs_line_spacing;
  int fs_recommend_size;
  int fs_size_pixel;
};

class FontSet
{
protected:
  void create_bitmap ();
  void paint_newline_bitmap (HDC);
  void paint_backsl_bitmap (HDC);
  void paint_tab_bitmap (HDC);
  void paint_fullspc_bitmap (HDC);
  void paint_halfspc_bitmap (HDC);
  void paint_sep_bitmap (HDC);
  void paint_blank (HDC);
  void paint_fold_bitmap (HDC);
  void save_params (const FontSetParam &);
  void load_params (FontSetParam &);

  static const UINT fs_lang_id[];
  static const TCHAR *const fs_regent[];
  struct fontface {const TCHAR *disp, *print; int charset;};
  static const fontface fs_default_face[];
public:
  enum
    {
      backsl,
      newline,
      htab,
      fullspc1,
      fullspc2,
      sep,
      blank,
      wblank1,
      wblank2,
      halfspc,
      bold_backsl,
      fold_sep0,
      fold_sep1,
      fold_mark_sep0,
      fold_mark_sep1,
      max_bitmap
    };

protected:
  FontObject fs_font[FONT_MAX];
  HBITMAP fs_hbm;
  SIZE fs_size;
  SIZE fs_cell;
  int fs_ascent;
  int fs_need_pad;
  int fs_line_spacing;
  int fs_use_backsl;
  int fs_line_width;
  int fs_recommend_size;
  int fs_size_pixel;

public:
  FontSet () : fs_hbm (0) {}
  ~FontSet () {if (fs_hbm) DeleteObject (fs_hbm);}
  int create (const FontSetParam &);
  void init ();
  const FontObject &font (int n) const {return fs_font[n];}
  const HBITMAP &hbm () const {return fs_hbm;}
  const SIZE &size () const {return fs_size;}
  const SIZE &cell () const {return fs_cell;}
  int need_pad_p () const {return fs_need_pad;}
  int use_backsl_p () const {return fs_use_backsl;}
  int line_width () const {return fs_line_width;}
  int line_spacing () const {return fs_line_spacing;}
  int recommend_size_p () const {return fs_recommend_size;}
  int size_pixel_p () const {return fs_size_pixel;}

#ifdef UNICODE
private:
  glyph_info_array fs_glyph_info_array;

public:
  void reset_glyph_info_cache ();
  const glyph_info &get_glyph_info (Char cc) const { return fs_glyph_info_array.get (cc); }
  int char_width (Char cc) const { return get_glyph_info (cc).cell_width; }
#endif /* UNICODE */

  static const TCHAR *regent (int n) {return fs_regent[n];}
  static const TCHAR *default_face (int n, int print)
    {return (!print || !fs_default_face[n].print
             ? fs_default_face[n].disp : fs_default_face[n].print);}
  static int default_charset (int n) {return fs_default_face[n].charset;}
  static UINT lang_id (int n) {return fs_lang_id[n];}
};

#endif /* _font_h_ */
