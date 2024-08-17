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
 * 2023 Beaver, Ringed Bevel
You can test plugin without installing by pasting this syntax into Gimp's GEGL graph (This graph requires other plugins 'ssg' and 'custom-bevel' this filter ships with those two plugins)

levels out-low=0.00
id=1
dst-out aux=[ ref=1

median-blur radius=5 alpha-percentile=0
sg:ssg radius=0 stroke=7
sg:ssg radius=0 stroke=7
sg:ssg radius=0 stroke=7
sg:ssg radius=0 stroke=7

median-blur radius=0
 ]
sg:custom-bevel depth=4 gaus=1
dst-over aux=[ ref=1 sg:custom-bevel depth=1 gaus=1  ]

--end of syntax--

Fun fact about this plugin

This plugin is meant to be a GEGL counterpart to a script fu named LAYERFX's "bevel and emboss" with its inner bevel on double ring mode.
It isn't as simple as just making this part of my bevel filter, as it has a very different algorithm. Script fu can use selection tools in Gimp
that GEGL can't. That is why GEGL makes this kind of effect radically different, but hey at least it is possible to mimick the double ring inner bevel. :)


This filter is not a carbon copy of script fu "bevel and emboss" double ring. It has its own unique spin. But that is directly the inspiration.
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



property_int  (bevelsize, _("Size of the bevel"), 3)
  value_range (1, 9)
  ui_range    (1, 9)
  ui_meta     ("unit", "pixel-distance")
  description (_("Median radius to control the size of the bevel"))


property_int  (ringsize, _("Ring size"), 6)
  value_range (4, 9)
  ui_range    (4, 9)
  ui_meta     ("unit", "pixel-distance")
  description (_("Control the size of the rings. This also creates gaps when bottom bevel's opacity is low."))


property_double (azimuth, _("Azimuth of the bevel"), 67.0)
    description (_("Light angle (degrees)"))
    value_range (20, 90)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation of the bevel"), 35.0)
    description (_("Elevation angle (degrees). This appears to rotate the brightest pixels."))
    value_range (7, 70)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth and or detail of the bevel"), 15)
    description (_("Brings out depth and or detail of the bevel depending on the blend mode"))
    value_range (4, 65)

enum_start (under_bevel_blend_mode)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "Hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "ColorDodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN,      "Lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "Addition",
              N_("Addition"))
enum_end (underbevelblendmode)

property_double (bottombevelopacity, _("Opacity of the bottom bevel."), 1.0)
    description (_("Opacity of the bottom bevel. 0 disables it entirely"))
    value_range (0.0, 1.0)


property_enum (blendbevel, _("Under bevel blend mode"),
    underbevelblendmode, under_bevel_blend_mode,
    GEGL_BLEND_MODE_TYPE_HARDLIGHT)
  description (_("Blend mode of the bevel under the rings"))


enum_start (ring_bevel_blend_mode)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTX, "Hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYX,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEX,      "ColorDodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUSX,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKENX,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTENX,      "Lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYX,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEX,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTX,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONX,      "Addition",
              N_("Addition"))
enum_end (ringbevelblendmode)

property_enum (blendrings, _("Ring bevel blend mode"),
    ringbevelblendmode, ring_bevel_blend_mode,
    GEGL_BLEND_MODE_TYPE_HARDLIGHTX)
  description (_("Blend mode of the ring bevel, but it also appears to effect the bevel below it."))



enum_start (shape_of_ring)
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_DIAMOND, "diamond", N_("Diamond"))
enum_end (shapeofring)

property_enum (ringshape, _("Shape of ring"),
    shapeofring, shape_of_ring,
               GEGL_MEDIAN_BLUR_NEIGHBORHOOD_CIRCLE)
  description (_("Shape of the Rings that internally uses Median Blur's neighborhood setting (square, circle, diamond)"))


