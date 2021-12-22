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
        vec3 normal = normalize(v_Normal.xyz);
        normal = normal * 2.0;
        vec3 direction = normalize(v_LightDirection);
        float nDotL = max(dot(direction, normal), 0.3);
        vec3 diffuse = v_LightColor * origin_color.rgb * nDotL;
        non_sun_color = vec4(diffuse, origin_color.a);

        vec3 shadowCoord = (v_PositionFromLight.xyz/v_PositionFromLight.w)/2.0 + 0.5;
        vec4 rgbaDepth = texture2D(u_ShadowMap, shadowCoord.xy);
        float depth = rgbaDepth.r;
        float visibility = (shadowCoord.z > depth ) ? 0.5 : 1.0;
        gl_FragColor = min(vec4(origin_color.rgb * visibility, origin_color.a), non_sun_color);
//        gl_FragColor = non_sun_color;
    } else {
        gl_FragColor = origin_color;
    }
}