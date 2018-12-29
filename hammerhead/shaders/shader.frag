#version 150
//It was expressed that some drivers required this next line to function properly
precision highp float;

in vec3 ex_Color;
in vec3 ex_Normal;
in vec2 ex_TextureCoord;
out vec4 gl_FragColor;
varying vec3 ec_pos;

uniform sampler2D ourTexture;

void main(void) {
	//Pass through our original color with full opacity
	//gl_FragColor = vec4(ex_Color, 1.0);

	//vec3 base3 = vec3(0.99, 0.96, 0.89);
    //vec3 base2 = vec3(0.92, 0.91, 0.83);
    //vec3 base00 = vec3(0.40, 0.48, 0.51);

	vec3 base3 = vec3(ex_Color.x * 0.99, ex_Color.y * 0.96, ex_Color.z * 0.89);
	vec3 base2 = vec3(ex_Color.x * 0.92, ex_Color.y * 0.91, ex_Color.z * 0.83);
	vec3 base00 = vec3(ex_Color.x * 0.40, ex_Color.y * 0.48, ex_Color.z * 0.51);

    vec3 ec_normal = normalize(cross(dFdx(ec_pos), dFdy(ec_pos)));
    //ec_normal.z *= zoom;
    ec_normal = normalize(ec_normal);
	//ec_normal = ex_Normal;

    float a = dot(ec_normal, vec3(0.0, 0.0, 1.0));
    float b = dot(ec_normal, vec3(-0.57, -0.57, 0.57));

	//gl_FragColor = texture(ourTexture, ex_TextureCoord);

	gl_FragColor = texture(ourTexture, ex_TextureCoord) * vec4((a*base2 + (1-a)*base00)*0.5 + (b*base3 + (1-b)*base00)*0.5, 1.0);

    //gl_FragColor = vec4((a*base2 + (1-a)*base00)*0.5 +
     //                   (b*base3 + (1-b)*base00)*0.5, 1.0);

	//gl_FragColor = vec4(ex_Color, 1.0);
}