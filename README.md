# GIMP 3 has serious problems with many of my plugins - this is the safe repo for GIMP 2.99/GIMP 3 users

## 52 GEGL plugins of mine compatible with GIMP 2.99/3 



### As of now GIMP 3 plugins are not compatible with GIMP 2.10 plugins (the normal repos)

Everyone using GIMP 2.99.18+/GIMP 3 that wishes to use my GEGL GIMP plugins must only use this repo and not any other repo on my Github. This is because of a known bug. Mixing these plugins with other normal repos will cause a crash on close in GIMP 3.  Meaning if a tab or GIMP's main window closes a crash may happen and it happens because the normal GEGL plugins have features that GIMP 2.99.19 is not compatible with. This repo removed those features


## How to install

Location to put binaries 

**Windows**

 C:\Users\(USERNAME)\AppData\Local\gegl-0.4\plug-ins
 
 **Linux** 

 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins

**Linux (Flatpak includes Chromebook)**

 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

then restart GIMP and go to GEGL operation to find the new plugins or if on 2.99.19 



## Compiling and Installing

You can run the build_plugin_name_linux/windows to easy compile. You even have an option to compile everything.
Below is an explanation how to compile ultra manually; though it is highly recommended you use the bash script.

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```


### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

## This is a downgrade because GIMP 3 broke stuff that works in GIMP 2.10

### Below is a list of what was lost

1. Over 17 plugins were removed indefinitely because they break GIMP 2.99.19 (pie divider, spheres, cutout, frame, text style collection (which contains dozens of plugins in one place), wood texture, align, video dedegradtion mod, bokeh, gum balls and more are gone and will not return until GIMP's team fixes things' .
   
2. All plugins that use image uploads had their image upload ability removed
3. GEGL Effects lost its gradient, shiny text and color blend mode picker
4. Glow Stick lost its ability to call gegl:bloom
5. Sparkle lost its dropshadow and had its graph rewritten
6. Rock Text 2 had a domain smooth removed and replaced with mean-curvature-blur; a virtually unnoticable change but it had to happen because gegl:crop left.

.
