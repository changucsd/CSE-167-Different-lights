#version 330 core
// This is a sample fragment shader.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


struct DirLight
{
    // vec3 position; // 现在不在需要光源位置了，因为它是无限远的
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    
    float constant;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
};



uniform float phong;
uniform float mode;
uniform float turnOn;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 lightColor;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 normalColor;
in vec3 Normal;
in vec3 FragPos;


// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    if(phong == 1.0f)
    {
        /*
        // 环境光
        vec3 ambient = lightColor * material.ambient;
    
        // 漫反射光
        vec3 norm = Normal;
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lightColor * (diff * material.diffuse);
    
        // 镜面高光
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lightColor * (spec * material.specular);
    
        vec3 result = ambient + diffuse + specular;
        color = vec4(result, 1.0f);
        
         */
        
        
        vec3 norm = Normal;
        vec3 viewDir = normalize(viewPos - FragPos);
        
        vec3 result;
        if(mode == 1.0f) // directional only
        {
            if(turnOn == 1.0f)
            {
                result = CalcDirLight(dirLight, norm, viewDir);
            }
            
            else
            {
                result = material.ambient;
            }
        }
        
        if (mode == 2.0f) // directional + point
        {
            if(turnOn == 1.0f)
            {
                result = CalcDirLight(dirLight, norm, viewDir);
                result += CalcPointLight(pointLight, norm, FragPos, viewDir);
            }
            else
            {
                result = CalcPointLight(pointLight, norm, FragPos, viewDir);
            }
 
        }
        
        if( mode == 3.0f) //directional + spot
        {
            if(turnOn == 1.0f)
            {
                result = CalcDirLight(dirLight, norm, viewDir);
                result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
            }
            else
            {
           
                result = CalcSpotLight(spotLight, norm, FragPos, viewDir);
            }

        }
        
        // == ======================================
        // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
        // For each phase, a calculate function is defined that calculates the corresponding color
        // per lamp. In the main() function we take all the calculated colors and sum them up for
        // this fragment's final color.
        // == ======================================
        // Phase 1: Directional lighting
        //vec3 result = CalcDirLight(dirLight, norm, viewDir);
        //vec3 result = CalcPointLight(pointLight, norm, FragPos, viewDir);
        //vec3 result = CalcSpotLight(spotLight, norm, FragPos, viewDir);
        // Phase 2: Point lights
        
        //result += CalcPointLight(pointLight, norm, FragPos, viewDir);
        /*
        // Phase 3: Spot light
        // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
        */
        
        color = vec4(result, 1.0);
    }
    
    else
    {
        color = vec4(normalColor,1.0f);
    }
    
}


// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}


// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance);
    
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    
    // Check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
    
    if(theta > light.cutOff) // Remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {
        // Ambient
        vec3 ambient = light.ambient * material.ambient;
        
        // Diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * material.diffuse);
        
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular *(spec * material.specular);
        
        // Attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0f / (light.constant + light.quadratic * (distance * distance));
        
        // ambient  *= attenuation;  // Also remove attenuation from ambient, because if we move too far, the light in spotlight would then be darker than outside (since outside spotlight we have ambient lighting).
        diffuse  *= attenuation;
        specular *= attenuation;
        
        return (ambient + diffuse + specular);
      
    }
    else    // else, use ambient light so scene isn't completely dark outside the spotlight.
    {
        return light.ambient * material.ambient;
    }
    
    
}


