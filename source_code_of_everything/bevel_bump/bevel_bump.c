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
 * 2022 Beaver (GEGL Bevel) 
 */

/*GEGL Bevel is a stand alone plugin but it is also part of GEGL Effects. The stand alone version does more then the GEGL Effects implementation of it. */


/*
Graph here to test without installing. 

"normal bevel"
id=1 gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[color value=#ffffff  opacity value=0.1 ]
gaussian-blur std-dev-x=2 std-dev-y=2
emboss azimuth=44 depth=9
gimp:threshold-alpha value=0.03

"alt emboss graph"
id=1 gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[color value=#ffffff  opacity value=0.1 ]
gaussian-blur std-dev-x=2 std-dev-y=2
emboss azimuth=44 depth=9


"alt sharp bevel graph"
median-blur radius=3 alpha-percentile=100 id=0 src aux=[ ref=0 median-blur radius=0 distance-transform  color-to-alpha opacity-threshold=0.1 color=black emboss depth=4
]
levels in-high=1.3 in-low=0.33
bilateral-filter blur-radius=3*/


#define EMBEDDEDGRAPH \
" color-to-alpha opacity-threshold=0.1 color=black "\


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


property_double (gaus, _("Radius Normal Bevel"), 7.0)
  value_range (0.5, 40.0)
  ui_range (1.0, 9.0)
  ui_gamma (1.5)
    description (_("Make a normal bevel bump map using Gaussian Blur"))



property_int (box, _("Radius Sharper Bevel"), 0)
    description (_("Make a sharper bevel bump map using Box Blur - this is not the same as --sharp bevel mode--. Box Blur 0 is the default setting which disables this effect by default. Both normal bevel and sharp bevel sliders can be used together to hybridiz the bevel. "))
   value_range (0, 10.0)
   ui_range    (0, 9.0)
   ui_gamma   (1.5)




property_double (elevation, _("Depth Angle"), 45.0)
    description (_("Elevation angle (degrees)"))
    value_range (0, 180)
    ui_meta ("unit", "degree")


property_int (depth, _("Depth"), 40)
    description (_("Emboss depth -Brings out depth and detail of the bevel"))
    value_range (1, 100)

property_double (azimuth, _("Rotate Lighting"), 40.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")


property_double (slideupblack, _("Black Bevel/Image Bevel mode."), 0.00)
    description (_("This slider allows GEGL bevel to works on black Bevels; but the user must still manually select blend modes like Grain Merge and Hardlight that are known to work with very dark Bevels. This also allows bevel to be applied on image file overlays without conforming to an image's details."))
  value_range   (0.00, 0.999)
  ui_steps      (0.01, 0.50)


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     bevel_bump
#define GEGL_OP_C_SOURCE bevel_bump.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *blur;
  GeglNode *boxblur;
  GeglNode *emb;
  GeglNode *th;
  GeglNode *whitecolor;
  GeglNode *normallayer;
  GeglNode *slideupblack;
  GeglNode *nop;
  GeglNode *replace;
  GeglNode *output;
} State; 


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *boxblur, *blur, *emb, *th,  *nop,   *whitecolor, *normallayer, *slideupblack;
  GeglColor *white_color = gegl_color_new ("#ffffff");

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

   blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",  "clip-extent", FALSE, "abyss-policy", 0,
                                  NULL);

   boxblur = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);

 emb   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

  th = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha", "value", 0.100,
                                  NULL);


  whitecolor    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                   "value", white_color, NULL);


normallayer = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 28,  "composite-mode", 2, NULL);


 slideupblack   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);


 nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "elevation", emb, "elevation");
  gegl_operation_meta_redirect (operation, "depth", emb, "depth");
  gegl_operation_meta_redirect (operation, "azimuth", emb, "azimuth");
  gegl_operation_meta_redirect (operation, "box", boxblur, "radius");
  gegl_operation_meta_redirect (operation, "slideupblack", slideupblack, "value");




 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->blur = blur;
  state->boxblur = boxblur;
  state->emb = emb;
  state->th = th;
  state->whitecolor = whitecolor;
  state->normallayer = normallayer;
  state->slideupblack = slideupblack;
  state->nop = nop;
  state->output = output;
  o->user_data = state;
}

static void

update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;

    gegl_node_link_many (state->input, state->normallayer, state->blur, state->boxblur,  state->emb, state->th,  state->output, NULL);
    gegl_node_link_many (state->input, state->whitecolor, state->slideupblack,  NULL);
    gegl_node_connect (state->normallayer, "aux", state->slideupblack, "output");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "sg:bevel",
    "title",       _("Bevel (to blend) "),
    "reference-hash", "45ed5656a28a512570f0f25sb2ac",
    "description", _("User is expected to use blending options with this plugin. Works best with blend modes multiply, hardlight and grain merge with varying opacities. ."
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Bevel (to blend) ..."),
    NULL);
}

#endif
