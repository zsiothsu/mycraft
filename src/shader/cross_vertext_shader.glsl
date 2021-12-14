attribute vec4 a_Position;
varying vec2 v_TexCoord;

void main() {
    gl_Position = a_Position;
    v_TexCoord = vec2(a_Position.x, a_Position.y);
}