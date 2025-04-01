#version 330

uniform vec2 resolution;
uniform sampler2D texture0;

vec2 barrelDistortion(vec2 uv) {
 
    uv = (uv - 0.5) * 2.0;

    uv *= 1.1;    
    uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);
    uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);
    
    uv = (uv / 2.0) + 0.5;
    uv = uv * 0.92 + 0.04;
    
    return uv;
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    
    vec2 distortedUV = barrelDistortion(uv);
    
    vec4 color = texture2D(texture0, distortedUV);
    
    if (distortedUV.x < 0.0 || distortedUV.x > 1.0 || 
        distortedUV.y < 0.0 || distortedUV.y > 1.0) {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }

    float vig = (0.0 + 1.0 * 16.0 * uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y));
    color.rgb *= vec3(pow(vig, 0.3));
    
    color.rgb *= vec3(0.85, 1.15, 0.80); 
    color.rgb = color.rgb * 1.1 - 0.05;
    color.rgb = clamp(color.rgb, 0.0, 1.0);
    
    gl_FragColor = color;
}