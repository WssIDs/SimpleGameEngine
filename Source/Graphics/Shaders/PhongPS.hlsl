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
    float3 materialColor;
};

float4 main(float3 worldPos : Position, float3 viewNormal : Normal) : SV_TARGET
{
    // fragment to light vector data
    const float3 vectorToLight = lightPosition - worldPos;
    const float distanceToLight = length(vectorToLight);
    const float3 directionToLight = vectorToLight / distanceToLight;

    // diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distanceToLight + attQuad * (distanceToLight * distanceToLight));
    // diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(directionToLight, viewNormal));
	// final color
    return float4(saturate((diffuse + ambient) * materialColor), 1.0f);
}