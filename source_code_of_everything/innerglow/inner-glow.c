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
 * 2022 Beaver (GEGL inner glow)
 */

/*GEGL Inner Glow is a stand alone plugin but it is also part of GEGL Effects. The stand alone version does more then the GEGL Effects implementation of it. */

/*
Recreation of Inner Glow's GEGL Graph. May not be 100% accurate but you can test it without installing this way.

id=0 src-in aux=[ ref=0 id=1 dst-atop   aux=[  ref=1 distance-transform  ] xor srgb=true     aux=[ ref=1 ] color-overlay value=#000000 dropshadow x=0 y=0 grow-radius=5 color-overlay value=#ff0000  ]
median-blur radius=3 alpha-percentile=94 

2024 Beaver Inner Glow 2 without gegl crop

 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES
/*This is a list of the default inner glow plugin and its modern successors.*/


/*This is GEGL syntax I wrote to invert transparency. Its operation is gegl:gegl and it represents a GEGL Graph inside a GEGL Graph.
please know distance transform is only here for filling the entire image with content. i could have used other
nodes like "solid-noise, cell-noise, ect... but distance transform was the fastest. gegl:color was faster but broke the graph */
#define GRAPHUSEDBYINNERGLOW \
"   id=1 dst-atop   aux=[  ref=1 distance-transform  ] xor srgb=true aux=[ ref=1 ] color-overlay value=#000000  "\



/*On June 24 2023 I finally figured out how to bake in GEGL Graphs*/

/*This is an enum list for the base shape of inner stroke/shadow/glow. It should not be renamed.*/
enum_start (gegl_dropshadow_grow_shapeigcrop)
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_SQUAREig,  "squareig",  N_("Square"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_CIRCLEig,  "circleig",  N_("Circle"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_DIAMONDig, "diamondig", N_("Diamond"))
enum_end (GeglDropshadowGrowShapeigcrop)


property_enum   (grow_shape, _("Grow shape"),
                 GeglDropshadowGrowShapeigcrop, gegl_dropshadow_grow_shapeigcrop,
                 GEGL_DROPSHADOW_GROW_SHAPE_CIRCLEig)
  description   (_("The shape to expand or contract the shadow in"))

property_double (x, _("X"), 0.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-20.0, 20.0)
  value_range   (-20.0, 20.0)
  ui_steps      (1, 2)
  ui_meta       ("axis", "x")

property_double (y, _("Y"), 0.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-20.0, 20.0)
  value_range   (-20.0, 20.0)
  ui_steps      (1, 2)
  ui_meta       ("axis", "y")

property_double (radius, _("Blur radius"), 9)
  value_range   (0.0, 60.0)
  ui_range      (0.0, 40.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_int (grow_radius, _("Grow radius"), 4)
  value_range   (1, 40)
  ui_range      (1, 30)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring. When using inverted modes this setting needs to be at a reasonable degree for it's effect to be noticable."))



property_double (opacity, _("Opacity"), 1.2)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)


property_color (value2, _("Color"), "#fbff00")
    description (_("The color to paint over the input"))
    ui_meta     ("role", "color-primary")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     inner_glow
#define GEGL_OP_C_SOURCE inner-glow.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *it;
 GeglNode *src;
 GeglNode *shadow;
 GeglNode *color2;
 GeglNode *medianset;
 GeglNode *in2;
 GeglNode *c2arevision;
 GeglNode *idref;
 GeglNode *atop;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));
  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

  state->it    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", GRAPHUSEDBYINNERGLOW,
                                  NULL);

/*On June 24 2023 I finally figured out how to bake in GEGL Graphs
At Nov 20 2023 I learned that two nodes can call the same graph, also this graph inverts transparency.*/

  state->in2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);



  state->src    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src",
                                  NULL);



  state->atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);


  state->idref    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);



  state->shadow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);


  state->color2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);




  state->medianset     = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       1,
                                         "alpha-percentile", 100.0,
                                         NULL);

#define c2afeb2024 \
"    color-to-alpha color=black   transparency-threshold=0 opacity-threshold=1  "\

  state->c2arevision    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", c2afeb2024,
                                  NULL);



gegl_operation_meta_redirect (operation, "grow_radius",  state->shadow, "grow-radius");
gegl_operation_meta_redirect (operation, "radius",  state->shadow, "radius");
gegl_operation_meta_redirect (operation, "opacity",  state->shadow, "opacity");
gegl_operation_meta_redirect (operation, "grow-shape",  state->shadow, "grow-shape");
gegl_operation_meta_redirect (operation, "value2",  state->color2, "value");
gegl_operation_meta_redirect (operation, "x",  state->shadow, "x");
gegl_operation_meta_redirect (operation, "y",  state->shadow, "y");



}

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

 gegl_node_link_many (state->input, state->medianset,  state->idref, state->in2,  state->output, NULL);
 gegl_node_link_many  (state->idref, state->it,  state->shadow, state->color2,   NULL);
 gegl_node_connect (state->in2, "aux", state->color2, "output");

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
    "name",        "sg:innerglow",
    "title",       _("Inner Glow (to blend) "),
    "reference-hash", "g3do6aaoo1100g0fjf25sb2ac",
    "description", _("GEGL does an inner glow effect. For this filter to work you need to use the 'Normal' or other blending options. Or a duplicate layer on top. "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Inner Glow (to blend) ..."),
    NULL);
}

#endif
