uniform sampler2D u_Sampler;
uniform sampler2D u_color_Sampler;
uniform int u_alpha_mode;
uniform int u_need_shadow;
uniform sampler2D u_ShadowMap;

varying vec2 v_TexCoord;
varying vec3 v_LightColor;
varying vec3 v_LightDirection;
varying vec3 v_Normal;
varying vec4 v_PositionFromLight;

void main() {
    vec4 origin_color;
    vec4 non_sun_color;

    if (u_alpha_mode == 0) {
        origin_color = texture2D(u_Sampler, vec2(v_TexCoord.x, 1.0 - v_TexCoord.y));
    } else {
        origin_color = texture2D(u_Sampler, vec2(v_TexCoord.x, 1.0 - v_TexCoord.y)) * texture2D(u_color_Sampler, vec2(0.4, 0.49));
    }

    if (u_need_shadow == 1) {
        gl_FragColor = origin_color * ((v_Normal.r + 1.0)/ 7.0);
    } else {
        gl_FragColor = origin_color;
    }
}