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
 * 2023 Beaver, Charcoal Sketch 

Test this filter without installing by pasting this GEGL Syntax inside Gimp's GEGL Graph filter

motion-blur-linear length=12 angle=130

gimp:desaturate mode=lightness
cartoon pct-black=20 mask-radius=1.9
mean-curvature-blur iterations=2
opacity value=10
median-blur radius=0
levels in-high=0.45

end of syntax

NOTE - THIS IS A VERY SIMPLE FILTER TO STUDY. FUTURE GEGL DEVS WILL BENEFIT FROM STUDYING THIS CODE. THIS WAS INSPIRED BY FILTERS I SAW INSIDE IN ADOBE PHOTOSHOP CS5
COMPARED TO MOST OF THE GEGL PLUGINS I MAKE NOWADAYS THIS ONE HAS VERY SIMPLE NODE CONNECTIONS.
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (linelength, _("Sketch line length"), 22)
   description (_("Length of the sketch lines"))
   value_range (1.0, 40.0)
   ui_range    (1.0, 40.0)
   ui_gamma    (3.0)

property_double (lineangle, _("Sketch line angle"), 130.0)
    description (_("Sketch line angle"))
    value_range (-180.0, 180)
    ui_meta     ("unit", "degree")
    ui_meta     ("direction", "cw")

property_double (cartoonmr, _("Cartoon (mask radius)"), 1.9)
   description (_("gegl:cartoon's mask radius setting"))
   value_range (1.0, 4.0)
   ui_range    (1.0, 4.0)
   ui_gamma    (3.0)

property_int (smooth, _("Mean Curvature Smoothing"), 2)
   description (_("Mean Curvature blur for smoothing the sketch. High values will smudge the lines"))
   value_range (1, 5)
   ui_range    (1, 5)
   ui_gamma    (3.0)

property_double (paperlight, _("Levels (in high)"), 0.45)
   description (_("Levels - in high setting"))
   value_range (0.4, 1.0)
   ui_range    (0.4, 1.0)
   ui_gamma    (3.0)


property_double (whitelight, _("Levels (out low)"), 0.0)
   description (_("Levels - out low setting"))
   value_range    (0.0, 1.0)
   ui_range    (0.0, 0.4)
   ui_gamma    (3.0)

property_double (saturation, _("Colored pencil control"), 0.0)
   description (_("Saturate/Desaturate to turn on or off the colored pencil effect"))
   value_range    (0.0, 1.0)
   ui_range    (0.0, 1.0)





#else

#define GEGL_OP_META
#define GEGL_OP_NAME     charcoal_sketch
#define GEGL_OP_C_SOURCE charcoal_sketch.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *endfix, *mcb, *lmb, *white, *saturation, *cartoon, *lchcolor,  *light, *gray, *output;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


                              
#define fix \
" opacity value=10 median-blur radius=0 "\

  endfix = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", fix,
                                  NULL);
/*Opacity at 10 is meant to remove a transparent border gap created by linear motion blur and median blur at 0 is meant to fix a opacity glitch caused by gegl:opacity.*/
  mcb = gegl_node_new_child (gegl,
                                  "operation", "gegl:mean-curvature-blur", 
                                  NULL);

  lmb = gegl_node_new_child (gegl,
                                  "operation", "gegl:motion-blur-linear", 
                                  NULL);

  cartoon = gegl_node_new_child (gegl,
                                  "operation", "gegl:cartoon", "pct-black", 0.9,
                                  NULL);

  white = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", 
                                  NULL);

  saturation = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation", 
                                  NULL);

  light = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", 
                                  NULL);

  gray = gegl_node_new_child (gegl,
                                  "operation", "gimp:desaturate", 
                                  NULL);

  lchcolor = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 26, "blend-space", 3, "opacity", 1.0,
                                  NULL);
/*Layer-Mode 26 is blend mode LCh Color, and Blend space 3 is blend-space=lab (LAB), for some reason all the other blend spaces including the default (0) make LCh color look terrible.
In GEGL syntax this command is 
id=1 gimp:layer-mode layer-mode=lch-color blend-space=lab opacity=0.0 aux=[ ref=1 ]
it is meant to enable or disable the colored pencil effect.

IF THIS FILTER BREAKS IN THE FUTURE THIS IS LIKELY THE AREA TO LOOK

Dear future GEGL devs, if Gimp ever gets new blend modes this filter will be one number off (thus using the wrong blend mode) and this filter
will malfunction until manually updated to include the new number for lch color's blend mode, which will either be number 25 or 27. (likely 27 but try both)*/

  gegl_node_link_many (input,   white, lmb, gray, cartoon, mcb, endfix, light, lchcolor, saturation, output, NULL);
  gegl_node_connect (lchcolor, "aux", input, "output");


 gegl_operation_meta_redirect (operation, "linelength", lmb, "length"); 
 gegl_operation_meta_redirect (operation, "lineangle", lmb, "angle"); 
 gegl_operation_meta_redirect (operation, "cartoonmr", cartoon, "mask-radius"); 
 gegl_operation_meta_redirect (operation, "paperlight", light, "in-high"); 
 gegl_operation_meta_redirect (operation, "whitelight", white, "out-low"); 
 gegl_operation_meta_redirect (operation, "smooth", mcb, "iterations"); 
 gegl_operation_meta_redirect (operation, "saturation", saturation, "scale"); 

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "sg:charcoalsketch",
    "title",       _("Sketch Charcoal Style"),
    "reference-hash", "324324beavis2232323butthead323",
    "description", _("Make a charcoal style sketch that also includes a colored pencil option."),
    "gimp:menu-path", "<Image>/Filters/Artistic",
    "gimp:menu-label", _("Sketched Image"),
    NULL);
}

#endif
