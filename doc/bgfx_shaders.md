Shaders
-------

* Attributes defined in the varying.def.sc file must have the same name as their binding. For example:
    vec2 a_position  : POSITION;
    vec2 a_texcoord0 : TEXCOORD0;
    
* Attributes used for instancing must have the names: i_data0, i_data1, i_data2, i_data3, i_data4 and 
have the type vec4. They can be bound to any binding (?).

