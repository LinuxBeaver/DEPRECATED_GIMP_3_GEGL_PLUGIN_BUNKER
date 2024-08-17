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
 * 2024 Beaver (GEGL Outline Deluxe, Intended for CMYK Student's build to make outline bevels easier to chain in 'layer effects'. )
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES
/*
Recreation of this plugin's graph


color-overlay value=white
ssg stroke=4
custom-bevel blendmode=Plus
 */


enum_start (gegl_stroke_grow_shapeoutline)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE2,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE2,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND2, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeoutline)

property_enum   (grow_shape, _("Grow shape"),
                 GeglstrokeGrowShapeoutline, gegl_stroke_grow_shapeoutline,
                 GEGL_stroke_GROW_SHAPE_CIRCLE2)
  description   (_("The shape to expand or contract the stroke in"))

property_color (color, _("Color Overlay"), "#ffffff")


property_int (stroke, _("Grow radius of Outline"), 9)
  value_range   (0, 100)
  ui_range      (0, 50)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the outline."))

property_int  (radius, _("Make the Outline go inward or outward"), 1)
  value_range (-250, 10)
  ui_range    (0, 2)
  description (_("Instruct the outline to go inward or outward."))

property_double (opacity, _("Opacity of Outline"), 2)
  value_range   (1.0, 4.0)
  ui_steps      (1.0, 4.0)

enum_start (f_bevel_outline)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "colordodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS,      "plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN,      "darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN,      "lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY,      "overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "grainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT,      "softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "addition",
              N_("Addition"))
enum_end (fbeveloutline)

property_enum (bevelblendmode, _("Blend Mode of Internal Emboss"),
    fbeveloutline, f_bevel_outline,
    GEGL_BLEND_MODE_TYPE_HARDLIGHT)


property_double (bevelazimuth, _("Azimuth of Bevel"), 67.0)
    description (_("Light angle (degrees)"))
    value_range (30, 90)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")


property_double (bevelelevation, _("Elevation of Bevel"), 25.0)
    description (_("Elevation angle (degrees)"))
    value_range (7, 90)
    ui_meta ("unit", "degree")

property_double (bevelgaus, _("Internal Gaussian Blur of for a normal bevel"), 2)
   description (_("Makes a normal bevel by calling an internal gaussian blur."))
   value_range (0.0, 9.0)


property_int (bevelbox, _("Internal Box Blur for a box bevel"), 3)
   description(_("Makes a box bevel which is somewhat different by calling an internal box blur"))
   value_range (0, 10)
   ui_range    (0, 10)
   ui_gamma   (1.5)



property_double (x, _("Horizontal position of Outline"), 0.0)
  description   (_("Horizontal outline offset"))
  ui_range      (-80, 80)
  ui_steps      (1, 5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Vertical position of Outline"), 0.0)
  description   (_("Vertical outline offset"))
  ui_range      (-80, 80)
  ui_steps      (1, 5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     outlinedeluxe
#define GEGL_OP_C_SOURCE outlinedeluxe.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *ssg;
 GeglNode *custombevel;
 GeglNode *multiply;
 GeglNode *dst;
 GeglNode *move;
 GeglNode *output;
}State;


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));


  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");


 state->ssg   = gegl_node_new_child (gegl,
                                  "operation", "sg:ssg",
                                  NULL);


 state->move   = gegl_node_new_child (gegl,
                                  "operation", "gegl:translate",
                                  NULL);



 state->custombevel   = gegl_node_new_child (gegl,
                                  "operation", "sg:custom-bevel",
                                  NULL);

 state->multiply   = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);


/*Fun fact. DST is the invisible blend mode in GEGL. It contains effects like they are in a hidden "layer group" and calls them later with ref from the (id ref) system*/
 state->dst   = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

  gegl_operation_meta_redirect (operation, "color",  state->ssg, "colorssg");
  gegl_operation_meta_redirect (operation, "opacity",  state->ssg, "opacityssg");
  gegl_operation_meta_redirect (operation, "stroke",  state->ssg, "stroke");
  gegl_operation_meta_redirect (operation, "x",  state->move, "x");
  gegl_operation_meta_redirect (operation, "y",  state->move, "y");
  gegl_operation_meta_redirect (operation, "grow_shape",  state->ssg, "grow_shape");
  gegl_operation_meta_redirect (operation, "radius",  state->ssg, "radius");
  gegl_operation_meta_redirect (operation, "bevelbox",  state->custombevel, "box");
  gegl_operation_meta_redirect (operation, "bevelgaus",  state->custombevel, "gaus");
  gegl_operation_meta_redirect (operation, "bevelelevation",  state->custombevel, "elevation");
  gegl_operation_meta_redirect (operation, "bevelazimuth",  state->custombevel, "azimuth");
  gegl_operation_meta_redirect (operation, "bevelblendmode",  state->custombevel, "blendmode");


} 

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


  gegl_node_link_many ( state->input, state->dst, state->ssg,  state->custombevel, state->move,  state->output, NULL);


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
    "name",        "sg:outlinebevel",
    "title",       _("Outline Bevel"),
    "reference-hash", "majorasmaskn642",
    "description", _("Make an outline with a bevel"
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Outline Bevel..."),
    NULL);
}

#endif
