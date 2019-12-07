#include "..\Functions\LightVectorData.hlsli"
#include "..\Functions\ShaderFunctions.hlsli"

#include "..\Lights\PointLight.hlsli"

cbuffer ObjectConstantBuffer
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};

Texture2D diffuseMap;
Texture2D normalMap : register(t2);


SamplerState splr;

float4 main(float3 viewFragmentPosition : Position, float3 viewNormal : Normal, float3 viewTangent : Tangent, float3 viewBitangent : Bitangent, float2 texCoord : Texcoord) : SV_TARGET
{
    float4 diffuseTex = diffuseMap.Sample(splr, texCoord);
    clip(diffuseTex.a < 0.1f ? -1 : 1);
    
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

    float3 specular = Speculate(diffuseColor, diffuseIntensity, viewNormal, lv.viewFragmentToLight, viewFragmentPosition, att, specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * diffuseTex.rgb + specular), diffuseTex.a);
}