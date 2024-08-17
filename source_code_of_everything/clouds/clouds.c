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
 * 2023 Beaver GEGL CLouds 
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*
GEGL GRAPH TO TEST WITHOUT INSTALLING. Feed this syntax to Gimp's GEGL graph and get a static preview of this plugin.

distance-transform src-in aux=[ noise-solid  detail=10 turbulent=22 seed=54 tileable=true ] 
hue-chroma lightness=-22
value-invert
id=1
hard-light aux=[ ref=1 color-overlay  value=#c3f2ff  ]
opacity value=4
id=2
hard-light aux=[ ref=2 distance-transform color-overlay  high-pass ]
 */


#define STRINGGIVEN \
" hue-chroma lightness=-22 value-invert id=1 hard-light aux=[ ref=1 color-overlay  value=#c3f2ff  ] opacity value=4 id=2 hard-light aux=[ ref=2 distance-transform color-overlay  high-pass ] levels out-low=0.1  "\


property_double (cloudsize, _("Scale clouds"), 4.0)
    description (_("Size of the clouds"))
    value_range (0.5, 14.0)
    ui_range    (1.5, 9.0)
    ui_meta     ("unit", "pixel-distance")
  ui_steps      (0.01, 0.50)


property_double (saturation, _("Saturation for the sky"), 0.6)
   description  (_("Saturation adjustment for the sky"))
  ui_range (0.5, 1.0)
   value_range  (0.5, 1.0)
  ui_steps      (0.01, 0.10)

property_double (hue, _("Hue Rotation (0 resets)"), 0)
   description  (_("Hue rotation for non-realistic clouds"))
  ui_range (-180, 180)
   value_range  (-180, 180)
  ui_steps      (0.01, 0.50)

property_seed (seed, _("Random seed of clouds"), rand)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     clouds
#define GEGL_OP_C_SOURCE clouds.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *noise, *srcin, *dt, *graph,  *clip, *hue, *saturation,  *repairgeglgraph;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  noise    = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-solid",
   "detail", 10,    "tileable", TRUE,
                                  NULL);


   graph   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string",  STRINGGIVEN,
                                  NULL);

   saturation   = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);


 dt   = gegl_node_new_child (gegl,
                                  "operation", "gegl:distance-transform",
                                  NULL);


 srcin   = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

 clip   = gegl_node_new_child (gegl,
                                  "operation", "gegl:rgb-clip",
                                  NULL);

 hue   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);


  repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       0,
                                         NULL);

  gegl_operation_meta_redirect (operation, "cloudsize", noise, "x-size");
  gegl_operation_meta_redirect (operation, "cloudsize", noise, "y-size");
  gegl_operation_meta_redirect (operation, "seed", noise, "seed");
  gegl_operation_meta_redirect (operation, "saturation", saturation, "scale");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");


  gegl_node_link_many (input, dt, srcin, graph, clip,  saturation, hue, repairgeglgraph, output, NULL);
  gegl_node_connect (srcin, "aux", noise, "output");


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "sg:clouds",
    "title",       _("Clouds"),
    "reference-hash", "beavb5ecl56oau1dgxsfge77g0laefe2g4f1b2ac",
    "description", _("GEGL renders a sky of clouds. "),
    NULL);
}

#endif
