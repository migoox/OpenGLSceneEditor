# Table of Contents
  * [Introduction](#introduction)
  * [Gallery](#gallery)
  * [Usage](#usage)
    + [Camera and modes](#camera-and-modes)
    + [Tree Panel and Editor Panel](#tree-panel-and-editor-panel)
    + [Properties Panel](#properties-panel)
      - [Light Component](#light-component)
      - [Cube Component](#cube-component)
      - [Model Component](#model-component)
    + [Camera Panel](#camera-panel)

# OpenGLSceneEditor
## Introduction
OpenGLSceneEditor is an editor build on my basic 3D Engine written in C++ with OpenGL. It supports 
- transformation of objects (position, rotation and scale),
- camera,
- dynamic lighting,
- textures (with loading),
- specular maps (with loading),
- 3-parameter materials (ambient, diffuse, specular),
- loading models and adding cubes.

Project was inspired by:
- The Cherno and his OpenGL series - https://www.youtube.com/@TheCherno,
- Learn OpenGL tutorials written by Joey de Vries - https://learnopengl.com/

3rd party repositories used:
- https://github.com/nigels-com/glew
- https://github.com/glfw/glfw
- https://github.com/ocornut/imgui
- https://github.com/rajsahae/thecherno_opengl

## Gallery
<img src="https://user-images.githubusercontent.com/56317134/221730526-58b4f4f9-05c8-424a-84f8-bc66df505416.png" width="400">

<img src="https://user-images.githubusercontent.com/56317134/221730546-e0ba057b-9cb3-424d-9489-5c3509459d38.png" width="400">

<img src="https://user-images.githubusercontent.com/56317134/221730582-32c86911-77fe-4238-bddf-72fbc5d8758b.png" width="400">

<img src="https://user-images.githubusercontent.com/56317134/221730613-3bc1e4e9-99f2-400b-8a2c-a9ac2acb6557.png" width="400">

## Usage
### Camera and modes
The camera operates in two modes - The Frozen and The Movable. 

| Key(s)          | Action                          | Mode             |
|-----------------|---------------------------------|------------------|
| `W`/`S`/`A`/`D` | Move forward/back/left/right    | The Movable Mode |
| `Space`         | Move up                         | The Movable Mode |
| `LShift`        | Move down                       | The Movable Mode |
| `E`             | Switch between the camera modes | Both modes       |

Upon entering The Frozen Mode, 
the mouse cursor will appear and you should be able to play with the GUI.

### Tree Panel and Editor Panel
*Tree Panel* allows to choose/delete/add an object. To choose an object
just click on on the entry from item list - its properties will appear in
*Properties Panel* discussed later. 

<img src="https://user-images.githubusercontent.com/56317134/221730623-d34e0c88-7459-4083-83f1-7ba0274f4d3e.png" width="150">

To outline a selected object, use *Editor Panel* and select "Selection visibility"
checkbox.

<img src="https://user-images.githubusercontent.com/56317134/221730641-627c5f70-b082-4237-83d8-199693e2d5a7.png" width="200">

From this panel you can also display boxes that represent light positions.

In order To add a new object, in *Tree Panel* click add button, which will cause the *Object Creator* to appear:

<img src="https://user-images.githubusercontent.com/56317134/221730652-d3636640-64f2-490a-96af-a222f195155b.png" width="200">

It allows you to choose the object type - *Cube/Model/Default Light/Directional Light/Point Light/Spotlight*
and give it a name (can be changed later in *Properties Panel*) that will appear in the *Tree Editor*.

### Properties Panel
The appereance of the *Properties Panel* will vary depending on a selected object's object type.
In *Properties Panel* we call each section (besides the header with "Name" and "Visibility") a *Component*. 

The only component that appears in every object type in the same form is the *Transform Component*, where you change Position, Scale
and Rotation of the selected object. 

<img src="https://user-images.githubusercontent.com/56317134/221730683-cc1ea067-fb75-4145-8437-cfd2abbadd2c.png" width="200">

#### Light Component
Appears in properties of objects of type 
- *Default Light*,
- *Directional Light*,
- *Point Light*,
- *Spotlight*.

All of the light Components have 3 features in common:
- Color - the color specified here covers these parts of the object that are directly exposed to light rays,
- Ambient - even if light is not directly facing an object, the object can't be completely dark. Parts of the object that are not directly exposed to light rays are covered with ambient color,
- Specular - allows to specify intensity of the effect when the light is reflected from the object directly into the eyes of the observer.

<img src="https://user-images.githubusercontent.com/56317134/221730710-e0f81156-ff1c-4db1-8447-79c3679addb1.png" width="400">

Same scenery, but with different parameters:

<img src="https://user-images.githubusercontent.com/56317134/221730730-a6ca93f7-ef01-4cf6-be4d-37b3f94cda14.png" width="400">

**Default Light** is the simplest light available. It has a behaviour of point light with unlimited range. *Component*
for this light type has only 3 features described above.

**Directional Light** can be used to simulate illumination created by sunlight, all of whose rays 
from our perspective are directed in the same direction and do not lose their strength. As the *Default Light*, *Component*
for this light type has only 3 features described above.

<img src="https://user-images.githubusercontent.com/56317134/221731094-2051cf05-d330-4154-b4bc-4dd4f32449f6.png" width="200">

**Point Light** has the same behaviour as *Default Light*, but it's range is limited. Intensity of the light
in the given point $P$ is given by the formula
$$\text{intensity} = \frac{1.0}{1.0 + l \cdot d + q \cdot d^2},$$
where parameters $l$ and $q$ are constants specified in the *Light Component* of the *Point Light*, and $d$ is a distance between $P$ and the position of the *Point Light*.

<img src="https://user-images.githubusercontent.com/56317134/221730757-205bd116-e85e-447a-a3a6-0f83942bfd90.png" width="200">

**Spotlight** behaves like a flashlight. It offers 2 parameters - *Cut off angle* ($\Theta$) and *Outer Cut off angle* ($\Phi$). In the image below you can see what these parameters are responsible for.

<img src="https://user-images.githubusercontent.com/56317134/221730807-43125a50-b87a-40d1-b0a1-91de03506396.png" width="200">

Image source: https://learnopengl.com/img/lighting/light_casters_spotlight_angles.png

<img src="https://user-images.githubusercontent.com/56317134/221730834-228301d0-98d2-4fc1-933a-1ec075689b92.png"  width="200">

#### Cube Component

#### Model Component

### Camera Panel
