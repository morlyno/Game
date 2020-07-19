#pragma once

#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Move,
			LeftPress,
			RightPress,
			LeftReleas,
			RightReleas,
			MittelPress,
			MittelReleas,
			WheelUp,
			WheelDown,
			Invalid
		};
		Event()
			:
			type( Type::Invalid ),
			LPresst( false ),
			RPresst( false ),
			MPresst( false ),
			x( 0 ),
			y( 0 )
		{}
		Event( Type type,const Mouse& parent )
			:
			type( type ),
			LPresst( parent.leftispresst ),
			RPresst( parent.rightispresst ),
			MPresst( parent.mittelispresst ),
			x( parent.x ),
			y( parent.y )
		{}
		std::pair<int,int> getPos() const
		{
			return std::make_pair( x,y );
		}
		int getPosX() const
		{
			return x;
		}
		int getPosY() const
		{
			return y;
		}
		bool getLPresst() const
		{
			return LPresst;
		}
		bool getRPresst() const
		{
			return RPresst;
		}
		bool getMPresst() const
		{
			return MPresst;
		}
	private:
		Type type;
		bool LPresst;
		bool RPresst;
		bool MPresst;
		int x;
		int y;
	};
public:
	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;
	std::pair<int,int> getPos() const;
	bool LeftIsPresst() const;
	bool RightIsPresst() const;
	bool inWindow() const;
	Event Read();
	void Flush();
	bool isEmpty();
private:
	void MouseMove( int x,int y );
	void RightPresst( int x,int y );
	void LeftPresst( int x,int y );
	void RightReleast( int x,int y );
	void LeftReleast( int x,int y );
	void MittelPresst( int x,int y );
	void MittelReleast( int x,int y );
	void WheelUp( int x,int y );
	void WheelDown( int x,int y );
	void LeaveWindow( int x,int y );
	void TrimBuffer();
private:
	bool leftispresst = false;
	bool rightispresst = false;
	bool mittelispresst = false;
	bool inwindow = false;
	int x = 0;
	int y = 0;
	static constexpr unsigned int MaxBufferSize = 4u;
public:
	std::queue<Event> buffer;
};