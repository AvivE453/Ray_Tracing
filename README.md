In this program I implemented a ray tracer from scratch.

Ray tracing is a rendering technique for generating an image by tracing the path of light through pixels in an image
plane and simulating the effects of its encounters with virtual objects.
A ray tracer shoots rays from the observer’s eye through a screen and into a scene of objects.
It calculates the ray’s intersection with the scene objects, finds the nearest intersection and calculates the color of
the surface according to its material and lighting conditions.

The project rendering engine supports :
  • Geometric primitives: Spheres, planes, background.
  • Lighting: Ambient light, directional lights, and spotlights.
  • Materials color: Phong model (ambient, diffuse, specular, shininess).
  • Shadows: Hard shadows via secondary rays.
  • Reflections (mirror object) & Transparency: With recursive ray tracing .

The 3D scene for rendering defined by using a text file, containing:
    ● "e" (eye) – Represents the Camera position coordinates (x,y,z).The 4th coordinate will always be 1.0 and can be ignored.
    ● "a" (ambient) – Represents the Global Ambient Intensity (r,g,b).The 4th coordinate will always be 1.0 and can be ignored.
    The next parameters represent the information about the light sources and objects:
        ● "d" (direction) – Represents the Light source direction (x, y, z). The 4th coordinate value will be 0.0 for Directional light and 1.0 for Spotlight.
        ● "p" (position) – (Only for spotlights) Represents the Spotlight position coordinates (x,y,z).The 4th coordinate value represents the cutoff angle cosine value.("p" order corresponds to the "d" spotlights order)
        ● "i" (intensity) – Represents the Light source Intensity (r, g, b).The 4th coordinate will always be 1.0 and can be ignored.("i" order corresponds to the "d" order)
        ● "o" (object), "r" (reflective) or "t" (transparent) – Represents Spheres and Planes, where:
            - Spheres (x,y,z,r) – where (x,y,z) is the center position and r is the radius (r>0).
            - Planes (a,b,c,d) – where (a,b,c,d) represents the coefficients of the plane equation (d<=0).
            Notice the following things:
                - The 4th coordinate determines if the object is a sphere or a plane.
                - Spheres and Planes can be either normal objects, reflective objects or transparent objects and will require different handling based on their type.
                - For "r" (reflective) and "t" (transparent) and next "c" parameter values can be ignored.
        ● "c” (color) - Represents the Ambient Material and Diffuse Material color (r,g,b).The 4th coordinate represents the Shininess value.("c" order corresponds to the "o", "r", "t" order)


Rendering Pipeline:

• Parse the scene description file into structured data.
• For each pixel:
    • Shoot a ray through its center (or multiple for anti-aliasing).
    • Check intersections with objects (spheres, planes).
    • Determine the nearest hit point.
    • Compute color using Phong shading with contributions from all lights.
    • Add shadow checks.
    • Handle recursive reflection and transparency rays if needed.
• Output the final image.





This file format defined the geometry, materials, and lights of your virtual world.

how to run ? • use "make" command to run the makefile • enter bin directory and run "./main scene1.txt" command
