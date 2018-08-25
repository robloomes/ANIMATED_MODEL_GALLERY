
# ANIMATED_MODEL_GALLERY
Dynamic, animated model gallery with user controlled camera.
Written in C++


Copy of Untitled document
To Run:
Compile: g++ -Wall -c "%f" Plane.cpp Ray.cpp RayTracer.cpp SceneObject.cpp Sphere.cpp Tetrahedron.cpp TextureBMP.cpp
Build exe: g++ -o “%e” Plane.cpp Ray.cpp RayTracer.cpp SceneObject.cpp Sphere.cpp Tetrahedron.cpp TextureBMP.cpp -lGL -lGLU -lglut

A Brief Description
-------------------
My scene contains a simple textured skybox, with a house containing several animated
models on display. From the point of initial loading, we can use the “left”, “right”, “up”, “down”
keys to manipulate the camera into moving around the scene in a tank-like fashion. The
house has an open entrance that you can “walk” into and inspect each model. Each model is
constructed by combining GLUT objects or using a surface of revolution method, and is
animated in some fashion by manipulating translation and/or rotation values over time.
The scene is lit by 2 light sources, the first being a central overhead light at the centre of the
house, and the second being a spotlight with its origin at the bulb of a lamp model in the
scene.

Models
------
1. Stool/goblet: The stool was created using GLUT cubes, a sphere, a cylinder, and 2
discs. The goblet was made from a surface of revolution, taking initial vector form file
“Goblet.off”.
2. Bird: flying around above the goblet is a bird made with an elongated sphere for the
body, and 2 discs for the wings. The wings are rotated around a variable vector point
at the centre of the bird body of the x-axis. The entire set of objects is then rotated
around a vector to produce a “flying” pattern.
3. Cube/Sphere mash: to the left of the scene a GLUT cube with 6 spheres is located
with several rotation points applied to the x-axis and y-axis to produce a somewhat
complex movement pattern.
4. Lamp: a lamp is modelled using GLUT objects (cubes, spheres, discs), with 1 section
moving along the x-axis. A spotlight source is also attached to this object.
5. Planets: simple GLUT spheres with planet-like textures are on display on the back of
the gallery, which have rotations around the y-axis to emulate planet day/night
cycles. Disc objects are also surrounding the planets to produce planet-ring systems.
In addition, a moon with its own “orbit” around the planets completes the features of
this system.
