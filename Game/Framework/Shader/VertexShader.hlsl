cbuffer cBuf
{
	matrix model;
	matrix modelview;
};

float4 main( float3 pos : Position ) : SV_POSITION
{
	return mul( float4( pos,1.0f ),modelview );
}