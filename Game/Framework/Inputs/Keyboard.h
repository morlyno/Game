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
		Event() noexcept
			:
			type( Type::Invalid ),
			keycode( 0 )
		{}
		Event( Type type,unsigned char keycode ) noexcept
			:
			type( type ),
			keycode( keycode )
		{}
		Type GetType() const noexcept
		{
			return type;
		}
		unsigned char GetKeyCode() const noexcept
		{
			return keycode;
		}
	private:
		Type type;
		unsigned char keycode;
	};
public:
	//Key
	bool KeyIsPresst( unsigned char keycode ) const noexcept;
	Event ReadKey() noexcept;
	void KeyFlush() noexcept;
	bool KeyIsEmpty() const noexcept;
	//Char
	char ReadChar() noexcept;
	void CharFlush() noexcept;
	bool CharIsEmpty() const noexcept;
	void TurnAutoRepeatOn() noexcept;
	void TurnAutoRepeatOff() noexcept;
	bool DoAutoRepeat() const noexcept;
private:
	void OnKeyPresst( unsigned char keycode ) noexcept;
	void OnKeyReleas( unsigned char keycode ) noexcept;
	void OnChar( unsigned char keycode ) noexcept;
	template<typename T>
	void TrimBuffer( std::queue<T>& container ) noexcept
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
	bool autorepeat = true;
};