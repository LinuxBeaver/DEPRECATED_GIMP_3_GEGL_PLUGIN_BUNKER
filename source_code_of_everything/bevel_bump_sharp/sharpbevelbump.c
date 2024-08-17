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
 * 2023 Beaver, Sharp Bevel Bump Map
 */

/* This plugin is very similar to my GEGL bevel (sg:bevel) plugin. 

This filter would ultimately benefit as being part of my GEGL bevel plugin (sg:bevel) and not as a stand alone.
During early development I (Beaver) tried to make this plugin part of the bevel plugin but it failed. Thus it is
a plugin of its own. This plugin would be very easy to add to GEGL Effects if it was part of sg:bevel and not its own plugin. 


 If you paste this syntax in Gimp's GEGL graph you can test this filter without installing it.

levels out-low=1.00
median-blur radius=3 alpha-percentile=100 id=0 src aux=[ ref=0 median-blur radius=0 distance-transform  color-to-alpha opacity-threshold=0.1 color=black emboss depth=4
]
levels in-high=1.3 in-low=0.33
bilateral-filter blur-radius=3*/


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

#define EMBEDDEDGRAPH \
" color-to-alpha opacity-threshold=0.1 color=black  "\
/* This is an embedded gegl graph for the plugin. It is calling color to alpha. This saves time so I don't have to embed a color*/

property_double (azimuth, _("Azimuth"), 67.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation"), 25.0)
    description (_("Elevation angle (degrees). This appears to rotate the brightest pixels."))
    value_range (7, 140)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth and or detail"), 24)
    description (_("Brings out depth and or detail of the bevel depending on the blend mode"))
    value_range (8, 100)

property_double (smooth, _("Smooth Roughness "), 2.5)
  description(_("Bilateral blur smoothes roughness  of the bevel but is resource intensive at high values."))
  value_range   (0.0, 10.0)
  ui_range      (0.0, 10.0)
  ui_gamma      (1.5)

property_enum (metric, _("Distance Map Setting"),
               GeglDistanceMetric, gegl_distance_metric, GEGL_DISTANCE_METRIC_CHEBYSHEV)
    description (_("Distance Map has three settings that alter the structure of the bevel. Chebyshev is the default; due to it being the best. But try the other two. "))


property_double (inhigh, _("Levels - in High"), 1.3)
    description (_("Levels in High Setting"))
    value_range (1.0, 1.5)
  ui_range      (1.0, 1.5)



property_int (blackbevel, _("Black Bevel/Image Bevel mode"), 1)
    description (_("This slider does two things. 1. It allows black and very dark bevels to work proper. 2. If there is an image overlay this slider will instruct the bevel to ignore the images texture. By default this slider is enabled. "))
    value_range (-1, 1)


/* In almost all cases ENUM list must be renamed. This seems to be an exception to the rule about ENUM list sharing the same name breaking GEGL plugins. It getting information from gegl-enums.c.
That might have something to do with this exception. I don't understand why this exception exist - but it does.*/

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     sharpbevelbump
#define GEGL_OP_C_SOURCE sharpbevelbump.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *emboss, *demandcolor, *idref, *replace, *dt, *bilateral, *opacity, *c2a, *median, *median2, *levels;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


 emboss   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

 opacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 0.60,
                                  NULL);
/* Its a bump map so putting the opacity at 60% makes sense so the blend is not very dark. If you edit the code and remove opacity (by making it value=1.00, you'll realize
there is too much black. 60& opacity is an attempt to make the black turn grayscalish. */

 demandcolor   = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", 
                                  NULL);
/* This is similar to slide up black in my bevel plugin, it forces the color of the bevel to stay bright as it does not work on black. */


 idref   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

replace = gegl_node_new_child (gegl, "operation", "gegl:src", NULL);

 levels   = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", "in-high", 1.3, "in-low", 0.33,
                                  NULL);

 dt   = gegl_node_new_child (gegl,
                                  "operation", "gegl:distance-transform", "metric", 2,
                                  NULL);

 bilateral   = gegl_node_new_child (gegl,
                                  "operation", "gegl:bilateral-filter", "edge-preservation", 0.0,
                                  NULL);


 c2a   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", EMBEDDEDGRAPH,
                                  NULL);

 median   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 3, "alpha-percentile", 100.0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

 median2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

  gegl_operation_meta_redirect (operation, "elevation", emboss, "elevation");
  gegl_operation_meta_redirect (operation, "depth", emboss, "depth");
  gegl_operation_meta_redirect (operation, "azimuth", emboss, "azimuth");
  gegl_operation_meta_redirect (operation, "metric", dt, "metric");
  gegl_operation_meta_redirect (operation, "smooth", bilateral, "blur-radius");
  gegl_operation_meta_redirect (operation, "blackbevel", demandcolor, "out-low");
  gegl_operation_meta_redirect (operation, "inhigh", levels, "in-high");

/* Most of the filters are here in the first row*/
    gegl_node_link_many (input, demandcolor, median, idref, replace, levels, bilateral, opacity, output, NULL);
/*Replace blend mode (gegl:src) is instructed to contain idref (gegl:nop) to emboss.
ifref is gegl:nop and it literally is id and ref in GEGL syntax. Notice how it is called twice before replace
and in replace.*/
    gegl_node_connect (replace, "aux", emboss, "output"); 
    gegl_node_link_many (idref, median2,  dt, c2a, emboss,  NULL); /*content inside the replace blend mode*/
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",               "sg:sharpbump",
    "title",              _("Sharp Bevel (to blend) "),
    "reference-hash",     "shfashaaa1af11fv55f309",
    "description", _("A Sharp Bevel bump map that is expected to be blended with Gimp blending options. Grain Merge, Multiply and Subtract have the best results but try everything including lowering the opacity."),
    "gimp:menu-path", "<Image>/Filters/Light and Shadow",
    "gimp:menu-label", _("Sharp Bevel Bump Map ..."),
    NULL);
}

#endif
