# lltool

![panorama](https://lh6.googleusercontent.com/-Q4p5qHEUWps/UXVzhYw-YgI/AAAAAAAAAdw/MTd5Gd1_TuM/s971/lltool.jpg)

## Landscape and Heightmap Triangulation Tool and Library

For details take a look to the [full manual](https://github.com/Gruftikus/lltool/wiki/).

### Overview

lltool provides several methods and algorithms for heightmap manipulation and terrain triangulation. Steering of the process is done via a batch script with a small and dedicated scripting language. The terrain triangulation is done in two steps: in a first step vertices are placed, either based on vertex placing methods, or via (a combination) of density algorithms. In second step the vertices are triangulated and the resulting mesh can be further manipulated. lltool provides multi-level LODs, break lines and polygon stencils.

### Features

* Map creation and manipulation: importers/exporters and basic terrain moddeling
* Support for cells and quads (for game LODs)
* Multiple algorithms for random Monte-Carlo vertex placement, e.g. based on first and second order, height, peak finding, and general purpose equations
* Triangle manipulation: stencils and break lines
* Output formats for meshes: obj- and VRML-files
* Texture map handling (import, export, scaling)
* Formats for textures: bmp and dds

### Installation

Go to the directory "externals". lltool needs (in addition to the included packages):

* [MTParser](http://www.codeproject.com/Articles/7335/An-extensible-math-expression-parser-with-plug-ins) by Mathieu Jacques, download the code and put it into externals/mtparser (Windows only!!!)

Windows:

* Open lltool.sln and build the project.

Linux:

* Run "make"

### Syntax

The command can be followed by several flags (see batch section in the full manual) in order to propagate variables to the execution script.

The section used by the execution script is defined by a section name in squared brackets (like the section in the script).

The last option might be the filename. If no filename is given, stdin is used, in this case, reading can be stopped with `@end`.

    lltool flag1 flag2 ... [section] filename.mpb

## References

### Software

[An extensible math expression parser with plug-ins (MTParser)](http://www.codeproject.com/Articles/7335/An-extensible-math-expression-parser-with-plug-ins) by Mathieu Jacques

[Triangle](http://www.cs.cmu.edu/~quake/triangle.html): "Engineering a 2D Quality Mesh Generator and Delaunay Triangulator", in "Applied Computational Geometry: Towards Geometric Engineering" (Ming C. Lin and Dinesh Manocha, editors), volume 1148 of Lecture Notes in Computer Science, pages 203-222, Springer-Verlag, Berlin, May 1996" by J. R. Shewchuk

[imageresampler](https://code.google.com/p/imageresampler/), a C++ class for separable filtering 2D image resampling by Rich Geldreich.

[crunch/crnlib](https://code.google.com/p/crunch/), for dds file generation.

[ANN](http://www.cs.umd.edu/~mount/ANN/), Approximate Nearest Neighbor Searching by David M. Mount and Sunil Arya

### Applications

* [tes4ll](http://oblivion.nexusmods.com/mods/40549/)
* [Immersive Interiors Landscape AddOn](http://oblivion.nexusmods.com/mods/43072/)
* [Realistic Normal Maps with Faked Terrain Shadows](http://oblivion.nexusmods.com/mods/41243/)

## Licence

Copyright (C) 2011-2014

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.








