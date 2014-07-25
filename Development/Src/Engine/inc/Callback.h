#pragma once

namespace NGTech
{
	/*
	 */
	class CallbackBase {

	public:

		CallbackBase() { }
		virtual ~CallbackBase() { }

		virtual void run() = 0;
	};

	/******************************************************************************\
	*
	* Function callbacks
	*
	\******************************************************************************/

	/*
	 */
	template <class Func> class Callback_0 : public CallbackBase {
	public:
		Callback_0(Func func) : func(func) { }
		virtual void run() { func(); }
	private:
		Func func;
	};

	template <class Func, class A0> class Callback_1 : public CallbackBase {
	public:
		Callback_1(Func func, A0 a0) : func(func), a0(a0) { }
		virtual void run() { func(a0); }
	private:
		Func func;
		A0 a0;
	};

	template <class Func, class A0, class A1> class Callback_2 : public CallbackBase {
	public:
		Callback_2(Func func, A0 a0, A1 a1) : func(func), a0(a0), a1(a1) { }
		virtual void run() { func(a0, a1); }
	private:
		Func func;
		A0 a0;
		A1 a1;
	};

	template <class Func, class A0, class A1, class A2> class Callback_3 : public CallbackBase {
	public:
		Callback_3(Func func, A0 a0, A1 a1, A2 a2) : func(func), a0(a0), a1(a1), a2(a2) { }
		virtual void run() { func(a0, a1, a2); }
	private:
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
	};

	template <class Func, class A0, class A1, class A2, class A3> class Callback_4 : public CallbackBase {
	public:
		Callback_4(Func func, A0 a0, A1 a1, A2 a2, A3 a3) : func(func), a0(a0), a1(a1), a2(a2), a3(a3) { }
		virtual void run() { func(a0, a1, a2, a3); }
	private:
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
	};

