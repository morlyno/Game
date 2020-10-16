#include "Mouse.h"

std::pair<int,int> Mouse::getPos() const noexcept
{
	return std::make_pair( x,y );
}

bool Mouse::LeftIsPresst() const noexcept
{
	return leftispresst;
}

bool Mouse::RightIsPresst() const noexcept
{
	return rightispresst;
}

bool Mouse::inWindow() const noexcept
{
	return inwindow;
}

Mouse::Event Mouse::Read() noexcept
{
	if ( buffer.size() > 0u )
	{
		auto e = buffer.front();
		TrimBuffer();
		return e;
	}
	return Mouse::Event();
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Event>();
}

bool Mouse::isEmpty() noexcept
{
	return buffer.empty();
}

void Mouse::OnMouseMove( int newx,int newy ) noexcept
{
	x = newx;
	y = newy;

	buffer.emplace( Mouse::Event::Type::Move,*this );
	TrimBuffer();
}

void Mouse::OnRightPresst( int newx,int newy ) noexcept
{
	rightispresst = true;

	buffer.emplace( Event::Type::RightPress,*this );
	TrimBuffer();
}

void Mouse::OnLeftPresst( int newx,int newy ) noexcept
{
	leftispresst = true;

	buffer.emplace( Event::Type::LeftPress,*this );
	TrimBuffer();
}

void Mouse::OnRightReleast( int newx,int newy ) noexcept
{
	rightispresst = false;

	buffer.emplace( Event::Type::RightReleas,*this );
	TrimBuffer();
}

void Mouse::OnLeftReleast( int newx,int newy ) noexcept
{
	leftispresst = false;

	buffer.emplace( Event::Type::LeftReleas,*this );
	TrimBuffer();
}

void Mouse::OnMittelPresst( int newx,int newy ) noexcept
{
	mittelispresst = true;

	buffer.emplace( Event::Type::MittelPress,*this );
	TrimBuffer();
}

void Mouse::OnMittelReleast( int newx,int newy ) noexcept
{
	mittelispresst = false;

	buffer.emplace( Event::Type::MittelReleas,*this );
	TrimBuffer();
}

void Mouse::OnWheelUp( int x,int y ) noexcept
{
	buffer.emplace( Event::Type::WheelUp,*this );
	TrimBuffer();
}

void Mouse::OnWheelDown( int x,int y ) noexcept
{
	buffer.emplace( Event::Type::WheelDown,*this );
	TrimBuffer();
}

void Mouse::OnEnterWindow( int x,int y ) noexcept
{
	inwindow = true;

	buffer.emplace( Event::Type::Enter,*this );
	TrimBuffer();
}

void Mouse::OnLeaveWindow( int x,int y ) noexcept
{
	inwindow = false;

	buffer.emplace( Event::Type::Leave,*this );
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while ( buffer.size() > MaxBufferSize )
	{
		buffer.pop();
	}
}
