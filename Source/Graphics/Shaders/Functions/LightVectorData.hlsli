struct LightVectorData
{
    float3 viewFragmentToLight;
    float distanceFragmentToLight;
    float3 viewDirectionFragmentToLight;
};



LightVectorData CalculateLightVectorData(const in float3 lightPosition, const in float3 fragmentPosition)
{
    LightVectorData lv;
    // fragment to light vector data
    lv.viewFragmentToLight = lightPosition - fragmentPosition;
    lv.distanceFragmentToLight = length(lv.viewFragmentToLight);
    lv.viewDirectionFragmentToLight = lv.viewFragmentToLight / lv.distanceFragmentToLight;
    
    return lv;
}