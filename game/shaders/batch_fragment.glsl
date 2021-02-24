#version 330 core
in vec2 TextCoordsFrag;
in vec4 ColorFrag;
out vec4 Result;

uniform sampler2DRect Texture;

void main()
{
    vec4 textColor;
    if (TextCoordsFrag == vec2(-1, -1)) {
        textColor = vec4(1, 1, 1, 1);
    }
    else {
        textColor = texture(Texture, TextCoordsFrag);
    }
    Result = ColorFrag * textColor;
}
