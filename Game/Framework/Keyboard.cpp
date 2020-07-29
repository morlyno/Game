#include "Keyboard.h"

bool Keyboard::KeyIsPresst( unsigned char keycode ) const
{
	return KeyState[keycode];
}

Keyboard::Event Keyboard::ReadKey()
{
	if ( KeyStateBuffer.size() > 0 )
	{
		const auto e = KeyStateBuffer.front();
		TrimBuffer( KeyStateBuffer );
		return e;
	}
	return Keyboard::Event();
}

void Keyboard::KeyFlush()
{
	while ( !KeyStateBuffer.empty() )
	{
		KeyStateBuffer.pop();
	}
}

bool Keyboard::KeyIsEmpty() const
{
	return KeyStateBuffer.empty();
}

char Keyboard::ReadChar()
{
	if ( CharBuffer.size() > 0 )
	{
		const auto c = CharBuffer.front();
		TrimBuffer( CharBuffer );
		return c;
	}
	return 0;
}

void Keyboard::CharFlush()
{
	while ( !CharBuffer.empty() )
	{
		CharBuffer.pop();
	}
}

bool Keyboard::CharIsEmpty() const
{
	return CharBuffer.empty();
}

void Keyboard::OnKeyPresst( unsigned char keycode )
{
	KeyState[keycode] = true;
	keyispresst = true;

	KeyStateBuffer.emplace( Keyboard::Event::Type::Press,keycode );
	TrimBuffer( KeyStateBuffer );
}

void Keyboard::OnKeyReleas( unsigned char keycode )
{
	KeyState[keycode] = false;
	keyispresst = false;

	KeyStateBuffer.emplace( Keyboard::Event::Type::Releas,keycode );
	TrimBuffer( KeyStateBuffer );
}

void Keyboard::OnChar( unsigned char keycode )
{
	CharBuffer.emplace( keycode );
	TrimBuffer( CharBuffer );
}
