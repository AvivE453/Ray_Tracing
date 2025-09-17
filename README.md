In this program I implemented a ray tracer from scratch.

Ray tracing is a rendering technique for generating an image by tracing the path of light through pixels in an image
plane and simulating the effects of its encounters with virtual objects.
A ray tracer shoots rays from the observer’s eye through a screen and into a scene of objects.
It calculates the ray’s intersection with the scene objects, finds the nearest intersection and calculates the color of
the surface according to its material and lighting conditions.

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

The project required you to build a small rendering engine with support for:
  •Geometric primitives: Spheres, planes, background.
  •Lighting: Ambient light, directional lights, and spotlights.
  •Materials: Phong model (ambient, diffuse, specular, shininess).
  •Shadows: Hard shadows via secondary rays.
  •Reflections & Transparency: With recursive ray tracing (up to 5 steps).
  •Anti-aliasing (bonus): Multi-sampling per pixel

The scene was described using a text file, containing:
  •Camera position (e)
  •Ambient light (a)
  •Light sources (d, p, i)
  •Objects: spheres/planes (o), reflective (r), transparent (t)
  •Object colors & shininess (c)

This file format defined the geometry, materials, and lights of your virtual world.

how to run ? • use "make" command to run the makefile • enter bin directory and run "./main scene1.txt" command
