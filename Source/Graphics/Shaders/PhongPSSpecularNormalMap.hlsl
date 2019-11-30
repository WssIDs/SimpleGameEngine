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
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool specularMapAlpha;
    bool glossinesMapEnabled;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
    float padding[3];
};

Texture2D tex;
Texture2D spec;
Texture2D gloss;
Texture2D nmap;


SamplerState splr;

float4 main(float3 viewPos : Position, float3 viewNormal : Normal, float3 tangent : Tangent, float3 bitangent : Bitangent, float2 texCoord : Texcoord) : SV_TARGET
{
    // sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
        // build the tranform (rotation) into tangent space
        const float3x3 tanToView = float3x3(
            normalize(tangent),
            normalize(bitangent),
            normalize(viewNormal)
        );
        // unpack normal data
        const float3 normalSample = nmap.Sample(splr, texCoord).xyz;
        viewNormal = normalSample * 2.0f - 1.0f;
        // bring normal from tanspace into view space
        viewNormal = mul(viewNormal, tanToView);
    }
    
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
    float3 specularReflectionColor;  
    float specularPower = specularPowerConst;

    if (specularMapEnabled)
    {
        const float4 specularSample = spec.Sample(splr, texCoord);
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
    
    
    
    float3 specular = att * (diffuseColor * diffuseIntensity ) * pow(max(0.0f, dot(normalize(-vReflect), normalize(viewPos))), specularPower);
    
    const float4 glossinessSample = gloss.Sample(splr, texCoord);
    if (glossinesMapEnabled)
    {
        specular = specular * glossinessSample.rgb;
    }
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, texCoord).rgb + specular * specularReflectionColor), 1.0f);
}