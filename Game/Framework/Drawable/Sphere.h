#pragma once

#include "DrawableMemberData.h"

#include "../Utility/MorMath.h"

class Fabric_Sphere
{
public:
	template<typename V>
	static std::pair<std::vector<V>,std::vector<unsigned short>> Make( int latDiv,int longDiv )
	{
		namespace dx = DirectX;
		assert( latDiv >= 3 );
		assert( longDiv >= 3 );

		constexpr float radius = 1.0f;
		const auto base = dx::XMVectorSet( 0.0f,0.0f,radius,0.0f );
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		std::vector<V> vertices;
		for ( int iLat = 1; iLat < latDiv; iLat++ )
		{
			const auto latBase = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationX( lattitudeAngle * iLat )
			);
			for ( int iLong = 0; iLong < longDiv; iLong++ )
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					latBase,
					dx::XMMatrixRotationZ( longitudeAngle * iLong )
				);
				dx::XMStoreFloat3( &vertices.back().pos,v );
			}
		}

		// add the cap vertices
		const auto iNorthPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3( &vertices.back().pos,base );
		const auto iSouthPole = (unsigned short)vertices.size();
		vertices.emplace_back();
		dx::XMStoreFloat3( &vertices.back().pos,dx::XMVectorNegate( base ) );

		const auto calcIdx = [latDiv,longDiv]( unsigned short iLat,unsigned short iLong )
		{ return iLat * longDiv + iLong; };
		std::vector<unsigned short> indices;
		for ( unsigned short iLat = 0; iLat < latDiv - 2; iLat++ )
		{
			for ( unsigned short iLong = 0; iLong < longDiv - 1; iLong++ )
			{
				indices.push_back( calcIdx( iLat,iLong ) );
				indices.push_back( calcIdx( iLat + 1,iLong ) );
				indices.push_back( calcIdx( iLat,iLong + 1 ) );
				indices.push_back( calcIdx( iLat,iLong + 1 ) );
				indices.push_back( calcIdx( iLat + 1,iLong ) );
				indices.push_back( calcIdx( iLat + 1,iLong + 1 ) );
			}
			// wrap band
			indices.push_back( calcIdx( iLat,longDiv - 1 ) );
			indices.push_back( calcIdx( iLat + 1,longDiv - 1 ) );
			indices.push_back( calcIdx( iLat,0 ) );
			indices.push_back( calcIdx( iLat,0 ) );
			indices.push_back( calcIdx( iLat + 1,longDiv - 1 ) );
			indices.push_back( calcIdx( iLat + 1,0 ) );
		}

		// cap fans
		for ( unsigned short iLong = 0; iLong < longDiv - 1; iLong++ )
		{
			// north
			indices.push_back( iNorthPole );
			indices.push_back( calcIdx( 0,iLong ) );
			indices.push_back( calcIdx( 0,iLong + 1 ) );
			// south
			indices.push_back( calcIdx( latDiv - 2,iLong + 1 ) );
			indices.push_back( calcIdx( latDiv - 2,iLong ) );
			indices.push_back( iSouthPole );
		}
		// wrap triangles
		// north
		indices.push_back( iNorthPole );
		indices.push_back( calcIdx( 0,longDiv - 1 ) );
		indices.push_back( calcIdx( 0,0 ) );
		// south
		indices.push_back( calcIdx( latDiv - 2,0 ) );
		indices.push_back( calcIdx( latDiv - 2,longDiv - 1 ) );
		indices.push_back( iSouthPole );

		return { std::move( vertices ),std::move( indices ) };
	}
};

class Sphere : public DrawableMemberData<Sphere>
{
public:
	Sphere( Graphics& gfx,float x,float y,float z,float roll,float pitch,float yaw,int index );
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept;
	DirectX::XMFLOAT4 GetColorXM() const noexcept;
	std::string GetType() const noexcept;
	bool SpawnControlWindow() noexcept override;
};