property_double (darkbevelslider, _("Dark color bevel fix"), 0.0)
    description (_("Elevation "))
    value_range (0.0, 0.12)
    ui_meta ("unit", "degree")
  description (_("Slide up if the Bevel is a dark color and the dark color will conform to the bevel. If not the dark color will not look proper. If this slider is up on a light color then it will modify it in a potential unwanted way."))




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     ringbevel
#define GEGL_OP_C_SOURCE ringbevel.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *levels;
 GeglNode *erase;
 GeglNode *s1;
 GeglNode *s2;
 GeglNode *s3;
 GeglNode *s4;
 GeglNode *bevel;
 GeglNode *behind;
 GeglNode *median;
 GeglNode *mediandictator;
 GeglNode *bevel2;
 GeglNode *idref;
 GeglNode *fix;
 GeglNode *image;
 GeglNode *multiply;
 GeglNode *bevelopacity;
 GeglNode *output;
}State;


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));


  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

  state->image = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->bevelopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  state->multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  state->bevel = gegl_node_new_child (gegl,
                                  "operation", "sg:custom-bevel", "depth", 4, "gaus", 1.0,
                                  NULL);


  state->bevel2 = gegl_node_new_child (gegl,
                                  "operation", "sg:custom-bevel", "depth", 1, "gaus", 1.0,
                                  NULL);

  state->mediandictator = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "alpha-percentile", 100.0,
                                  NULL);

  state->erase = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-out",
                                  NULL);


  state->behind = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over",
                                  NULL);

  state->idref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->fix = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);

  state->levels = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels",
                                  NULL);

  state->s1 = gegl_node_new_child (gegl,
                                  "operation", "sg:ssg", "radius", 0, "stroke", 7,
                                  NULL);

  state->s2 = gegl_node_new_child (gegl,
                                  "operation", "sg:ssg", "radius", 0, "stroke", 7,
                                  NULL);

  state->s3 = gegl_node_new_child (gegl,
                                  "operation", "sg:ssg", "radius", 0, "stroke", 7,
                                  NULL);

  state->s4 = gegl_node_new_child (gegl,
                                  "operation", "sg:ssg", "radius", 0, "stroke", 7,
                                  NULL);


  state->median = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 5, "alpha-percentile", 0.0,
                                  NULL);


/*This is a median blur's radius being called to make the bevel larger, I can't use custom bevel's size because that doesn't work in enlarging both custom bevels*/
    gegl_operation_meta_redirect (operation, "bevelsize", state->mediandictator, "radius");

/*These list the azimuth, elevation and depth of my custom bevel plugin.*/
    gegl_operation_meta_redirect (operation, "azimuth", state->bevel, "azimuth");
    gegl_operation_meta_redirect (operation, "elevation", state->bevel, "elevation");
    gegl_operation_meta_redirect (operation, "depth", state->bevel, "depth");

/*This controls the size of ALL ssg nodes*/
    gegl_operation_meta_redirect (operation, "ringsize", state->s1, "stroke");
    gegl_operation_meta_redirect (operation, "ringsize", state->s2, "stroke");
    gegl_operation_meta_redirect (operation, "ringsize", state->s3, "stroke");
    gegl_operation_meta_redirect (operation, "ringsize", state->s4, "stroke");

/*This controls the blend mode of the bevel and the rings*/
    gegl_operation_meta_redirect (operation, "blendrings", state->bevel, "blendmode");
    gegl_operation_meta_redirect (operation, "blendbevel", state->bevel2, "blendmode");


/*This controls the shape of the rings. It is sg:ssg grow-shape=square,circle,diamond*/
    gegl_operation_meta_redirect (operation, "ringshape", state->s1, "grow-shape");
    gegl_operation_meta_redirect (operation, "ringshape", state->s2, "grow-shape");
    gegl_operation_meta_redirect (operation, "ringshape", state->s3, "grow-shape");
    gegl_operation_meta_redirect (operation, "ringshape", state->s4, "grow-shape");

/*Slide up if Bevel color is dark. This allows the bevel to work on dark colors*/
    gegl_operation_meta_redirect (operation, "darkbevelslider", state->levels, "out-low");




/*Disable bottom bevel (bevel 2)*/
    gegl_operation_meta_redirect (operation, "bottombevelopacity", state->bevelopacity, "value");



}

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


  gegl_node_link_many (state->input, state->mediandictator, state->levels, state->idref, state->erase,   state->bevel, state->behind,  state->multiply, state->output, NULL);
  gegl_node_link_many (state->idref, state->median, state->s1, state->s2, state->s3, state->s4,   state->fix, NULL);
  gegl_node_connect (state->erase, "aux", state->fix, "output");
  gegl_node_link_many (state->idref, state->bevel2, state->bevelopacity,   NULL);
  gegl_node_connect (state->behind, "aux", state->bevelopacity, "output");
  gegl_node_connect (state->multiply, "aux", state->image, "output");


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
    "name",        "sg:ringbevel",
    "title",       _("Ring Bevel "),
    "reference-hash", "r34jghavetheringrightherejfj329",
    "description", _("Make a Ringed Bevel effect"),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Ringed Bevel ..."),
    NULL);
}

#endif
