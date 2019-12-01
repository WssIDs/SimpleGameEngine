#include "..\Functions\LightVectorData.hlsli"
#include "..\Functions\ShaderFunctions.hlsli"

#include "..\Lights\PointLight.hlsli"

cbuffer ObjectConstantBuffer
{
    float4 materialColor;
    float4 specularColor;
    float specularPower;
};

float4 main(float3 viewFragmentPosition : Position, float3 viewNormal : Normal) : SV_TARGET
{
    viewNormal = normalize(viewNormal);
    // fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPosition, viewFragmentPosition);

    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distanceFragmentToLight);
    // diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.viewDirectionFragmentToLight, viewNormal);
    // specular
    float3 specular = Speculate(diffuseColor, diffuseIntensity, viewNormal, lv.viewFragmentToLight, viewFragmentPosition, att, specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * materialColor.rgb + specular), 1.0f);
}