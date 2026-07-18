#version 450 core

struct PointLight {
   vec4 color;
   vec4 position;

   float intensity;
   float radius;
   float unused0;
   float unuseed1;
};

struct DirectionalLight {
   vec4 color;
   vec4 direction;

   float ambientStrength;
   float intensity;
   float unused0;
   float unuseed1;
};

struct Spotlight {
   vec4 color;
   vec4 direction;
   vec4 position;

   float intensity;
   float radius;
   float innerCutOff;
   float outerCutOff;
};

layout (std140, binding = 0) uniform LightBlock {

   DirectionalLight dirLight;

   int pointLightCount;
   int unused3;
   int unused4;
   int unused5;
   
   PointLight pointLights[32];

   int spotlightCount;
   int unused6;
   int unused7;
   int unused8;

   Spotlight spotlights[12];
} lighting;


in vec4 vertColor;
in vec2 texCoord;

in vec3 fragPos;
in vec3 normal;
in vec3 viewPos;

out vec4 fragColor;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform vec4 uColor;
uniform float uSpecularStrength;
uniform float uShininess;

float calculateAttenuation(float dist, float radius) {

   float distFallOff = 1.0 / (dist * dist + 1.0);

   float ratio = dist / radius;

   float ratio4 = ratio * ratio * ratio * ratio;
   float windowing = clamp(1.0 - ratio4, 0.0, 1.0);

   windowing = windowing * windowing;

   float attenuation = distFallOff * windowing;
   return attenuation;
}

vec3 calculateDirLight(DirectionalLight dirLight, vec3 viewDir) {

   vec3 result;

   vec3 lightColor = dirLight.color.rgb;
   float intensity = dirLight.intensity;

   vec3 norm = normalize(normal);

   vec3 lightDir = normalize(-dirLight.direction.xyz);
   float diff = max(dot(norm, lightDir), 0.0);
   
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
   
   vec3 diffuse = diff * lightColor * vec3(texture(diffuseMap, texCoord));
   vec3 specular = uSpecularStrength * spec * lightColor *  vec3(texture(specularMap, texCoord));

   result =  diffuse + specular;
   return result;
}

vec3 calculatePointLight(PointLight pointLight, vec3 viewDir) {

   vec3 result;

   vec3 lightColor = pointLight.color.rgb;
   vec3 lightPos = pointLight.position.xyz;

   vec3 norm = normalize(normal);

   vec3 lightDir = normalize(lightPos - fragPos);
   float diff = max(dot(norm, lightDir), 0.0);

   vec3 reflectDir = reflect(-lightDir, norm);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);

   vec3 diffuse = lightColor * diff * vec3(texture(diffuseMap, texCoord));
   vec3 specular = uSpecularStrength * spec * lightColor * vec3(texture(specularMap, texCoord));
   //uSpecularStrength 
   float dist = length(lightPos - fragPos); 
   float attenuation = calculateAttenuation(dist, pointLight.radius);

   result = (diffuse + specular) * attenuation * pointLight.intensity;

   return result;
} 

vec3 calculateSpotlight(Spotlight spotlight, vec3 viewDir) {

   vec3 result = vec3(0.0);

   vec3 lightPos = spotlight.position.xyz;
   vec3 lightColor = spotlight.color.rgb;
   vec3 lightDir = normalize(lightPos - fragPos);

   float theta = dot(lightDir, normalize(-spotlight.direction.xyz));
   float epsilon =  spotlight.innerCutOff - spotlight.outerCutOff;
   float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);

   vec3 norm = normalize(normal);

   float diff = max(dot(norm, lightDir), 0.0);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);

   vec3 diffuse = lightColor * diff * vec3(texture(diffuseMap, texCoord));
   vec3 specular = uSpecularStrength * spec * lightColor * vec3(texture(specularMap, texCoord));

   float dist = length(lightPos - fragPos); 
   float attenuation = calculateAttenuation(dist, spotlight.radius);

   result = (diffuse + specular) * attenuation * spotlight.intensity * intensity;
   return result;
}

void main() 
{  
   //vec3 diffMap = vec3(texture(diffuseMap, texCoord));
   //vec3 specMap = vec3(texture(specularMap, texCoord));


   //float ambientStrength = 1.0;

   float ambientStrength = lighting.dirLight.ambientStrength;

   vec3 ambient = ambientStrength * vec3(texture(diffuseMap, texCoord));

   vec3 viewDir = normalize(viewPos - fragPos);
   
   vec3 result = vec3(0.0);

   //result += calculateDirLight(lighting.dirLight, viewDir);

   for (int i = 0; i < lighting.pointLightCount; i++) {
      result += calculatePointLight(lighting.pointLights[i], viewDir);
   }

   for (int i = 0; i < lighting.spotlightCount; i++) {
      result += calculateSpotlight(lighting.spotlights[i], viewDir);
   }

   result += ambient;
   fragColor =  vec4(result, 1.0f);
   //fragColor = texture(specularMap, texCoord);
}  
