Texture2D t2d;

SamplerState smpl;

float4 main( float2 tex : TexCoord ) : SV_Target
{
	return t2d.Sample( smpl,tex );
}