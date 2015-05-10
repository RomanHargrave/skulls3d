# Compile skulls3d #

You need:
  * The [SDL](http://www.libsdl.org/), as of 2010-02, the latest development version is 1.2.14-VC8 ([download](http://www.libsdl.org/release/SDL-devel-1.2.14-VC8.zip)).
  * [swegl](http://code.google.com/p/swegl/), the 3D software renderer. Do not get the latest TRUNK version. You need the branch tagged "v0.0.1"


Once downloaded the source tree should look like this:
```
mycodefolder/skulls3d/Src/main.cpp
                         /...
                     /Projects/VC 2008/skulls3d.sln
                                      /...
                     /Projects/VCE 2005/skulls3d.sln
                                       /...
            /swegl/Projects/VC 2008/swegl.sln
                                   /...
                           /VCE 2005/swegl.sln
                                    /...
                  /Src/DataClasses
                      /...
```

Then you have to copy your copy DOOM.WAD from your registered copy of DOOM into the folder skulls3d. Copy SDL.dll there too.

You now need to change a user preference in the project. Open skulls3d.sln right-click on the project, open the properties box. Select the configuration "All configurations". Now select Configuration Properties > Debugging. As Working Directory, set "$(ProjectDir)\..\..\" without the quotes. This way, when debugging or running from visual studio, the program runs in the root folder of the project, thus having access to doom.wad and sdl.dll.

Now press F5 and hope for the best.