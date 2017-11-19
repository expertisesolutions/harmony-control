// Copyright Felipe Magno de Almeida 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef HARMONY_ERROR_HPP
#define HARMONY_ERROR_HPP

#include <boost/system/error_code.hpp>

namespace harmony {

enum class harmony_error_code
{
  sending_already_started,
  unknown_error,
  command_not_found = 566,
};

inline boost::system::error_code make_error_code( harmony_error_code e );
inline boost::system::error_condition make_error_condition( harmony_error_code e );
  
}

namespace boost { namespace system {
    template<> struct is_error_code_enum<harmony::harmony_error_code>
      { static const bool value = true; };
} }

namespace harmony {

struct harmony_error_category : public boost::system::error_category
{
  const char * name() const noexcept { return "harmony_error_category"; }
  std::string  message( int ev ) const noexcept
  {
    switch(static_cast<harmony_error_code>(ev))
    {
    case harmony_error_code::sending_already_started:
      return "Sending command is in progress, wait to send another command";
    case harmony_error_code::unknown_error:
      return "Unknown error";
    case harmony_error_code::command_not_found:
      return "Command not found for device id";
    default:
      return "unknown";
    }
  }

  boost::system::error_condition default_error_condition( int ev ) const noexcept
  {
    return make_error_condition(static_cast<harmony_error_code>(ev));
  }
};

inline boost::system::error_category const& get_harmony_error_category()
{
  static harmony_error_category const category;
  return category;
}
  
inline boost::system::error_code make_error_code( harmony_error_code e )
{ return boost::system::error_code( static_cast<int>(e), get_harmony_error_category() ); }
inline boost::system::error_condition make_error_condition( harmony_error_code e )
{ return boost::system::error_condition( static_cast<int>(e), get_harmony_error_category() ); }
  
}

#endif
