# Starter project for OpenGL projects

[Link to source video](https://www.youtube.com/watch?v=LxEFn-cGdE0&t=252s)

In this project:

- `glad` is used to search for OpenGL functions
- `glfw` is used to open windows. It was installed on my system with `sudo apt-get install libglfw3-dev`

## Documentation for OpenGL

Great site for OpenGL documentation: [docs.gl](https://docs.gl/gl3/glBufferData)

## General vocabulary

- **Index Buffer**:
  Store vertex positions in order to remove duplicate memory of vertices when rendering multiple triangles
  with shared vertices.

- **Vector**:
  Can simply be a position on a screen

- **Projection Matrix**:
  Tells the window how to map the vertices to it.

- **MVP or PVM**:
  Model view projection matrix. Transformation pipeline that goes through each vertex.
  Model, view and projection are each a 4 by 4 matrix, and they all get multiplied in that order 
  (actually PVM in OpenGL, MVP in DirectX due to row ordering)

- **Normalized divise coordinates**:
  Normalized space we can then map to the window. Normalized as in between +-1.

- **Projections**:
  Allow you to decide what your coordinate space is so you can render objects however you like (inside or outside screen), 
  and worry about normalization later.
  Orthographic projecetion is usually used for 2D, as opposed to perspective projection which is used for 3D.

- **Model matrix**
  Way for us to simulate the model (vertex we're drawing). Represents the object transformation.

- **View/Camera matrix**
  View of the camera. Way for us to simulate a camera.

- **Material**
  Shader + uniforms
