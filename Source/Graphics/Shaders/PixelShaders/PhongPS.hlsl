#include "..\Functions\LightVectorData.hlsli"
#include "..\Functions\ShaderFunctions.hlsli"

#include "..\Lights\PointLight.hlsli"

cbuffer ObjectConstantBuffer
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D diffuseMap;

SamplerState splr;

float4 main(float3 viewFragmentPosition : Position, float3 viewNormal : Normal, float2 texCoord : Texcoord) : SV_TARGET
{
    float4 diffuseTex = diffuseMap.Sample(splr, texCoord);
    
    clip(diffuseTex.a < 0.1f ? -1 : 1);
    
    viewNormal = normalize(viewNormal);
 
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