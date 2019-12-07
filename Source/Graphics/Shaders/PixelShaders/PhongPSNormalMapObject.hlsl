#include "..\Lights\PointLight.hlsli"
#include "..\Functions\LightVectorData.hlsli"
#include "..\Functions\ShaderFunctions.hlsli"

cbuffer ObjectConstantBuffer
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};

#include "..\Buffers\Transform.hlsli"


Texture2D diffuseMap;
Texture2D normalMap : register(t2);

SamplerState splr;


float4 main(float3 viewFragmentPosition : Position, float3 viewNormal : Normal, float2 texCoord : Texcoord) : SV_Target
{
    float4 diffuseTex = diffuseMap.Sample(splr, texCoord);
    clip(diffuseTex.a < 0.1f ? -1 : 1);
    
	// sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
        // sample and unpack normal data
        const float3 normalSample = normalMap.Sample(splr, texCoord).xyz;
        const float3 objectNormal = normalSample * 2.0f - 1.0f;

        viewNormal = normalize(mul(objectNormal, (float3x3) modelView));
    }
    
    // fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPosition, viewFragmentPosition);

    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distanceFragmentToLight);
    // diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.viewDirectionFragmentToLight, viewNormal);
    
    const float3 specular = Speculate(specularIntensity.rrr, 1.0f, viewNormal, lv.viewFragmentToLight, viewFragmentPosition, att, specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * diffuseTex.rgb + specular), diffuseTex.a);
}