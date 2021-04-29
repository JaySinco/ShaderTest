/*
  (C) 2019 David Lettier
  lettier.com
*/

#version 150

uniform sampler2D p3d_Texture0;

in vec2 texCoord;

out vec4 fragColor;

void main() { fragColor = texture(p3d_Texture0, texCoord); }
