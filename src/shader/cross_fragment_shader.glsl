varying vec2 v_TexCoord;
uniform sampler2D fbo;

void main() {
    vec4 rgba = texture2D(fbo, v_TexCoord.xy);

    float r = 1.0 - rgba.r;
    float g = 1.0 - rgba.g;
    float b = 1.0 - rgba.b;
    gl_FragColor = vec4(r, g, b, 0.5);
}