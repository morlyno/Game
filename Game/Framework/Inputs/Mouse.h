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
			Enter,
			Leave,
			Invalid
		};
		Event() noexcept
			:
			type( Type::Invalid ),
			LPresst( false ),
			RPresst( false ),
			MPresst( false ),
			x( 0 ),
			y( 0 )
		{}
		Event( Type type,const Mouse& parent ) noexcept
			:
			type( type ),
			LPresst( parent.leftispresst ),
			RPresst( parent.rightispresst ),
			MPresst( parent.mittelispresst ),
			x( parent.x ),
			y( parent.y )
		{}
		std::pair<int,int> getPos() const noexcept
		{
			return std::make_pair( x,y );
		}
		int getPosX() const noexcept
		{
			return x;
		}
		int getPosY() const noexcept
		{
			return y;
		}
		bool getLPresst() const noexcept
		{
			return LPresst;
		}
		bool getRPresst() const noexcept
		{
			return RPresst;
		}
		bool getMPresst() const noexcept
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
	std::pair<int,int> getPos() const noexcept;
	bool LeftIsPresst() const noexcept;
	bool RightIsPresst() const noexcept;
	bool inWindow() const noexcept;
	Event Read() noexcept;
	void Flush() noexcept;
	bool isEmpty() noexcept;
private:
	void MouseMove( int x,int y ) noexcept;
	void RightPresst( int x,int y ) noexcept;
	void LeftPresst( int x,int y ) noexcept;
	void RightReleast( int x,int y ) noexcept;
	void LeftReleast( int x,int y ) noexcept;
	void MittelPresst( int x,int y ) noexcept;
	void MittelReleast( int x,int y ) noexcept;
	void WheelUp( int x,int y ) noexcept;
	void WheelDown( int x,int y ) noexcept;
	void EnterWindow( int x,int y ) noexcept;
	void LeaveWindow( int x,int y ) noexcept;
	void TrimBuffer() noexcept;
private:
	bool leftispresst = false;
	bool rightispresst = false;
	bool mittelispresst = false;
	bool inwindow = false;
	int x = 0;
	int y = 0;
	static constexpr unsigned int MaxBufferSize = 4u;
	std::queue<Event> buffer;
};