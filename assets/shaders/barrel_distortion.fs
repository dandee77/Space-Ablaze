#version 330

uniform vec2 resolution;
uniform sampler2D texture0;

vec2 barrelDistortion(vec2 uv) {
    // Normalize to [-1, 1] range
    uv = (uv - 0.5) * 2.0;
    
    // Apply distortion
    uv *= 1.1;    
    uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);
    uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);
    
    // Return to [0, 1] range with slight border
    uv = (uv / 2.0) + 0.5;
    uv = uv * 0.92 + 0.04;
    
    return uv;
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    
    // Apply only the barrel distortion
    vec2 distortedUV = barrelDistortion(uv);
    
    // Sample texture with distortion
    vec4 color = texture2D(texture0, distortedUV);
    
    // Black out any pixels outside the distorted UV range
    if (distortedUV.x < 0.0 || distortedUV.x > 1.0 || 
        distortedUV.y < 0.0 || distortedUV.y > 1.0) {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    
    gl_FragColor = color;
}