	template <class Func, class A0, class A1, class A2, class A3, class A4> class Callback_5 : public CallbackBase {
	public:
		Callback_5(Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) : func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4) { }
		virtual void run() { func(a0, a1, a2, a3, a4); }
	private:
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
	};

	template <class Func, class A0, class A1, class A2, class A3, class A4, class A5> class Callback_6 : public CallbackBase {
	public:
		Callback_6(Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) : func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5) { }
		virtual void run() { func(a0, a1, a2, a3, a4, a5); }
	private:
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
	};

	template <class Func, class A0, class A1, class A2, class A3, class A4, class A5, class A6> class Callback_7 : public CallbackBase {
	public:
		Callback_7(Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) : func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6) { }
		virtual void run() { func(a0, a1, a2, a3, a4, a5, a6); }
	private:
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
		A6 a6;
	};

	template <class Func, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7> class Callback_8 : public CallbackBase {
	public:
		Callback_8(Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) : func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6), a7(a7) { }
		virtual void run() { func(a0, a1, a2, a3, a4, a5, a6, a7); }
	private:
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
		A6 a6;
		A6 a7;
	};

	/*
	 */
	template <class Ret>
	CallbackBase *MakeCallback(Ret(*func)()) {
		return new Callback_0<Ret(*)()>(func);
	}

	template <class Ret, class A0>
	CallbackBase *MakeCallback(Ret(*func)(A0), A0 a0) {
		return new Callback_1<Ret(*)(A0), A0>(func, a0);
	}

	template <class Ret, class A0, class A1>
	CallbackBase *MakeCallback(Ret(*func)(A0, A1), A0 a0, A1 a1) {
		return new Callback_2<Ret(*)(A0, A1), A0, A1>(func, a0, a1);
	}

	template <class Ret, class A0, class A1, class A2>
	CallbackBase *MakeCallback(Ret(*func)(A0, A1, A2), A0 a0, A1 a1, A2 a2) {
		return new Callback_3<Ret(*)(A0, A1, A2), A0, A1, A2>(func, a0, a1, a2);
	}

	template <class Ret, class A0, class A1, class A2, class A3>
	CallbackBase *MakeCallback(Ret(*func)(A0, A1, A2, A3), A0 a0, A1 a1, A2 a2, A3 a3) {
		return new Callback_4<Ret(*)(A0, A1, A2, A3), A0, A1, A2, A3>(func, a0, a1, a2, a3);
	}

	template <class Ret, class A0, class A1, class A2, class A3, class A4>
	CallbackBase *MakeCallback(Ret(*func)(A0, A1, A2, A3, A4), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
		return new Callback_5<Ret(*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(func, a0, a1, a2, a3, a4);
	}

	template <class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
	CallbackBase *MakeCallback(Ret(*func)(A0, A1, A2, A3, A4, A5), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
		return new Callback_6<Ret(*)(A0, A1, A2, A3, A4, A5), A0, A1, A2, A3, A4, A5>(func, a0, a1, a2, a3, a4, a5);
	}

	template <class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
	CallbackBase *MakeCallback(Ret(*func)(A0, A1, A2, A3, A4, A5, A6), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
		return new Callback_7<Ret(*)(A0, A1, A2, A3, A4, A5, A6), A0, A1, A2, A3, A4, A5, A6>(func, a0, a1, a2, a3, a4, a5, a6);
	}

	template <class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	CallbackBase *MakeCallback(Ret(*func)(A0, A1, A2, A3, A4, A5, A6, A7), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
		return new Callback_8<Ret(*)(A0, A1, A2, A3, A4, A5, A6, A7), A0, A1, A2, A3, A4, A5, A6, A7>(func, a0, a1, a2, a3, a4, a5, a6, a7);
	}

	/******************************************************************************\
	*
	* Object function callbacks
	*
	\******************************************************************************/

	/*
	 */
	template <class Class, class Func> class ObjectCallback_0 : public CallbackBase {
	public:
		ObjectCallback_0(Class *object, Func func) : object(object), func(func) { }
		virtual void run() { (object->*func)(); }
	private:
		Class *object;
		Func func;
	};

	template <class Class, class Func, class A0> class ObjectCallback_1 : public CallbackBase {
	public:
		ObjectCallback_1(Class *object, Func func, A0 a0) : object(object), func(func), a0(a0) { }
		virtual void run() { (object->*func)(a0); }
	private:
		Class *object;
		Func func;
		A0 a0;
	};

	template <class Class, class Func, class A0, class A1> class ObjectCallback_2 : public CallbackBase {
	public:
		ObjectCallback_2(Class *object, Func func, A0 a0, A1 a1) : object(object), func(func), a0(a0), a1(a1) { }
		virtual void run() { (object->*func)(a0, a1); }
	private:
		Class *object;
		Func func;
		A0 a0;
		A1 a1;
	};

	template <class Class, class Func, class A0, class A1, class A2> class ObjectCallback_3 : public CallbackBase {
	public:
		ObjectCallback_3(Class *object, Func func, A0 a0, A1 a1, A2 a2) : object(object), func(func), a0(a0), a1(a1), a2(a2) { }
		virtual void run() { (object->*func)(a0, a1, a2); }
	private:
		Class *object;
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
	};

	template <class Class, class Func, class A0, class A1, class A2, class A3> class ObjectCallback_4 : public CallbackBase {
	public:
		ObjectCallback_4(Class *object, Func func, A0 a0, A1 a1, A2 a2, A3 a3) : object(object), func(func), a0(a0), a1(a1), a2(a2), a3(a3) { }
		virtual void run() { (object->*func)(a0, a1, a2, a3); }
	private:
		Class *object;
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
	};

	template <class Class, class Func, class A0, class A1, class A2, class A3, class A4> class ObjectCallback_5 : public CallbackBase {
	public:
		ObjectCallback_5(Class *object, Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) : object(object), func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4) { }
		virtual void run() { (object->*func)(a0, a1, a2, a3, a4); }
	private:
		Class *object;
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
	};

	template <class Class, class Func, class A0, class A1, class A2, class A3, class A4, class A5> class ObjectCallback_6 : public CallbackBase {
	public:
		ObjectCallback_6(Class *object, Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) : object(object), func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5) { }
		virtual void run() { (object->*func)(a0, a1, a2, a3, a4, a5); }
	private:
		Class *object;
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
	};

	template <class Class, class Func, class A0, class A1, class A2, class A3, class A4, class A5, class A6> class ObjectCallback_7 : public CallbackBase {
	public:
		ObjectCallback_7(Class *object, Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) : object(object), func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6) { }
		virtual void run() { (object->*func)(a0, a1, a2, a3, a4, a5, a6); }
	private:
		Class *object;
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
		A6 a6;
	};

	template <class Class, class Func, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7> class ObjectCallback_8 : public CallbackBase {
	public:
		ObjectCallback_8(Class *object, Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) : object(object), func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4), a5(a5), a6(a6), a7(a7) { }
		virtual void run() { (object->*func)(a0, a1, a2, a3, a4, a5, a6, a7); }
	private:
		Class *object;
		Func func;
		A0 a0;
		A1 a1;
		A2 a2;
		A3 a3;
		A4 a4;
		A5 a5;
		A6 a6;
		A7 a7;
	};

	/*
	 */
	template <class Class, class Ret>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)()) {
		return new ObjectCallback_0<Class, Ret(Class::*)()>(object, func);
	}

	template <class Class, class Ret, class A0>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)(A0), A0 a0) {
		return new ObjectCallback_1<Class, Ret(Class::*)(A0), A0>(object, func, a0);
	}

	template <class Class, class Ret, class A0, class A1>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)(A0, A1), A0 a0, A1 a1) {
		return new ObjectCallback_2<Class, Ret(Class::*)(A0, A1), A0, A1>(object, func, a0, a1);
	}

	template <class Class, class Ret, class A0, class A1, class A2>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)(A0, A1, A2), A0 a0, A1 a1, A2 a2) {
		return new ObjectCallback_3<Class, Ret(Class::*)(A0, A1, A2), A0, A1, A2>(object, func, a0, a1, a2);
	}

	template <class Class, class Ret, class A0, class A1, class A2, class A3>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)(A0, A1, A2, A3), A0 a0, A1 a1, A2 a2, A3 a3) {
		return new ObjectCallback_4<Class, Ret(Class::*)(A0, A1, A2, A3), A0, A1, A2, A3>(object, func, a0, a1, a2, a3);
	}

	template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)(A0, A1, A2, A3, A4), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
		return new ObjectCallback_5<Class, Ret(Class::*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(object, func, a0, a1, a2, a3, a4);
	}

	template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)(A0, A1, A2, A3, A4, A5), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
		return new ObjectCallback_6<Class, Ret(Class::*)(A0, A1, A2, A3, A4, A5), A0, A1, A2, A3, A4, A5>(object, func, a0, a1, a2, a3, a4, a5);
	}

	template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)(A0, A1, A2, A3, A4, A5, A6), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
		return new ObjectCallback_7<Class, Ret(Class::*)(A0, A1, A2, A3, A4, A5, A6), A0, A1, A2, A3, A4, A5, A6>(object, func, a0, a1, a2, a3, a4, a5, a6);
	}

	template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	CallbackBase *MakeCallback(Class *object, Ret(Class::*func)(A0, A1, A2, A3, A4, A5, A6, A7), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
		return new ObjectCallback_8<Class, Ret(Class::*)(A0, A1, A2, A3, A4, A5, A6, A7), A0, A1, A2, A3, A4, A5, A6, A7>(object, func, a0, a1, a2, a3, a4, a5, a6, a7);
	}
}