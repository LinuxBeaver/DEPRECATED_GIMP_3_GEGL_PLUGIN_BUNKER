/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2024, beaver, template plugin

optionally put graph here

noise-solid x-size=6 y-size=10 seed=24
detail=0 tileable=true 




id=1 over aux=[ ref=1 emboss depth=95 elevation=120 opacity value=0.30 ]  gimp:layer-mode layer-mode=burn blend-space=rgb-perceptual opacity=0.67 aux=[ color value=#006bff] over aux=[ color value=color value=#006bff  opacity value=0.25 ] id=2 gimp:layer-mode layer-mode=lighten-only  aux=[ ref=2 median-blur radius=70 opacity value=0.4 ]  id=3 
 


color-dodge srgb=false aux=[ ref=3 opacity value=0.3 emboss depth=4 ]





end of syntax
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*Properties go here*/


property_enum(guichange, _("Options Displayed"),
    unfavorableoc, unfavorableoc2,
    OCEANSURFACE2_SHOW_DEFAULT)
  description(_("Change the GUI option"))


enum_start (unfavorableoc2)
enum_value   (OCEANSURFACE2_SHOW_DEFAULT, "default", N_("Favorable Sliders"))
enum_value   (OCEANSURFACE2_SHOW_UNFAVORABLE, "unfavorable", N_("Unfavorable sliders"))
  enum_end (unfavorableoc)

property_double (x_size, _("X Size for current waves"), 4.9)
    description (_("Horizontal current texture size"))
    value_range (4, 16.0)
    ui_range    (4, 16.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")
ui_meta ("visible", "guichange {unfavorable}")

property_double (y_size, _("Y Size for current waves"), 8.3)
    description (_("Vertical current texture size"))
    value_range (4, 16.0)
    ui_range    (4, 16.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")
ui_meta ("visible", "guichange {unfavorable}")

property_boolean (tile, _("Tile the Ocean's surface texture"), TRUE)
  description    (_("Tile the texture"))

property_seed   (seed, _("Random seed of current waves"), rand)
    description (_("The random seed for the noise function"))


property_double (elevation, _("Elevation of current waves"), 110.0)
    description (_("Emboss Elevation angle (degrees)"))
    value_range (80, 120)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth of current waves"), 29)
    description (_("Emboss depth setting"))
    value_range (15, 50)

property_double (navy, _("Dark Ocean"), 0)
    description (_("Option to make the ocean navy blue. This is Disabled in default."))
    value_range (0.0, 1.0)


property_double (hue, _("Color Rotation"), 0)
    description (_("Hue adjustment"))
    value_range (-180, 180)


property_double (lightness, _("Lightness"), 0)
    description (_("Lightness adjustment"))
    value_range (-30, 30)

property_double (saturation, _("Saturation"), 1)
    description (_("Saturation adjustment"))
    value_range (0, 3)
#else

#define GEGL_OP_META
#define GEGL_OP_NAME     ocean_surface_2
#define GEGL_OP_C_SOURCE ocean_surface_2.c

#include "gegl-op.h"

/*starred nodes go inside typedef struct */

typedef struct
{
 GeglNode *input;
 GeglNode *noisesolid;
 GeglNode *normal;
 GeglNode *emboss;
 GeglNode *opacity;
 GeglNode *string;
 GeglNode *opacity2;
 GeglNode *emboss2;
 GeglNode *hue;
 GeglNode *route2;
 GeglNode *route3;
 GeglNode *route4;
 GeglNode *normal2;
 GeglNode *opacity3;
 GeglNode *saturation;
 GeglNode *fix;
 GeglNode *string2;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);


  State *state = o->user_data = g_malloc0 (sizeof (State));


  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

 state->noisesolid = gegl_node_new_child (gegl, "operation", "gegl:noise-solid", "detail", 0, "tileable", TRUE,   NULL);

 state->normal = gegl_node_new_child (gegl, "operation", "gegl:over", NULL);

 state->emboss = gegl_node_new_child (gegl, "operation", "gegl:emboss", NULL);

 state->opacity = gegl_node_new_child (gegl, "operation", "gegl:opacity", NULL);

#define LG \
" id=1 over aux=[ ref=1 emboss depth=95 elevation=120 opacity value=0.30 ]  gimp:layer-mode layer-mode=burn blend-space=rgb-perceptual opacity=0.67 aux=[ color value=#006bff] over aux=[ color  value=#006bff  opacity value=0.25 ] id=2 gimp:layer-mode layer-mode=lighten-only  aux=[ ref=2 median-blur radius=70 opacity value=0.4 ]   "\

 state->string = gegl_node_new_child (gegl, "operation", "gegl:gegl", "string", LG, NULL);

 state->opacity2 = gegl_node_new_child (gegl, "operation", "gegl:opacity", "value", 0.3, NULL);

 state->emboss2 = gegl_node_new_child (gegl, "operation", "gegl:emboss", "depth", 4, NULL);

 state->hue = gegl_node_new_child (gegl, "operation", "gegl:hue-chroma",  NULL);

 state->saturation = gegl_node_new_child (gegl, "operation", "gegl:saturation",  NULL);

 state->route2 = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);

 state->route3 = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);

 state->route4 = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);

 state->opacity3 = gegl_node_new_child (gegl, "operation", "gegl:opacity",  NULL);

 state->normal2 = gegl_node_new_child (gegl, "operation", "gegl:over",  NULL);

 state->fix = gegl_node_new_child (gegl, "operation", "gegl:median-blur", "radius", 0, "abyss-policy", 0, NULL);

