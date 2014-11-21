#pragma once


namespace NGTech {
	class Noncopyable
	{
	private:
		Noncopyable(const Noncopyable&);
		void operator=(const Noncopyable&);
	};
}