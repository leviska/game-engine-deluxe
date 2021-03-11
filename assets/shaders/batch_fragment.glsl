#version 330 core
in vec2 TextCoordsFrag;
in vec4 ColorFrag;
in float TextureIdFrag;
out vec4 Result;

layout (binding = 0) uniform sampler2DRect Texture0;
layout (binding = 1) uniform sampler2DRect Texture1;
layout (binding = 2) uniform sampler2DRect Texture2;
layout (binding = 3) uniform sampler2DRect Texture3;
layout (binding = 4) uniform sampler2DRect Texture4;
layout (binding = 5) uniform sampler2DRect Texture5;
layout (binding = 6) uniform sampler2DRect Texture6;
layout (binding = 7) uniform sampler2DRect Texture7;
layout (binding = 8) uniform sampler2DRect Texture8;
layout (binding = 9) uniform sampler2DRect Texture9;
layout (binding = 10) uniform sampler2DRect Texture10;
layout (binding = 11) uniform sampler2DRect Texture11;
layout (binding = 12) uniform sampler2DRect Texture12;
layout (binding = 13) uniform sampler2DRect Texture13;
layout (binding = 14) uniform sampler2DRect Texture14;
layout (binding = 15) uniform sampler2DRect Texture15;

void main()
{
    vec4 textColor;
    if (TextureIdFrag == 0) {
        textColor = texture(Texture0, TextCoordsFrag);
    } else if (TextureIdFrag == 1) {
        textColor = texture(Texture1, TextCoordsFrag);
    } else if (TextureIdFrag == 2) {
        textColor = texture(Texture2, TextCoordsFrag);
    } else if (TextureIdFrag == 3) {
        textColor = texture(Texture3, TextCoordsFrag);
    } else if (TextureIdFrag == 4) {
        textColor = texture(Texture4, TextCoordsFrag);
    } else if (TextureIdFrag == 5) {
        textColor = texture(Texture5, TextCoordsFrag);
    } else if (TextureIdFrag == 6) {
        textColor = texture(Texture6, TextCoordsFrag);
    } else if (TextureIdFrag == 7) {
        textColor = texture(Texture7, TextCoordsFrag);
    } else if (TextureIdFrag == 8) {
        textColor = texture(Texture8, TextCoordsFrag);
    } else if (TextureIdFrag == 9) {
        textColor = texture(Texture9, TextCoordsFrag);
    } else if (TextureIdFrag == 10) {
        textColor = texture(Texture10, TextCoordsFrag);
    } else if (TextureIdFrag == 11) {
        textColor = texture(Texture11, TextCoordsFrag);
    } else if (TextureIdFrag == 12) {
        textColor = texture(Texture12, TextCoordsFrag);
    } else if (TextureIdFrag == 13) {
        textColor = texture(Texture13, TextCoordsFrag);
    } else if (TextureIdFrag == 14) {
        textColor = texture(Texture14, TextCoordsFrag);
    } else if (TextureIdFrag == 15) {
        textColor = texture(Texture15, TextCoordsFrag);
    }
    Result = ColorFrag * textColor;
}
