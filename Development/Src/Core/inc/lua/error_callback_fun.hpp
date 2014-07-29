#pragma once

// Internal Includes
#include "config.hpp"
#include "lua_state_fwd.hpp"

namespace luabind
{
	class type_id;

	typedef void(*error_callback_fun)(lua_State*);
	typedef void(*cast_failed_callback_fun)(lua_State*, type_id const&);
	typedef int(*pcall_callback_fun)(lua_State*);
}