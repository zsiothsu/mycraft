attribute vec4 a_Position;
attribute vec2 a_TexCoord;
attribute vec3 a_Normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 u_BlockPosition;

uniform vec3 u_LightColor;
uniform vec3 u_LightDirection;

uniform mat4 M_FromLight;
uniform mat4 V_FromLight;
uniform mat4 P_FromLight;

varying vec2 v_TexCoord;
varying vec3 v_LightColor;
varying vec3 v_LightDirection;
varying vec3 v_Normal;
varying vec4 v_PositionFromLight;

void main() {
    vec4 position = a_Position + vec4(u_BlockPosition, 0.0);
    gl_Position = P * V * M * a_Position;

    v_TexCoord = a_TexCoord;
    v_LightColor = u_LightColor;
    v_LightDirection = u_LightDirection;
    v_Normal = a_Normal;
    v_PositionFromLight = P_FromLight * V_FromLight * M_FromLight * a_Position;
}