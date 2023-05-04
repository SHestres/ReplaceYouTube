#version 330 core

out vec4 FragColor;


uniform int width;
uniform int height;

vec3 YUVtoRGB(vec3 yuvIN)
{
    vec3 rgbOut;
    
    return rgbOut;
}


vec4 ptColor()
{
    vec4 color;

    color = vec4(gl_FragCoord.x / width,0.0,gl_FragCoord.y / height,1.0f);

    return color;
}



void main()
{
   FragColor = ptColor();
}