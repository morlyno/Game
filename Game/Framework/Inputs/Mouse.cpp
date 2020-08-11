#include "Mouse.h"

std::pair<int,int> Mouse::getPos() const
{
	return std::make_pair( x,y );
}

bool Mouse::LeftIsPresst() const
{
	return leftispresst;
}

bool Mouse::RightIsPresst() const
{
	return rightispresst;
}

bool Mouse::inWindow() const
{
	return inwindow;
}

Mouse::Event Mouse::Read()
{
	if ( buffer.size() > 0u )
	{
		auto e = buffer.front();
		TrimBuffer();
		return e;
	}
	return Mouse::Event();
}

void Mouse::Flush()
{
	while ( buffer.size() > 0 )
	{
		buffer.pop();
	}
}

bool Mouse::isEmpty()
{
	return buffer.empty();
}

void Mouse::MouseMove( int newx,int newy )
{
	x = newx;
	y = newy;

	buffer.emplace( Mouse::Event::Type::Move,*this );
	TrimBuffer();
}

void Mouse::RightPresst( int newx,int newy )
{
	rightispresst = true;

	buffer.emplace( Event::Type::RightPress,*this );
	TrimBuffer();
}

void Mouse::LeftPresst( int newx,int newy )
{
	leftispresst = true;

	buffer.emplace( Event::Type::LeftPress,*this );
	TrimBuffer();
}

void Mouse::RightReleast( int newx,int newy )
{
	rightispresst = false;

	buffer.emplace( Event::Type::RightReleas,*this );
	TrimBuffer();
}

void Mouse::LeftReleast( int newx,int newy )
{
	leftispresst = false;

	buffer.emplace( Event::Type::LeftReleas,*this );
	TrimBuffer();
}

void Mouse::MittelPresst( int newx,int newy )
{
	mittelispresst = true;

	buffer.emplace( Event::Type::MittelPress,*this );
	TrimBuffer();
}

void Mouse::MittelReleast( int newx,int newy )
{
	mittelispresst = false;

	buffer.emplace( Event::Type::MittelReleas,*this );
	TrimBuffer();
}

void Mouse::WheelUp( int x,int y )
{
	buffer.emplace( Event::Type::WheelUp,*this );
	TrimBuffer();
}

void Mouse::WheelDown( int x,int y )
{
	buffer.emplace( Event::Type::WheelDown,*this );
	TrimBuffer();
}

void Mouse::EnterWindow( int x,int y )
{
	inwindow = true;

	buffer.emplace( Event::Type::Enter,*this );
	TrimBuffer();
}

void Mouse::LeaveWindow( int x,int y )
{
	inwindow = false;

	buffer.emplace( Event::Type::Leave,*this );
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	while ( buffer.size() > MaxBufferSize )
	{
		buffer.pop();
	}
}
