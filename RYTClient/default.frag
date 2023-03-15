#version 330 core

out vec4 FragColor;

#define maxIterations 1000

uniform int width;
uniform int height;
uniform float xScale;
uniform float yScale;
uniform float xOffset;
uniform float yOffset;

vec3 YUVtoRGB(vec3 yuvIN)
{
    vec3 rgbOut;
    
    return rgbOut
}


vec4 ptColor()
{
    vec4 color;

    int iterations = iterations();

    if(iterations == maxIterations)
        color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    else
    {
        color = HtoRGB(float(iterations) * 360 * 1 / maxIterations);
    }

    return color;
}



void main()
{
   FragColor = ptColor();
}