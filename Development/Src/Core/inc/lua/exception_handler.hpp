// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_EXCEPTION_HANDLER_050601_HPP
#define LUABIND_EXCEPTION_HANDLER_050601_HPP

#include <lua/config.hpp>           // for LUABIND_API
#include <type_traits>
#include <lua/lua_include.hpp>
#include <lua/detail/meta.hpp>

#ifdef LUABIND_SUPPORT_NOTHROW_POLICY
#include <boost/optional.hpp>
#endif

namespace luabind {

# ifndef LUABIND_NO_EXCEPTIONS

namespace detail
{

  struct LUABIND_API exception_handler_base
  {
      exception_handler_base()
        : next(0)
      {}

      virtual ~exception_handler_base() {}
      virtual void handle(lua_State*) const = 0;

      void try_next(lua_State*) const;

      exception_handler_base* next;
  };

  template<class E, class Handler>
  struct exception_handler : exception_handler_base
  {
      typedef E const& argument;

      exception_handler(Handler handler)
        : handler(handler)
      {}

      void handle(lua_State* L) const
      {
          try
          {
              try_next(L);
          }
          catch (argument e)
          {
              handler(L, e);
          }
      }

      Handler handler;
  };

  LUABIND_API void handle_exception_aux(lua_State* L);
  LUABIND_API void register_exception_handler(exception_handler_base*);

} // namespace detail

# endif

template<class E, class Handler>
void register_exception_handler(Handler handler, meta::type<E>* = 0)
{
# ifndef LUABIND_NO_EXCEPTIONS
    detail::register_exception_handler(
        new detail::exception_handler<E, Handler>(handler)
    );
# endif
}

#ifdef LUABIND_SUPPORT_NOTHROW_POLICY
template<class R, class F>
boost::optional<R> handle_exceptions(lua_State* L, F fn, boost::type<R>* = 0)
{
# ifndef LUABIND_NO_EXCEPTIONS
    try
    {
        return fn();
    }
    catch (...)
    {
        detail::handle_exception_aux(L);
    }

    return boost::optional<R>();
# else
    return fn();
# endif
}
#endif

} // namespace luabind

#endif // LUABIND_EXCEPTION_HANDLER_050601_HPP

