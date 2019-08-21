uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;

attribute vec4 vPosition;
uniform vec4 vColor;

varying vec4 varColor;
varying vec2 varTextCoords;

void main()
{
	varColor = vColor;
	gl_Position = uProjection * uView * uModel * vPosition;
}
