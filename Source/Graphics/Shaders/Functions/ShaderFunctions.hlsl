float3 MapNormal(
    const in float3 tangent,
    const in float3 bitangent,
    const in float3 normal,
    const in float2 textureCoordinate,
    uniform Texture2D normalMap,
    uniform SamplerState samplerState)
{
    // build the tranform (rotation) into tangent space
    const float3x3 tanToTarget = float3x3(tangent, bitangent, normal);
    
    // sample and unpack normal data
    const float3 normalSample = normalMap.Sample(samplerState, textureCoordinate).xyz;
    float3 tanNormal = normalSample * 2.0f - 1.0f;
    
    // bring normal from tanspace into view space
    return normalize(mul(tanNormal, tanToTarget));
}

//// Calculation

float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distanceFragmentToLight)
{
    return 1.0f / (attConst + attLin * distanceFragmentToLight + attQuad * (distanceFragmentToLight * distanceFragmentToLight));
}

float3 Diffuse(
    uniform float3 diffuseColor,
    uniform float diffuseIntensity,
    const in float attenuation,
    const in float3 viewDirectionFragmentToLight,
    const in float3 viewNormal)
{
    return diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(viewDirectionFragmentToLight, viewNormal));
}

float3 Speculate(
    const in float3 specularColor,
    uniform float specularIntensity,
    const in float3 viewNormal,
    const in float3 viewFragmentToLight,
    const in float3 viewPosition,
    const in float attenuation,
    const in float specularPower)
{
    // reflected light vector
    const float3 vViewer = viewNormal * dot(viewFragmentToLight, viewNormal);
    const float3 vReflect = normalize(vViewer * 2.0f - viewFragmentToLight);
    // vector from camera to fragment (in view space)
    const float3 viewCamToFrag = normalize(viewPosition);
    // calculate specular component color based on angle between
    // viewing vector and reflection vector, narrow with power function
    return attenuation * specularColor * specularIntensity * pow(max(0.0f, dot(-vReflect, viewCamToFrag)), specularPower);
}