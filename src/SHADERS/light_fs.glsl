#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
//uniform vec3 objectColor;
uniform sampler2D Diffuse;
uniform sampler2D Specular;

void main()
{
    // ambient
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor * texture(Diffuse, TexCoord).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * texture(Diffuse, TexCoord).rgb;
    
    // specular
    float specularStrength = 0.774597;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
        
    // halfway 
    vec3 halfway = normalize(viewDir + lightDir);

    float specAmount = pow(max(dot(viewDir, halfway), 0.0), 64);
    vec3 specular = specularStrength * specAmount * lightColor * texture(Specular, TexCoord).rgb;  
    
    // texture and light combination
    vec3 result = ambient + diffuse + specular;
    FragColor =  vec4(result, 1.0f);
}
