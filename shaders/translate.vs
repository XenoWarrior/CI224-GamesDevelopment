#version 130

in vec3 position;

out vec3 frag_color;

mat4 projection(
    float angle_of_view_y,
    float aspect_ratio,
    float z_near,
    float z_far
    ) {
    // from https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
    return mat4(
             vec4(1.0/(tan(angle_of_view_y)*aspect_ratio), 0.0, 0.0, 0.0),
             vec4(0.0, angle_of_view_y, 0.0, 0.0),
             vec4(0.0, 0.0, (z_far+z_near)/(z_far-z_near), -1.0),
             vec4(0.0, 0.0, (2.0*z_far*z_near)/(z_near-z_far), 0.0)
           );
}

mat4 translate(float x, float y, float z) {
    return mat4(
             vec4(1.0, 0.0, 0.0, 0.0),
             vec4(0.0, 1.0, 0.0, 0.0),
             vec4(0.0, 0.0, 1.0, 0.0),
             vec4(x, y, z, 1.0)
           );
}

void main() {
      gl_Position = projection(radians(45.0), 4.0/3.0, -0.1, -1000.0)
                      * translate(0.0, 0.0, -5.0)
                      * vec4(position, 1.0f);

      vec3 COLOR_MASKS[8];
      COLOR_MASKS[0] = vec3(0.0, 0.0, 0.0);
      COLOR_MASKS[1] = vec3(0.5, 0.5, 0.5);
      COLOR_MASKS[2] = vec3(1.0, 0.5, 0.5);
      COLOR_MASKS[3] = vec3(0.5, 1.0, 0.5);
      COLOR_MASKS[4] = vec3(0.5, 0.5, 1.0);
      COLOR_MASKS[5] = vec3(0.5, 1.0, 1.0);
      COLOR_MASKS[6] = vec3(1.0, 0.5, 1.0);
      COLOR_MASKS[7] = vec3(1.0, 1.0, 0.5);

      frag_color = vec3(COLOR_MASKS[6]); 
}
