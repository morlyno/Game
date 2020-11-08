#pragma once

#include <vector>
#include <cassert>
#include <DirectXMath.h>
#include <utility>
#include "../Utility/MorMath.h"
#include "../Utility/MorUtility.h"
#include "../VertexLayout.h"

namespace Geometry
{
	template<typename V>
	class IndexTriangleList
	{
	public:
		IndexTriangleList() = default;
		IndexTriangleList( std::vector<V>&& vertices,std::vector<unsigned short>&& indices )
			:
			vertices( std::move( vertices ) ),
			indices( std::move( indices ) )
		{
			assert( vertices.size() % 3 == 0 );
			assert( indices.size() % 3 == 0 );
		}
	public:
		std::vector<V> vertices;
		std::vector<unsigned short> indices;
	};
	namespace Sphere
	{
		template<typename V>
		static IndexTriangleList<V> MakeTeselected( int latDiv,int longDiv )
		{
			namespace dx = DirectX;
			assert( latDiv >= 3 );
			assert( longDiv >= 3 );

			constexpr float radius = 0.5f;
			const auto base = dx::XMVectorSet( 0.0f,0.0f,radius,0.0f );
			const float lattitudeAngle = Mor::PI / latDiv;
			const float longitudeAngle = 2.0f * Mor::PI / longDiv;

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
		template<typename V>
		static IndexTriangleList<V> Make()
		{
			return Geometry::Sphere::MakeTeselected<V>( 12,24 );
		}
		static void MakeTeselceted( VertexData& vd,std::vector<unsigned short>& indices,int latDiv,int longDiv )
		{
			namespace dx = DirectX;
			assert( latDiv >= 3 );
			assert( longDiv >= 3 );

			constexpr float radius = 0.5;
			const auto base = dx::XMVectorSet( 0.0f,0.0f,radius,0.0f );
			const float lattitudeAngle = Mor::PI / latDiv;
			const float longitudeAngle = 2.0f * Mor::PI / longDiv;

			for ( int iLat = 1; iLat < latDiv; iLat++ )
			{
				const auto latBase = dx::XMVector3Transform(
					base,
					dx::XMMatrixRotationX( lattitudeAngle * iLat )
				);
				for ( int iLong = 0; iLong < longDiv; iLong++ )
				{
					vd.Resize( vd.Size() + 1u );
					auto v = dx::XMVector3Transform(
						latBase,
						dx::XMMatrixRotationZ( longitudeAngle * iLong )
					);
					dx::XMStoreFloat3( &vd.back().Get<VertexLayout::Position3D>(),v );
				}
			}

			// add the cap vertices
			const auto iNorthPole = (unsigned short)vd.Size();
			vd.Resize( vd.Size() + 1u );
			dx::XMStoreFloat3( &vd.back().Get<VertexLayout::Position3D>(),base );
			const auto iSouthPole = (unsigned short)vd.Size();
			vd.Resize( vd.Size() + 1u );
			dx::XMStoreFloat3( &vd.back().Get<VertexLayout::Position3D>(),dx::XMVectorNegate( base ) );

			const auto calcIdx = [latDiv,longDiv]( unsigned short iLat,unsigned short iLong )
			{ return iLat * longDiv + iLong; };
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
		}
		static void Make( VertexData& vd,std::vector<unsigned short>& indices )
		{
			MakeTeselceted( vd,indices,12,24 );
		}
	}
	namespace Cube
	{
		template<typename V>
		static IndexTriangleList<V> Make()
		{
			namespace dx = DirectX;

			constexpr float side = 0.5f;

			std::vector<V> vertices( 8 );
			vertices[0].pos = { -side,-side,-side };
			vertices[1].pos = { side,-side,-side };
			vertices[2].pos = { -side,side,-side };
			vertices[3].pos = { side,side,-side };
			vertices[4].pos = { -side,-side,side };
			vertices[5].pos = { side,-side,side };
			vertices[6].pos = { -side,side,side };
			vertices[7].pos = { side,side,side };

			return{
				std::move( vertices ),{
					0,2,1, 2,3,1,
					1,3,5, 3,7,5,
					2,6,3, 3,6,7,
					4,5,7, 4,7,6,
					0,4,2, 2,4,6,
					0,1,4, 1,5,4
				}
			};
		}
		template<typename V>
		static IndexTriangleList<V> MakeIndipendent()
		{
			constexpr float side = 0.5f;

			std::vector<V> vertices( 24 );
			vertices[0].pos = { -side,-side,-side };// 0 near side
			vertices[1].pos = { side,-side,-side };// 1
			vertices[2].pos = { -side,side,-side };// 2
			vertices[3].pos = { side,side,-side };// 3
			vertices[4].pos = { -side,-side,side };// 4 far side
			vertices[5].pos = { side,-side,side };// 5
			vertices[6].pos = { -side,side,side };// 6
			vertices[7].pos = { side,side,side };// 7
			vertices[8].pos = { -side,-side,-side };// 8 left side
			vertices[9].pos = { -side,side,-side };// 9
			vertices[10].pos = { -side,-side,side };// 10
			vertices[11].pos = { -side,side,side };// 11
			vertices[12].pos = { side,-side,-side };// 12 right side
			vertices[13].pos = { side,side,-side };// 13
			vertices[14].pos = { side,-side,side };// 14
			vertices[15].pos = { side,side,side };// 15
			vertices[16].pos = { -side,-side,-side };// 16 bottom side
			vertices[17].pos = { side,-side,-side };// 17
			vertices[18].pos = { -side,-side,side };// 18
			vertices[19].pos = { side,-side,side };// 19
			vertices[20].pos = { -side,side,-side };// 20 top side
			vertices[21].pos = { side,side,-side };// 21
			vertices[22].pos = { -side,side,side };// 22
			vertices[23].pos = { side,side,side };// 23

			return{
				std::move( vertices ),{
					0,2, 1,    2,3,1,
					4,5, 7,    4,7,6,
					8,10, 9,  10,11,9,
					12,13,15, 12,15,14,
					16,17,18, 18,17,19,
					20,23,21, 20,22,23
				}
			};
		}
		template<typename V>
		static IndexTriangleList<V> MakeIndipendetTextured()
		{
			auto mesh = Geometry::Cube::MakeIndipendent<V>();
			mesh.vertices[0].tc = { 0.0f,0.0f };
			mesh.vertices[1].tc = { 1.0f,0.0f };
			mesh.vertices[2].tc = { 0.0f,1.0f };
			mesh.vertices[3].tc = { 1.0f,1.0f };
			mesh.vertices[4].tc = { 0.0f,0.0f };
			mesh.vertices[5].tc = { 1.0f,0.0f };
			mesh.vertices[6].tc = { 0.0f,1.0f };
			mesh.vertices[7].tc = { 1.0f,1.0f };
			mesh.vertices[8].tc = { 0.0f,0.0f };
			mesh.vertices[9].tc = { 1.0f,0.0f };
			mesh.vertices[10].tc = { 0.0f,1.0f };
			mesh.vertices[11].tc = { 1.0f,1.0f };
			mesh.vertices[12].tc = { 0.0f,0.0f };
			mesh.vertices[13].tc = { 1.0f,0.0f };
			mesh.vertices[14].tc = { 0.0f,1.0f };
			mesh.vertices[15].tc = { 1.0f,1.0f };
			mesh.vertices[16].tc = { 0.0f,0.0f };
			mesh.vertices[17].tc = { 1.0f,0.0f };
			mesh.vertices[18].tc = { 0.0f,1.0f };
			mesh.vertices[19].tc = { 1.0f,1.0f };
			mesh.vertices[20].tc = { 0.0f,0.0f };
			mesh.vertices[21].tc = { 1.0f,0.0f };
			mesh.vertices[22].tc = { 0.0f,1.0f };
			mesh.vertices[23].tc = { 1.0f,1.0f };

			return std::move( mesh );
		}
		static void MakeIndipendent( VertexData& vd )
		{
			constexpr float side = 0.5f;

			vd.Resize( 24 );

			vd[0].Get<VertexLayout::Position3D>() = { -side,-side,-side };// 0 near side
			vd[1].Get<VertexLayout::Position3D>() = { side,-side,-side };// 1
			vd[2].Get<VertexLayout::Position3D>() = { -side,side,-side };// 2
			vd[3].Get<VertexLayout::Position3D>() = { side,side,-side };// 3
			vd[4].Get<VertexLayout::Position3D>() = { -side,-side,side };// 4 far side
			vd[5].Get<VertexLayout::Position3D>() = { side,-side,side };// 5
			vd[6].Get<VertexLayout::Position3D>() = { -side,side,side };// 6
			vd[7].Get<VertexLayout::Position3D>() = { side,side,side };// 7
			vd[8].Get<VertexLayout::Position3D>() = { -side,-side,-side };// 8 left side
			vd[9].Get<VertexLayout::Position3D>() = { -side,side,-side };// 9
			vd[10].Get<VertexLayout::Position3D>() = { -side,-side,side };// 10
			vd[11].Get<VertexLayout::Position3D>() = { -side,side,side };// 11
			vd[12].Get<VertexLayout::Position3D>() = { side,-side,-side };// 12 right side
			vd[13].Get<VertexLayout::Position3D>() = { side,side,-side };// 13
			vd[14].Get<VertexLayout::Position3D>() = { side,-side,side };// 14
			vd[15].Get<VertexLayout::Position3D>() = { side,side,side };// 15
			vd[16].Get<VertexLayout::Position3D>() = { -side,-side,-side };// 16 bottom side
			vd[17].Get<VertexLayout::Position3D>() = { side,-side,-side };// 17
			vd[18].Get<VertexLayout::Position3D>() = { -side,-side,side };// 18
			vd[19].Get<VertexLayout::Position3D>() = { side,-side,side };// 19
			vd[20].Get<VertexLayout::Position3D>() = { -side,side,-side };// 20 top side
			vd[21].Get<VertexLayout::Position3D>() = { side,side,-side };// 21
			vd[22].Get<VertexLayout::Position3D>() = { -side,side,side };// 22
			vd[23].Get<VertexLayout::Position3D>() = { side,side,side };// 23
		}
		static auto MakeIndicesIndipendent()
		{
			std::vector<unsigned short> indices =
			{
				 0, 2, 1,  2, 3, 1,
				 4, 5, 7,  4, 7, 6,
				 8,10, 9, 10,11, 9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			};
			return std::move( indices );
		}
	}
	namespace Plane
	{
		template<typename V>
		static IndexTriangleList<V> Make()
		{
			namespace dx = DirectX;

			constexpr float side = 0.5f;

			std::vector<V> vertex( 4 );
			vertex[0].pos = { -side,side,0.0f };
			vertex[1].pos = { side,side,0.0f };
			vertex[2].pos = { side,-side,0.0f };
			vertex[3].pos = { -side,-side,0.0f };

			return {
				std::move( vertex ),
				{
					0,1,2,
					2,3,0
				}
			};
		}
		template<typename V>
		static IndexTriangleList<V> MakeTextured()
		{
			auto mesh = Geometry::Plane::Make<V>();

			mesh.vertices[0].tc = { 0.0f,0.0f };
			mesh.vertices[1].tc = { 1.0f,0.0f };
			mesh.vertices[2].tc = { 1.0f,1.0f };
			mesh.vertices[3].tc = { 0.0f,1.0f };

			return std::move( mesh );
		}
	}
	template<typename V>
	static void MakeNormals( IndexTriangleList<V>& itl )
	{
		using namespace DirectX;
		//namespace dx = DirectX;

		for ( size_t i = 0; i < itl.indices.size(); i += 3 )
		{
			auto& v0 = itl.vertices[itl.indices[i]];
			auto& v1 = itl.vertices[itl.indices[i + 1]];
			auto& v2 = itl.vertices[itl.indices[i + 2]];

			const auto p0 = XMLoadFloat3( &v0.pos );
			const auto p1 = XMLoadFloat3( &v1.pos );
			const auto p2 = XMLoadFloat3( &v2.pos );

			const auto n = XMVector3Normalize( XMVector3Cross( (p1 - p0),(p2 - p0) ) );

			XMStoreFloat3( &v0.n,n );
			XMStoreFloat3( &v1.n,n );
			XMStoreFloat3( &v2.n,n );
		}
	}
	template<class Indices>
	static void MakeNormals( VertexData& vd,const Indices& indices )
	{
		using namespace DirectX;

		for ( size_t i = 0; i < indices.size(); i += 3 )
		{
			auto v0 = vd[indices[i]];
			auto v1 = vd[indices[i + 1]];
			auto v2 = vd[indices[i + 2]];

			const auto p0 = XMLoadFloat3( &v0.Get<VertexLayout::Position3D>() );
			const auto p1 = XMLoadFloat3( &v1.Get<VertexLayout::Position3D>() );
			const auto p2 = XMLoadFloat3( &v2.Get<VertexLayout::Position3D>() );

			const auto n = XMVector3Normalize( XMVector3Cross( (p1 - p0),(p2 - p0) ) );

			XMStoreFloat3( &v0.Get<VertexLayout::Normal>(),n );
			XMStoreFloat3( &v1.Get<VertexLayout::Normal>(),n );
			XMStoreFloat3( &v2.Get<VertexLayout::Normal>(),n );
		}
	}
	template<class Indices>
	static void MakeTanBitan( VertexData& vd,const Indices& indices )
	{
		using namespace DirectX;
		
		for ( size_t i = 0; i < indices.size(); i += 3 )
		{
			const auto& vd0 = vd[indices[i + 0]];
			const auto& vd1 = vd[indices[i + 1]];
			const auto& vd2 = vd[indices[i + 2]];

			const auto& v0 = vd0.Get<VertexLayout::Position3D>();
			const auto& v1 = vd1.Get<VertexLayout::Position3D>();
			const auto& v2 = vd2.Get<VertexLayout::Position3D>();

			const auto& t0 = vd0.Get<VertexLayout::Texture2D>();
			const auto& t1 = vd1.Get<VertexLayout::Texture2D>();
			const auto& t2 = vd2.Get<VertexLayout::Texture2D>();

			const auto edge0 = Mor::XMFloat3Sub( v1,v0 );
			const auto edge1 = Mor::XMFloat3Sub( v2,v0 );
			const auto dtex0 = Mor::XMFloat2Sub( t1,t0 );
			const auto dtex1 = Mor::XMFloat2Sub( t2,t0 );

			float f = 1.0f / (dtex0.x * dtex1.y - dtex1.x * dtex0.y);

			VertexLayout::Map<VertexLayout::Tangent>::sysType tan;
			tan.x = f * (dtex1.y * edge0.x - dtex0.y * edge1.x);
			tan.y = f * (dtex1.y * edge0.y - dtex0.y * edge1.y);
			tan.z = f * (dtex1.y * edge0.z - dtex0.y * edge1.z);

			VertexLayout::Map<VertexLayout::Bitangent>::sysType bitan;
			bitan.z = f * (-dtex1.x * edge0.z + dtex0.x * edge1.z);
			bitan.x = f * (-dtex1.x * edge0.x + dtex0.x * edge1.x);
			bitan.y = f * (-dtex1.x * edge0.y + dtex0.x * edge1.y);

			vd0.Get<VertexLayout::Tangent>() = tan;
			vd1.Get<VertexLayout::Tangent>() = tan;
			vd2.Get<VertexLayout::Tangent>() = tan;

			vd0.Get<VertexLayout::Bitangent>() = bitan;
			vd1.Get<VertexLayout::Bitangent>() = bitan;
			vd2.Get<VertexLayout::Bitangent>() = bitan;
		}
	}
}