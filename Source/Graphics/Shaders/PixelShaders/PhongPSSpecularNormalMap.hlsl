#include "..\Lights\PointLight.hlsl"
#include "..\Functions\ShaderFunctions.hlsl"

cbuffer ObjectConstantBuffer
{
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool specularMapAlpha;
    bool glossinesMapEnabled;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
    float padding[3];
};

Texture2D diffuseMap;
Texture2D specularMap;
Texture2D glossinessMap;
Texture2D normalMap;


SamplerState splr;

float4 main(float3 viewPosition : Position, float3 viewNormal : Normal, float3 viewTangent : Tangent, float3 viewBitangent : Bitangent, float2 texCoord : Texcoord) : SV_TARGET
{
    viewNormal = normalize(viewNormal);
    // sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
        viewNormal = MapNormal(normalize(viewTangent), normalize(viewBitangent), viewNormal, texCoord, normalMap, splr);
    }
    
    // fragment to light vector data
    const float3 viewFragmentToLight = viewLightPosition - viewPosition;
    const float distanceFragmentToLight = length(viewFragmentToLight);
    const float3 viewDirectionFragmentToLight = viewFragmentToLight / distanceFragmentToLight;

    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, distanceFragmentToLight);
    // diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, viewDirectionFragmentToLight, viewNormal);
    // specular ( angle between viewer vector and reflect vector)
    float3 specularReflectionColor;  
    float specularPower = specularPowerConst;

    if (specularMapEnabled)
    {
        const float4 specularSample = specularMap.Sample(splr, texCoord);
        specularReflectionColor = specularSample.rgb * specularMapWeight;
        
        if (specularMapAlpha)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
    }
    else
    {
        specularReflectionColor = specularColor;
    }
    
    float3 specularReflected = Speculate(specularReflectionColor, 1.0f, viewNormal, viewFragmentToLight, viewPosition, att, specularPower);
    
    const float4 glossinessSample = glossinessMap.Sample(splr, texCoord);
    if (glossinesMapEnabled)
    {
        specularReflected = specularReflected * glossinessSample.rgb;
    }
	// final color
    return float4(saturate((diffuse + ambient) * diffuseMap.Sample(splr, texCoord).rgb + specularReflected), 1.0f);
}