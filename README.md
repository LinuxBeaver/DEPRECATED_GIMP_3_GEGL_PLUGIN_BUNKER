# GIMP 3 has serious problems with many of my plugins - this is the safe repo for GIMP 2.99/GIMP 3 users

## 60 GEGL plugins of mine compatible with GIMP 2.99/3 

Source code and Linux and Windows binaries avalible - please remove everything from /gegl-0.4/plug-ins before using them or you will experience a crash on closing GIMP 2.99/3. 

Download here
https://github.com/LinuxBeaver/GIMP_3_GEGL_PLUGIN_BUNKER/releases/tag/GIMP3_GEGL_plugins

![image](https://github.com/user-attachments/assets/767ab0d8-adb1-486e-b0fc-eca5502738c1)


![image](https://github.com/LinuxBeaver/GIMP_3_GEGL_PLUGIN_BUNKER/assets/78667207/a8cf1f6d-3eb2-47bc-879e-44966ea6baa9)


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

1. Over 10 plugins were removed indefinitely because they break GIMP 2.99.19 (pie divider, spheres, cutout, frame, text style collection (which contains dozens of plugins in one place), wood texture, align, video dedegradtion mod, bokeh, gum balls are gone.   
2. All plugins that use image uploads had their image upload ability removed
3. GEGL Effects lost its gradient, shiny text and color blend mode picker
4. Glow Stick lost its ability to call gegl:bloom
5. Sparkle lost its dropshadow and had its graph rewritten
6. Rock Text 2 had a domain smooth removed and replaced with mean-curvature-blur; a virtually unnoticable change but it had to happen because gegl:crop left.

### A few technical new features/upgrades happened

1. All plugins now work on layer groups in GIMP 2.99.19 including GEGL Effects
2. GEGL Effects now has a checkbox on its color picker

### GEGL Effects is the only repo on my Github with a 2.99.19 branch

In the future more individual repos may appear...

https://github.com/LinuxBeaver/Gimp_Layer_Effects_Text_Styler_Plugin_GEGL_Effects/tree/Continual_299_19_crash_proof
   

### To explain the problem technically

GIMP 2.99.19/GIMP 3 RC1 is causing extreme problems with my normal plugin repos. Any GEGL plugin of mine that uses the following nodes or has the following warnings

1. gegl:crop (in use internally anywhere) 
2. gegl:load (for uploading an image)
3. **int** where **double** should be (or vice versa)
4. Invalid parameters (ie emboss azimuth=361)
5. Invalid meta tags (ie gegl sensitive without a checkbox)
6. If most render filters are used without being inside the src-in blend mode
7. If GIMP operations use sliders (like gimp:threshold-alpha) inside my plugins. GIMP ops can be called but they cannot be given sliders

has the potential to crash GIMP 3 on closing a window or tab. The longterm goal is for GIMP's team to fix the crash (not me) and until GIMP's team fixes the problem this "GIMP 3 plugin bunker" is the only solution.  GIMP 2.10 and GIMP 3 plugins cannot co-exist anymore.  This is a special branch of all my plugins specifically meant for GIMP 2.99/GIMP 3 only. 

If you use GIMP 2.99.19/GIMP 3 and want to use my plugins everything you download should be here and you should not mix GIMP 2.10 plugins with these as if you do they will crash when closing a tab or GIMP.

## List of GEGL plugins that are indefinitely broke in GIMP 2.99.19

These are plugins I could not successfuly modify and they likely will not be fixed until GIMP's team does something. I put serious effort into fixing some of them and it just won't work, its too complicated.

https://github.com/LinuxBeaver/Gimp_Plugin_Equal_Pie_Divider/

https://github.com/LinuxBeaver/GEGL-Spheres---Render-Spheres-in-Gimp/

https://github.com/LinuxBeaver/GEGL-CutOut-Effect/

https://github.com/LinuxBeaver/Gimp-Text-Style-Collection-Plugin/

https://github.com/LinuxBeaver/Basic-Frame-Gimp-Plugin/

https://github.com/LinuxBeaver/Noise_in_Motion_Gimp_plugin/

https://github.com/LinuxBeaver/Gimp_Align_Tool_Filter/

https://github.com/LinuxBeaver/Fork-of-gegl-video-degradation/

https://github.com/LinuxBeaver/GEGL-Bokeh

https://github.com/LinuxBeaver/Gimp-Balls-and-Confetti-Plugin/releases/

https://github.com/LinuxBeaver/Glass_Tiles_Gimp_Plugin


# This bunker repo permanent solution for 2.99.18/GIMP 3 users unless GIMP's team fixes things. Until further notice GIMP 2.99/3 users should NOT use normal repos or the crash will happen. 
