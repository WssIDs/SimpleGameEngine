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
    float3 materialColors[6];
    float padding;
    float specularIntensity;
    float specularPower;
};

float4 main(float3 worldPos : Position, float3 viewNormal : Normal, uint tid : SV_PrimitiveID) : SV_TARGET
{
    // fragment to light vector data
    const float3 vectorToLight = lightPosition - worldPos;
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
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-vReflect), normalize(worldPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient + specular) * materialColors[(tid / 2) % 6]), 1.0f);
}