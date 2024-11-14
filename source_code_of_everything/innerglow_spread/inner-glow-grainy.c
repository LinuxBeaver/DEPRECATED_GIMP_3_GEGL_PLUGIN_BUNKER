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


property_double (noise, _("Increase Noise"), 60.0)
    value_range (10.0, 100.0)


property_double (noiserepeat, _("Repetition of noise"), 8.0)
    value_range (5.0, 100.0)

property_seed (seed, _("Noise seed"), rand)


/*On June 24 2023 I finally figured out how to bake in GEGL Graphs*/

/*This is an enum list for the base shape of inner stroke/shadow/glow. It should not be renamed.*/
enum_start (gegl_dropshadow_grow_shapeigcropgrainy)
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_SQUAREig,  "squareig",  N_("Square"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_CIRCLEig,  "circleig",  N_("Circle"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_DIAMONDig, "diamondig", N_("Diamond"))
enum_end (GeglDropshadowGrowShapeigcropgrainy)


property_enum   (grow_shape, _("Grow shape"),
                 GeglDropshadowGrowShapeigcropgrainy, gegl_dropshadow_grow_shapeigcropgrainy,
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


property_color (value, _("Color"), "#fbff00")
    description (_("The color to paint over the input"))
    ui_meta     ("role", "color-primary")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     inner_glow_grainy
#define GEGL_OP_C_SOURCE inner-glow-grainy.c

#include "gegl-op.h"


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;



           GeglNode*input    = gegl_node_get_input_proxy (gegl, "input");
           GeglNode*output   = gegl_node_get_output_proxy (gegl, "output");

           GeglNode*it    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", GRAPHUSEDBYINNERGLOW,
                                  NULL);

/*On June 24 2023 I finally figured out how to bake in GEGL Graphs
At Nov 20 2023 I learned that two nodes can call the same graph, also this graph inverts transparency.*/

           GeglNode*in2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);


           GeglNode*idref    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


           GeglNode*pick    = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-pick",
                                  NULL);

           GeglNode*shadow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);


           GeglNode*color2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

           GeglNode*medianset     = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       1,
                                         "alpha-percentile", 100.0,
                                         NULL);


gegl_operation_meta_redirect (operation, "grow_radius",  shadow, "grow-radius");
gegl_operation_meta_redirect (operation, "radius",  shadow, "radius");
gegl_operation_meta_redirect (operation, "opacity",  shadow, "opacity");
gegl_operation_meta_redirect (operation, "grow-shape",  shadow, "grow-shape");
gegl_operation_meta_redirect (operation, "value",  color2, "value");
gegl_operation_meta_redirect (operation, "x",  shadow, "x");
gegl_operation_meta_redirect (operation, "y",  shadow, "y");
gegl_operation_meta_redirect (operation, "noise", pick, "pct-random");
gegl_operation_meta_redirect (operation, "noiserepeat", pick, "repeat");
gegl_operation_meta_redirect (operation, "seed", pick, "seed");

 gegl_node_link_many (input, medianset, idref, in2,   output, NULL);
 gegl_node_link_many  (idref, it,  shadow, pick,   color2,   NULL);
 gegl_node_connect (in2, "aux", color2, "output");

  }

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_class->threaded = FALSE;  gegl_operation_class_set_keys (operation_class,
    "name",        "sg:innerglow-grainy",
    "title",       _("Inner Glow grain style (to blend) "),
    "reference-hash", "g3do6aaoo1100g0fjf25sb2ac299",
    "description", _("GEGL does an inner glow effect. For this filter to work you need to use the 'Normal' or other blending options. Or a duplicate layer on top. Special build to not crash GIMP 2.99.19 "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling/InnerGlows",
    "gimp:menu-label", _("Inner Glow grain style (to blend)..."),
    NULL);
}

#endif