#define G2 \
" gimp:layer-mode layer-mode=overlay blend-space=rgb-perceptual  aux=[ color value=#000000 opacity value=0.9 ]    "\

 state->string2 = gegl_node_new_child (gegl, "operation", "gegl:gegl", "string", G2, NULL);

 gegl_operation_meta_redirect (operation, "tile", state->noisesolid,  "tileable");
 gegl_operation_meta_redirect (operation, "elevation", state->emboss,  "elevation");
 gegl_operation_meta_redirect (operation, "depth", state->emboss,  "depth");
 gegl_operation_meta_redirect (operation, "x-size", state->noisesolid,  "x-size");
 gegl_operation_meta_redirect (operation, "y-size", state->noisesolid,  "y-size");
 gegl_operation_meta_redirect (operation, "seed", state->noisesolid,  "seed");
 gegl_operation_meta_redirect (operation, "hue", state->hue,  "hue");
 gegl_operation_meta_redirect (operation, "lightness", state->hue,  "lightness");
 gegl_operation_meta_redirect (operation, "saturation", state->saturation,  "scale");
 gegl_operation_meta_redirect (operation, "navy", state->opacity3,  "value");
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  gegl_node_link_many (state->noisesolid, state->route2, state->normal, state->string,  state->fix, state->hue, state->saturation, state->route3, state->normal2,  state->output,  NULL);
  gegl_node_connect (state->normal, "aux", state->route4, "output");
  gegl_node_link_many (state->route2, state->emboss, state->route4,   NULL);
  gegl_node_connect (state->normal2, "aux", state->opacity3, "output");
  gegl_node_link_many (state->route3, state->string2, state->opacity3, NULL);
/*optional connect from and too is here
  gegl_node_connect (state->blendmode, "aux", state->lastnodeinlist, "output"); */

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:oceansurface-2",
    "title",       _("Ocean surface 2"),
    "reference-hash", "xfceisbasedwithmuteplayerclassic",
    "description", _("Render an Ocean's surface background"),
/*<Image>/Colors <Image>/Filters are top level menus in GIMP*/
    "gimp:menu-path", "<Image>/Filters/Render/Fun",
    "gimp:menu-label", _("Ocean Surface 2..."),
    NULL);
}

#endif
