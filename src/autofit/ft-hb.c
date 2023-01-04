#include <freetype/freetype.h>
#include <freetype/tttables.h>

#ifdef FT_CONFIG_OPTION_USE_HARFBUZZ

#include "ft-hb.h"

/* The following three functions are a more or less verbatim
 * copy of corresponding HarfBuzz code from hb-ft.cc
 */

static hb_blob_t *
_hb_ft_reference_table (hb_face_t *face, hb_tag_t tag, void *user_data)
{
  FT_Face ft_face = (FT_Face) user_data;
  FT_Byte *buffer;
  FT_ULong  length = 0;
  FT_Error error;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_Load_Sfnt_Table (ft_face, tag, 0, NULL, &length);
  if (error)
    return NULL;

  buffer = (FT_Byte *) malloc (length);
  if (!buffer)
    return NULL;

  error = FT_Load_Sfnt_Table (ft_face, tag, 0, buffer, &length);
  if (error)
  {
    free (buffer);
    return NULL;
  }

  return hb_blob_create ((const char *) buffer, length,
                         HB_MEMORY_MODE_WRITABLE,
                         buffer, free);
}

static hb_face_t *
_hb_ft_face_create (FT_Face           ft_face,
                    hb_destroy_func_t destroy)
{
  hb_face_t *face;

  if (!ft_face->stream->read) {
    hb_blob_t *blob;

    blob = hb_blob_create ((const char *) ft_face->stream->base,
                           (unsigned int) ft_face->stream->size,
                           HB_MEMORY_MODE_READONLY,
                           ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_index);
    hb_blob_destroy (blob);
  } else {
    face = hb_face_create_for_tables (_hb_ft_reference_table, ft_face, destroy);
  }

  hb_face_set_index (face, ft_face->face_index);
  hb_face_set_upem (face, ft_face->units_per_EM);

  return face;
}

hb_font_t *
_hb_ft_font_create (FT_Face           ft_face,
                    hb_destroy_func_t destroy)
{
  hb_font_t *font;
  hb_face_t *face;

  face = _hb_ft_face_create (ft_face, destroy);
  font = hb_font_create (face);
  hb_face_destroy (face);
  return font;
}

#endif
