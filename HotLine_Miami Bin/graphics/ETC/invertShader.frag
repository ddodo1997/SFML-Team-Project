uniform sampler2D texture;
uniform float fade; // 페이드 효과를 위한 값 (0.0 ~ 1.0)

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    vec3 invertedColor = vec3(1.0) - color.rgb; // 색 반전
    gl_FragColor = vec4(mix(color.rgb, invertedColor, fade), color.a);
}