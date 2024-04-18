// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef AXISSHADERS_H_
# define AXISSHADERS_H_
# define VAR_FragColor "m"
# define VAR_ModelViewProjectionMatrix "i"
# define VAR_in_Color "P"
# define VAR_in_Position "l"
# define VAR_out_Color "v"

const char *axis_frag_glsl =
 "#version 330\n"
 "in vec3 v;"
 "out vec4 m;"
 "void main()"
 "{"
   "m=vec4(mix(v,vec3(1),.5f),1);"
 "}";

const char *axis_vert_glsl =
 "#version 330\n"
 "uniform mat4 i;"
 "layout(location=0) in vec3 l;"
 "layout(location=1) in vec3 P;"
 "out vec3 v;"
 "void main()"
 "{"
   "gl_Position=i*vec4(l,1);"
   "v=P;"
 "}";

#endif // AXISSHADERS_H_
