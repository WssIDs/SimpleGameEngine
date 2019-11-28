cbuffer LigthConstantBuffer
{
    float3 lightPosition;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectConstantBuffer
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D tex;

SamplerState splr;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float2 texCoord : Texcoord) : SV_TARGET
{
    // fragment to light vector data
    const float3 vectorToLight = lightPosition - viewPos;
    const float distanceToLight = length(vectorToLight);
    const float3 directionToLight = vectorToLight / distanceToLight;

    // attenuation
    const float att = 1.0f / (attConst + attLin * distanceToLight + attQuad * (distanceToLight * distanceToLight));
    // diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(directionToLight, viewNormal));
    // reflected light vector
    const float3 vViewer = viewNormal * dot(vectorToLight, viewNormal);
    const float3 vReflect = 2.0f * vViewer - vectorToLight;
    // specular ( angle between viewer vector and reflect vector)
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize( -vReflect), normalize(viewPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, texCoord).rgb + specular), 1.0f);
}