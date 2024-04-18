// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef AXISFRAG_H_
# define AXISFRAG_H_
# define VAR_FragColor "v"
# define VAR_out_Color "m"

const char *axis_frag =
 "#version 330\n"
 "in vec3 m;"
 "out vec4 v;"
 "void main()"
 "{"
   "v=vec4(mix(m,vec3(1),.5f),1);"
 "}";

#endif // AXISFRAG_H_
