# Fifed (Fif editor)
Fifed is the editor for fif, it is not functional at the moment.

# Modules
Fif is split into modules:
- core (window handling, profiling, etc)  
- gfx (rendering, opengl)   

# OpenGL
Fif is using OpenGL ES 3.0 as the rendering backend, which is why it is so easy to port apps made with fif to web using emscripten

# Dependencies
## Submodules (no need to install them manually)   
- glm  
- entt  
- glfw  
- glad
- ImGui    

## Need to be manually installed   
- emscripten (emsdk) (optional)
