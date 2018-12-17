#version 150
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in vec3 in_Position;
in vec3 in_Color;

//mvp matrix is the result of multiplying the model, view, and projection matrices
uniform mat4 mvpmatrix;

//We output the ex_Color variable to the next shader in the chain
out vec3 ex_Color;
void main(void) {
	gl_Position = mvpmatrix * vec4(in_Position, 1.0);
	
	//We're simply passing the color through unmodified
	ex_Color = in_Color;
}