#version 330 core
out vec4 FragColor;

uniform float time;

void main()
{
    float r = abs(sin(time));
    float g = abs(sin(time + 2.0944)); // phase of 2pi/3
    float b = abs(sin(time - 4.18879)); // twice phase size of above one
    
    FragColor = vec4(r, g, b, 1.0); 

    //FragColor = vec4(1.0); // set all 4 vector values to 1.0
}