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
 * 
 * 2023 Beaver (GEGL Aura)
 */

/*
Graph of GEGL Aura to test without installing
 
aura radius=24 seed=13   gradius=2 opacity=1.3 tile-size=10  
mosaic tile-neatness=0 tile-type=triangles tile-size=14 seed=33 light-color=#00000000  joints-color=#00000000  tile-spacing=1
opacity value=1.5
oilify
gaussian-blur std-dev-x=7 std-dev-y=7
color-overlay value=#ffc400
 */




#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES




property_color (color, _("Color"), "#00ff15")

property_double (radius, _("Lens Blur to Glow"), 11.0)
  description (_("Blur radius"))
  value_range (0.0, G_MAXDOUBLE)
  ui_range    (0.0, 50.0)
  ui_gamma    (2.0)
  ui_meta     ("unit", "pixel-distance")



property_double (gradius, _("Gaussian Blur to Glow"), 0.0)
  description (_("Blur radius"))
  value_range (0.0, G_MAXDOUBLE)
  ui_range    (0.0, 50.0)
  ui_gamma    (2.0)
  ui_meta     ("unit", "pixel-distance")


property_double (opacity, _("Hyper Opacity"), 1.0)
  value_range   (1.0, 4.0)
  ui_steps      (1.0, 4.0)


property_double (tile_size, _("Aura Size"), 3.8)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (3.0, 16.0)
    ui_meta     ("unit", "pixel-distance")


property_double (tile_saturation, _("Gaps in Aura"), 5.4)
    description (_("Expand tiles by this amount"))
    value_range (3.0, 8.0)

property_int    (mask_radius, _("Internal Oilify filter for Aura"), 4)
    description (_("Radius of circle around pixel, can also be scaled per pixel by a buffer on the aux pad."))
    value_range (1, 7)
    ui_range (1, 7)
    ui_meta     ("unit", "pixel-distance")


property_seed (seed, _("Random seed for Aura"), rand)




/*  GEGL does not allow multiple "rand" commands to exist in this template but renaming it to random or perhaps anything seems to solve the issue. */




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     outerglow
#define GEGL_OP_C_SOURCE outerglow.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *color;
  GeglNode *opacity;
  GeglNode *hopacity;
  GeglNode *xor;
  GeglNode *oilify;
  GeglNode *cubism;
  GeglNode *lblur;
  GeglNode *nop;
  GeglNode *gblur;
  GeglNode *fixgraph;
  GeglNode *output;
} State; 



static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *color, *oilify, *cubism, *xor, *fixgraph, *gblur, *lblur, *nop, *hopacity,  *opacity;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  hopacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 1.0,
                                  NULL);



  opacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 1.1,
                                  NULL);


  xor   = gegl_node_new_child (gegl,
                                  "operation", "gegl:xor",
                                  NULL);

  oilify   = gegl_node_new_child (gegl,
                                  "operation", "gegl:oilify",
                                  NULL);

  cubism   = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);

  lblur   = gegl_node_new_child (gegl,
                                  "operation", "gegl:lens-blur",
                                  NULL);

  nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  gblur    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  fixgraph = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);

/* Median Blur at 0 repairs a GEGL Graph from Opacity bug. This is for Gimp's non-destructive future */

  gegl_operation_meta_redirect (operation, "color", color, "value");
  gegl_operation_meta_redirect (operation, "radius", lblur, "radius");
  gegl_operation_meta_redirect (operation, "opacity", opacity, "value");
  gegl_operation_meta_redirect (operation, "tile_size", cubism, "tile-size");
  gegl_operation_meta_redirect (operation, "tile_saturation", cubism, "tile-saturation");
  gegl_operation_meta_redirect (operation, "mask_radius", oilify, "mask-radius");
  gegl_operation_meta_redirect (operation, "seed", cubism, "seed");
  gegl_operation_meta_redirect (operation, "gradius", gblur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gradius", gblur, "std-dev-y");

 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->nop = nop;
  state->cubism = cubism;
  state->oilify = oilify;
  state->lblur = lblur;
  state->gblur = gblur;
  state->xor = xor;
  state->color = color;
  state->hopacity = hopacity;
  state->opacity = opacity;
  state->fixgraph = fixgraph;
  state->output = output;
  o->user_data = state;
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

    gegl_node_link_many (state->input, state->hopacity, state->nop, state->cubism, state->oilify, state->lblur, state->gblur, state->xor, state->color,  state->opacity, state->fixgraph, state->output, NULL);
      gegl_node_connect (state->xor, "aux", state->nop, "output");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;


  operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);


  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "sg:aura",
    "title",       _("Aura "),
    "reference-hash", "3oudo6vg25ara040vxn3vv5sb2a",
    "description", _("Generate both an Outer Glow and Inner Glow aura-ish effect. Apply filter on duplicate layer above transparent subject/object. Filter is intended to be used with Gimp blend modes HSV Hue, HSL Color and Grain Merge. Filter benefits from the Union Composite Mode when used in transparent backgrounds."
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Aura ..."),
    NULL);
}
/* GEGL cannot replicate union with the master Gimp GEGL blend modes that all GEGL filters in Gimp have. Users have to manually change the layer space to union. */
#endif
