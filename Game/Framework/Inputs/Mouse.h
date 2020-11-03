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
			Delta,
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
			y( 0 ),
			dx( 0 ),
			dy( 0 )
		{}
		Event( Type type,const Mouse& parent ) noexcept
			:
			type( type ),
			LPresst( parent.leftispresst ),
			RPresst( parent.rightispresst ),
			MPresst( parent.mittelispresst ),
			x( parent.x ),
			y( parent.y ),
			dx( parent.dx ),
			dy( parent.dy )
		{}
		std::pair<int,int> GetPos() const noexcept
		{
			return std::make_pair( x,y );
		}
		std::pair<int,int> GetDelta() const noexcept
		{
			return std::make_pair( dx,dy );
		}
		int GetPosX() const noexcept
		{
			return x;
		}
		int GetPosY() const noexcept
		{
			return y;
		}
		int GetDeltaX() const noexcept
		{
			return dx;
		}
		int GetDeltaY() const noexcept
		{
			return dy;
		}
		bool GetLPresst() const noexcept
		{
			return LPresst;
		}
		bool GetRPresst() const noexcept
		{
			return RPresst;
		}
		bool GetMPresst() const noexcept
		{
			return MPresst;
		}
		Type GetType() const noexcept
		{
			return type;
		}
	private:
		Type type;
		bool LPresst;
		bool RPresst;
		bool MPresst;
		int x;
		int y;
		int dx;
		int dy;
	};
public:
	Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;
	std::pair<int,int> GetPos() const noexcept;
	std::pair<int,int> GetDelta() const noexcept;
	int GetDeltaX() const noexcept;
	int GetDeltaY() const noexcept;
	bool LeftIsPresst() const noexcept;
	bool RightIsPresst() const noexcept;
	bool inWindow() const noexcept;
	Event Read() noexcept;
	void Flush() noexcept;
	bool isEmpty() noexcept;
private:
	void OnMouseMove( int x,int y ) noexcept;
	void OnRawMouse( int x,int y ) noexcept;
	void OnRightPresst( int x,int y ) noexcept;
	void OnLeftPresst( int x,int y ) noexcept;
	void OnRightReleast( int x,int y ) noexcept;
	void OnLeftReleast( int x,int y ) noexcept;
	void OnMittelPresst( int x,int y ) noexcept;
	void OnMittelReleast( int x,int y ) noexcept;
	void OnWheelUp( int x,int y ) noexcept;
	void OnWheelDown( int x,int y ) noexcept;
	void OnEnterWindow( int x,int y ) noexcept;
	void OnLeaveWindow( int x,int y ) noexcept;
	void TrimBuffer() noexcept;
private:
	bool leftispresst = false;
	bool rightispresst = false;
	bool mittelispresst = false;
	bool inwindow = false;
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;
	static constexpr unsigned int MaxBufferSize = 4u;
	std::queue<Event> buffer;
};