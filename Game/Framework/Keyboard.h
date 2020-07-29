#pragma once

#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Releas,
			Invalid
		};
		Event()
			:
			type( Type::Invalid ),
			keycode( 0 )
		{}
		Event( Type type,unsigned char keycode )
			:
			type( type ),
			keycode( keycode )
		{}
		Type GetType() const
		{
			return type;
		}
		unsigned char GetKeyCode() const
		{
			return keycode;
		}
	private:
		Type type;
		unsigned char keycode;
	};
public:
	//Key
	bool KeyIsPresst( unsigned char keycode ) const;
	Event ReadKey();
	void KeyFlush();
	bool KeyIsEmpty() const;
	//Char
	char ReadChar();
	void CharFlush();
	bool CharIsEmpty() const;
private:
	void OnKeyPresst( unsigned char keycode );
	void OnKeyReleas( unsigned char keycode );
	void OnChar( unsigned char keycode );
	template<typename T>
	void TrimBuffer( std::queue<T>& container )
	{
		while ( container.size() > maxBufferSize )
		{
			container.pop();
		}
	}
private:
	static constexpr unsigned int maxBufferSize = 4u;
	static constexpr size_t nKeys = 256u;
	std::bitset<nKeys> KeyState;
	std::queue<Event> KeyStateBuffer;
	std::queue<char> CharBuffer;
	bool keyispresst = false;
};