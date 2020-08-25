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

void Mouse::MouseMove( int newx,int newy ) noexcept
{
	x = newx;
	y = newy;

	buffer.emplace( Mouse::Event::Type::Move,*this );
	TrimBuffer();
}

void Mouse::RightPresst( int newx,int newy ) noexcept
{
	rightispresst = true;

	buffer.emplace( Event::Type::RightPress,*this );
	TrimBuffer();
}

void Mouse::LeftPresst( int newx,int newy ) noexcept
{
	leftispresst = true;

	buffer.emplace( Event::Type::LeftPress,*this );
	TrimBuffer();
}

void Mouse::RightReleast( int newx,int newy ) noexcept
{
	rightispresst = false;

	buffer.emplace( Event::Type::RightReleas,*this );
	TrimBuffer();
}

void Mouse::LeftReleast( int newx,int newy ) noexcept
{
	leftispresst = false;

	buffer.emplace( Event::Type::LeftReleas,*this );
	TrimBuffer();
}

void Mouse::MittelPresst( int newx,int newy ) noexcept
{
	mittelispresst = true;

	buffer.emplace( Event::Type::MittelPress,*this );
	TrimBuffer();
}

void Mouse::MittelReleast( int newx,int newy ) noexcept
{
	mittelispresst = false;

	buffer.emplace( Event::Type::MittelReleas,*this );
	TrimBuffer();
}

void Mouse::WheelUp( int x,int y ) noexcept
{
	buffer.emplace( Event::Type::WheelUp,*this );
	TrimBuffer();
}

void Mouse::WheelDown( int x,int y ) noexcept
{
	buffer.emplace( Event::Type::WheelDown,*this );
	TrimBuffer();
}

void Mouse::EnterWindow( int x,int y ) noexcept
{
	inwindow = true;

	buffer.emplace( Event::Type::Enter,*this );
	TrimBuffer();
}

void Mouse::LeaveWindow( int x,int y ) noexcept
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
