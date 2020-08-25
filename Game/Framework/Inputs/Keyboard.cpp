#include "Keyboard.h"

bool Keyboard::KeyIsPresst( unsigned char keycode ) const noexcept
{
	return KeyState[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if ( KeyStateBuffer.size() > 0 )
	{
		const auto e = KeyStateBuffer.front();
		KeyStateBuffer.pop();
		return e;
	}
	return Keyboard::Event();
}

void Keyboard::KeyFlush() noexcept
{
	while ( !KeyStateBuffer.empty() )
	{
		KeyStateBuffer.pop();
	}
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return KeyStateBuffer.empty();
}

char Keyboard::ReadChar() noexcept
{
	if ( CharBuffer.size() > 0 )
	{
		const auto c = CharBuffer.front();
		CharBuffer.pop();
		return c;
	}
	return 0;
}

void Keyboard::CharFlush() noexcept
{
	while ( !CharBuffer.empty() )
	{
		CharBuffer.pop();
	}
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return CharBuffer.empty();
}

void Keyboard::OnKeyPresst( unsigned char keycode ) noexcept
{
	KeyState[keycode] = true;
	keyispresst = true;

	KeyStateBuffer.emplace( Keyboard::Event::Type::Press,keycode );
	TrimBuffer( KeyStateBuffer );
}

void Keyboard::OnKeyReleas( unsigned char keycode ) noexcept
{
	KeyState[keycode] = false;
	keyispresst = false;

	KeyStateBuffer.emplace( Keyboard::Event::Type::Releas,keycode );
	TrimBuffer( KeyStateBuffer );
}

void Keyboard::OnChar( unsigned char keycode ) noexcept
{
	CharBuffer.emplace( keycode );
	TrimBuffer( CharBuffer );
}
