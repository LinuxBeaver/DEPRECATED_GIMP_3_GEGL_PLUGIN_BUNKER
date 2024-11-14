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
 * 2023 Beaver, Cake Text


--end of syntax
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_int (sizecontrol, _("Size control of the filter"), 0)
   description  (_("Size control of the filter"))
   value_range  (0, 10)


#define FIRST \
"  id=x cubism tile-size=4 seed=442 dst aux=[ ref=x median-blur  abyss-policy=none radius=2  alpha-percentile=0 cubism tile-size=3 id=x2  ] color-overlay value=#ecc7ff cubism tile-size=4 seed=442 gaussian-blur std-dev-x=5 std-dev-y=5 id=1 gaussian-blur  abyss-policy=none  clip-extent=false std-dev-x=6 std-dev-y=6 id=frostingcolor gimp:layer-mode layer-mode=overlay opacity=0.60 aux=[ ref=frostingcolor emboss ] xor aux=[ ref=1     ] opacity value=2 id=frostingcolor2 gimp:layer-mode layer-mode=overlay opacity=0.60 aux=[ ref=frostingcolor2 emboss  ] over aux=[ ref=x2  sg:innerglow value=black radius=5  opacity value=0.65 ] over aux=[ ref=x2  color-overlay value=#747474 opacity value=0.3 ] mean-curvature-blur  opacity value=2 softglow   glow-radius=50 softglow   glow-radius=40 median-blur  abyss-policy=none radius=0 dst-over aux=[ ref=x median-blur  abyss-policy=none radius=40 alpha-percentile=100 color-overlay value=#ee83ff  cubism tile-size=12 gaussian-blur  abyss-policy=none  clip-extent=false std-dev-x=2 std-dev-y=2  gaussian-blur std-dev-x=2 std-dev-y=2 abyss-policy=none clip-extent=none box-blur radius=1 id=cakelove plus aux=[ ref=cakelove emboss depth=24 elevation=25 ] ] saturation scale=1.25 median-blur  abyss-policy=none radius=0 id=hslx gimp:layer-mode layer-mode=hsl-color composite-mode=auto aux=[ ref=hslx  id=melaniescake median-blur  abyss-policy=none radius=0 over aux=[ ref=melaniescake sg:starfield saturation-distance=0.045999999999999999  value-distance=0.050999999999999997  seed=0  out-high=3.972  gamma=8.8529999999999998  std-dev=0.52500000000000002  saturation=1  color-to-alpha color=black opacity value=2.2 median-blur  abyss-policy=none radius= percentile=0 opacity value=2 ] gaussian-blur  abyss-policy=none  clip-extent=false  std-dev-x=1.5 std-dev-y=1.5 median-blur  abyss-policy=none radius=0 ] id=bottomcake  dst-over aux=[ ref=bottomcake id=bread dst-over aux=[ ref=bread color-overlay value=#ffc383   median-blur  abyss-policy=none radius=25 alpha-percentile=100 id=texture gimp:layer-mode layer-mode=softlight opacity=0.84 composite-mode=auto aux=[ ref=texture noise-rgb noise-rgb noise-rgb noise-reduction iterations=1 ] id=recolorit gimp:layer-mode layer-mode=hsl-color aux=[ ref=recolorit color-overlay value=#ab84b5 ] ] median-blur  abyss-policy=none radius=0  sg:extrusion   extrusion-x=0.493000000000000004  extrusion-y=-1.9709999999999998  length=0.016  depth=2 exposure=-0.97099999999999997  median-blur  abyss-policy=none radius=0 id=lnburn gimp:layer-mode layer-mode=linear-burn composite-space=rgb-perceptual  opacity=0.011 aux=[ ref=lnburn ]  ] median-blur  abyss-policy=none radius=0 id=bevit gimp:layer-mode layer-mode=softlight opacity=0.34 composite-mode=auto aux=[ ref=bevit sg:bevel hue-chroma lightness=14 ] bloom strength=9 sg:threshold-alpha  "\





#else

#define GEGL_OP_META
#define GEGL_OP_NAME     cake
#define GEGL_OP_C_SOURCE cake.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;


    GeglNode *input    = gegl_node_get_input_proxy (gegl, "input");
    GeglNode *output   = gegl_node_get_output_proxy (gegl, "output");

   GeglNode*syntax1    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", FIRST,
                                  NULL);


   GeglNode*sizecontrol    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "alpha-percentile", 100.0,
                                  NULL);


  gegl_node_link_many (input, sizecontrol, syntax1, output, NULL);


  gegl_operation_meta_redirect (operation, "sizecontrol",  sizecontrol, "radius");


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_class->threaded = FALSE;  gegl_operation_class_set_keys (operation_class,
    "name",        "sg:cake",
    "title",       _("Cake Text"),
    "reference-hash", "nblodvricasa48j3gj221g4ac",
    "description", _("Cake Text"),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Cake text..."),
    NULL);
}

#endif
