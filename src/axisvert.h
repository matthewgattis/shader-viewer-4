// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef AXISVERT_H_
# define AXISVERT_H_
# define VAR_ModelViewProjectionMatrix "v"
# define VAR_in_Color "m"
# define VAR_in_Position "i"
# define VAR_out_Color "n"

const char *axis_vert =
 "#version 330\n"
 "uniform mat4 v;"
 "layout(location=0) in vec3 i;"
 "layout(location=1) in vec3 m;"
 "out vec3 n;"
 "void main()"
 "{"
   "gl_Position=v*vec4(i,1);"
   "n=m;"
 "}";

#endif // AXISVERT_H_
