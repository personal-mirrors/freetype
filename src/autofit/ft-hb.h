#ifndef FT_HB_H
#define FT_HB_H

#include <hb.h>

hb_font_t * _hb_ft_font_create (FT_Face           ft_face,
                                hb_destroy_func_t destroy);

#endif
