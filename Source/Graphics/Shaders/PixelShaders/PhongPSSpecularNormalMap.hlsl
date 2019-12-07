#include "..\Functions\LightVectorData.hlsli"
#include "..\Functions\ShaderFunctions.hlsli"

#include "..\Lights\PointLight.hlsli"

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

float4 main(float3 viewFragmentPosition : Position, float3 viewNormal : Normal, float3 viewTangent : Tangent, float3 viewBitangent : Bitangent, float2 texCoord : Texcoord) : SV_TARGET
{
    float4 diffuseTex = diffuseMap.Sample(splr, texCoord);
    clip(diffuseTex.a < 0.1f ? -1 : 1);
    
     // flip normal when backface
    if (dot(viewNormal, viewFragmentPosition) >= 0.0f)
    {
        viewNormal = -viewNormal;
    }
    
    viewNormal = normalize(viewNormal);
    // sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
        viewNormal = MapNormal(normalize(viewTangent), normalize(viewBitangent), viewNormal, texCoord, normalMap, splr);
    }
    
    // fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPosition, viewFragmentPosition);

    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distanceFragmentToLight);
    // diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.viewDirectionFragmentToLight, viewNormal);
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
    
    float3 specularReflected = Speculate(specularReflectionColor, 1.0f, viewNormal, lv.viewFragmentToLight, viewFragmentPosition, att, specularPower);
    
    const float4 glossinessSample = glossinessMap.Sample(splr, texCoord);
    if (glossinesMapEnabled)
    {
        specularReflected = specularReflected * glossinessSample.rgb;
    }
	// final color
    return float4(saturate((diffuse + ambient) * diffuseTex.rgb + specularReflected), diffuseTex.a);
}