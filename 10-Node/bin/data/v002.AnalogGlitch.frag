// define our rectangular texture samplers 
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;	// bars and distortion texture
uniform vec2 	texdim0;

uniform float   barsamount;	// changes bars amount
uniform float   distortion;	// amount of distortion
uniform int     resolution; // for scanlines
uniform float   vsync;		// sync
uniform float   hsync;

// define our varying texture coordinates 
//
varying vec2 texcoord0;

void main (void) 
{ 		
	vec2 point = texcoord0/texdim0;
	vec2 original = point;

	vec4 bars = texture2DRect(tex1, texcoord0);

	// scanlines
	vec4 stripes; // our base 'stripe color'
	stripes = vec4(floor(mod(texcoord0.y, float(resolution)+0.0001)));
	stripes = clamp(stripes, 0.0, 1.0);
	stripes = 2.0 - (stripes - vec4(0.)); // subtract

	// get rough luma 
	vec4 key = texture2DRect(tex0, (vec2(point.y, point.y)) * texdim0);
	key += texture2DRect(tex0, (1.0 - vec2(point.y, point.y)) * texdim0);
	key -= bars.r;
	//key *= 0.33;
	float d = key.r + key.g + key.b;
	d /= 3.0;
	point.x -= d * distortion * 0.1;	

	//sync			
	vec2 texcoord = point + ( mod(vec2(hsync, vsync), 1.0)); 
	
	// wrap
	texcoord = mod(texcoord, 1.0);
	
	// outout
	vec4 result = stripes * texture2DRect(tex0, texcoord * texdim0);
	gl_FragColor = mix(result, bars*result, barsamount);
} 


