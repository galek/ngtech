[GLSL_VERTEX_SHADER]

varying vec2 pixel;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	pixel = gl_Position.xy;
}

[GLSL_FRAGMENT_SHADER]
#version 330
//OUT
layout(location = 0) out vec4 OutColor;

#define MAX_ITER 64.0

varying vec2 pixel;

uniform vec2 position;
uniform vec2 scale;
uniform sampler2D colorMap;

void main() {
	float x, y, x0, y0;
	x = x0 = (pixel.x * scale.x + position.x); 
	y = y0 = (pixel.y * scale.y + position.y); 

	float x2 = x*x;
	float y2 = y*y;

	float iteration = 0.0;

	while(x2 + y2 < 4.0 && iteration < MAX_ITER) {
 		y = 2.0 * x * y + y0;
	    x = x2 - y2 + x0;

		x2 = x*x;     
		y2 = y*y;

		iteration++;
	};
					 
	OutColor = texture2D(colorMap, vec2(iteration / MAX_ITER, iteration / MAX_ITER));
}
