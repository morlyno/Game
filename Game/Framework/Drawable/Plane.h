#pragma once

#include "DrawableBase.h"
#include "DrawableMemberData.h"

class Squeare
{
public:
	template<typename V>
	static std::pair<std::vector<V>,std::vector<unsigned short>> MakeTeselected( int divishions_x,int divishions_y ) noexcept
	{
		namespace dx = DirectX;

		const float width = 2.0f;
		const float height = 2.0f;

		const float side_x = width / 2.0f; // 1
		const float side_y = height / 2.0f; // 1

		const int nVectices_x = divishions_x + 2; // 3
		const int nVectices_y = divishions_y + 2; // 3

		const float add_x = width / (divishions_x + 1); // 1
		const float add_y = height / (divishions_y + 1); // 1

		const auto botemleft = dx::XMVectorSet( -side_x,-side_y,0.0f,0.0f );
		std::vector<V> vertices( nVectices_x * nVectices_y );
		for ( int y = 0; y < nVectices_y; ++y )
		{
			for ( int x = 0; x < nVectices_x; ++x )
			{
				const auto c = dx::XMVectorAdd(
					botemleft,
					dx::XMVectorSet( (float)x * add_x,(float)y * add_y,0.0f,0.0f )
				);
				dx::XMStoreFloat3( &vertices[y * (int)nVectices_x + x].pos,c );
			}
		}
		
		const int ntriangle = ((divishions_y * 2) + 2) * (divishions_x + 1);
		const int nIndices = ntriangle * 3;
		std::vector<unsigned short> indices( nIndices );
		
		/*int add = -1;
		for ( int i = 0; i < nTriangle / 2; ++i )
		{
			const int nextindex = 6 * i;
			add = (i % (nVectices_x - 1) == 0 && add != -1) ? add += 2 : ++add;
			indices[0 + nextindex] = 0 + add;
			indices[1 + nextindex] = 3 + add;
			indices[2 + nextindex] = 1 + add;

			indices[3 + nextindex] = 3 + add;
			indices[4 + nextindex] = 4 + add;
			indices[5 + nextindex] = 1 + add;
		}*/

		int i = 0;
		for ( int y = 0; y < nVectices_y - 1; ++y )
		{
			const int lineadd = y * nVectices_x;
			for ( int x = 0; x < nVectices_x - 1; ++x )
			{
				indices[i + 0] = x + lineadd;
				indices[i + 1] = nVectices_x + x + lineadd;
				indices[i + 2] = x + 1 + lineadd;
				indices[i + 3] = nVectices_x + x + lineadd;
				indices[i + 4] = nVectices_x + 1 + x + lineadd;
				indices[i + 5] = x + 1 + lineadd;
				i += 6;
			}
		}

		return { std::move( vertices ),std::move( indices ) };
	}
	template<typename V>
	static std::pair<std::vector<V>,std::vector<unsigned short>> Make() noexcept
	{
		return Squeare::MakeTeselected<V>( 0,0 );
	}
	template<typename T>
	static void SacleVertices( std::vector<T>& vec,float width,float height,float depth) noexcept
	{
		const auto matrix = DirectX::XMMatrixScaling( width,height,depth );
		for ( auto& v : vec )
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3( &v.pos );
			DirectX::XMStoreFloat3(
				&v.pos,
				DirectX::XMVector3Transform( pos,matrix )
			);
		}
	}
};

class Plane : public DrawableMemberData<Plane>
{
public:
	Plane( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,float sx,float sy,float sz,int index );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	DirectX::XMFLOAT4 GetColorXM() const noexcept override;
	std::string GetType() const noexcept override;
};