uniform sampler2D u_Sampler;

varying vec2 v_TexCoord;

void main() {
    gl_FragColor = texture2D(u_Sampler, vec2(v_TexCoord.x, 1.0 - v_TexCoord.y));
}