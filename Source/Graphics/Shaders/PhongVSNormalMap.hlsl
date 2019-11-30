cbuffer ConstantBuffer
{
    matrix modelView;
    matrix modelViewProj;
};

struct VSOut
{
    float3 viewPos : Position;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float3 bitangent : Bitangent;
    float2 texCoord : Texcoord;
    float4 position : SV_Position;
};

VSOut main(float3 position : Position, float3 normal : Normal, float3 tangent : Tangent, float3 bitangent : Bitangent, float2 texCoord : Texcoord)
{
    VSOut vso;
    vso.viewPos = (float3) mul(float4(position, 1.0f), modelView);
    vso.normal = mul(normal, (float3x3) modelView);
    vso.normal = normalize(vso.normal);
    vso.tangent = mul(tangent, (float3x3) modelView);
    vso.bitangent = mul(bitangent, (float3x3) modelView);
    vso.position = mul(float4(position, 1.0f), modelViewProj);
    vso.texCoord = texCoord;
    return vso;
}