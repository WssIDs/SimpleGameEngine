#include "..\Functions\LightVectorData.hlsli"
#include "..\Functions\ShaderFunctions.hlsli"

#include "..\Lights\PointLight.hlsli"

cbuffer ObjectConstantBuffer
{
    float specularPowerConst;
    float specularMapWeight;
    bool specularMapAlpha;
};


Texture2D diffuseMap;
Texture2D specularMap;

SamplerState splr;

float4 main(float3 viewFragmentPosition : Position, float3 viewNormal : Normal, float2 texCoord : Texcoord) : SV_Target
{
     // normalize the mesh normal
    viewNormal = normalize(viewNormal);
     
    // fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPosition, viewFragmentPosition);

    // specular ( angle between viewer vector and reflect vector)
    float specularPower = specularPowerConst;
    const float4 specularSample = specularMap.Sample(splr, texCoord);
    const float3 specularReflectionColor = specularSample.rgb * specularMapWeight;
        
    if (specularMapAlpha)
    {
        specularPower = pow(2.0f, specularSample.a * 13.0f);
    }
    
    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distanceFragmentToLight);
    // diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.viewDirectionFragmentToLight, viewNormal);
    
    float3 specularReflected = Speculate(specularReflectionColor, 1.0f, viewNormal, lv.viewFragmentToLight, viewFragmentPosition, att, specularPower);
   
    return float4(saturate((diffuse + ambient) * diffuseMap.Sample(splr, texCoord).rgb + specularReflected), 1.0f);
}