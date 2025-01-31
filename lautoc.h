/*
** LuaAutoC - Automagically use C Functions and Structs with the Lua API
** https://github.com/orangeduck/LuaAutoC
** Daniel Holden - contact@theorangeduck.com
** Licensed under BSD
*/

#ifndef lautoc_h
#define lautoc_h

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/*
** EXPORT/IMPORT
*/
#if defined(LAC_DLL)
  #if defined(_WINDOWS) || defined(_WIN32)
    #if defined(LAC_IMPL)
      #define LAC_API __declspec(dllexport)
    #else
      #define LAC_API __declspec(dllimport)
    #endif
  #elif defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)
    #if defined(LAC_IMPL)
      #define LAC_API __attribute__((visibility("default")))
    #else
      #define LAC_API
    #endif
  #endif
#else
  #define LAC_API
#endif

/*
** Open / Close
*/

#define LUAA_REGISTRYPREFIX "lautoc_"

LAC_API void luaA_open(lua_State* L);
LAC_API void luaA_close(lua_State* L);

/*
** Types
*/

#define CAT(a, b) a ## b
#define SECOND(a, b, ...) b
#define IS_PROBE(...) SECOND(__VA_ARGS__, 0)
#define PROBE() ~, 1
#define NOT(x) IS_PROBE(CAT(_NOT_, x))
#define _NOT_0 PROBE()
#define BOOL(x) NOT(NOT(x))

#define IF(c) _IF(BOOL(c))
#define _IF(c) CAT(_IF_, c)
#define _IF_0(...)
#define _IF_1(...) __VA_ARGS__

#define IS_PAREN(x) IS_PROBE(IS_PAREN_PROBE x)
#define IS_PAREN_PROBE(...) PROBE()

#define IS_VOID(t) IS_PAREN( CAT(_IS_VOID_,t) (()) )
#define _IS_VOID_void(x)  x

#define SIZEOF(t) \
  IF(IS_VOID(t)) (1) \
    IF(NOT(IS_VOID(t)))(sizeof(t))


#define luaA_type(L, type) luaA_type_add(L, #type, SIZEOF(type))

enum {
  LUAA_INVALID_TYPE = -1
};

typedef lua_Integer luaA_Type;
typedef int (*luaA_Pushfunc)(lua_State*, luaA_Type, const void*);
typedef void (*luaA_Tofunc)(lua_State*, luaA_Type, void*, int);

LAC_API luaA_Type luaA_type_add(lua_State* L, const char* type, size_t size);
LAC_API luaA_Type luaA_type_find(lua_State* L, const char* type);

LAC_API const char* luaA_typename(lua_State* L, luaA_Type id);
LAC_API size_t luaA_typesize(lua_State* L, luaA_Type id);

/*
** Stack
*/

#define luaA_push(L, type, c_in) luaA_push_type(L, luaA_type(L, type), c_in)
#define luaA_to(L, type, c_out, index) luaA_to_type(L, luaA_type(L, type), c_out, index)

#define luaA_conversion(L, type, push_func, to_func) luaA_conversion_type(L, luaA_type(L, type), push_func, to_func);
#define luaA_conversion_push(L, type, func) luaA_conversion_push_type(L, luaA_type(L, type), func)
#define luaA_conversion_to(L, type, func) luaA_conversion_to_type(L, luaA_type(L, type), func)

#define luaA_conversion_registered(L, type) luaA_conversion_registered_type(L, luaA_type(L, type));
#define luaA_conversion_push_registered(L, type) luaA_conversion_push_registered_typ(L, luaA_type(L, type));
#define luaA_conversion_to_registered(L, type) luaA_conversion_to_registered_type(L, luaA_type(L, type));

LAC_API int luaA_push_type(lua_State* L, luaA_Type type, const void* c_in);
LAC_API void luaA_to_type(lua_State* L, luaA_Type type, void* c_out, int index);

LAC_API void luaA_conversion_type(lua_State* L, luaA_Type type_id, luaA_Pushfunc push_func, luaA_Tofunc to_func);
LAC_API void luaA_conversion_push_type(lua_State* L, luaA_Type type_id, luaA_Pushfunc func);
LAC_API void luaA_conversion_to_type(lua_State* L, luaA_Type type_id, luaA_Tofunc func);

LAC_API bool luaA_conversion_registered_type(lua_State* L, luaA_Type type);
LAC_API bool luaA_conversion_push_registered_type(lua_State* L, luaA_Type type);
LAC_API bool luaA_conversion_to_registered_type(lua_State* L, luaA_Type type);

LAC_API int luaA_push_bool(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_char(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_signed_char(lua_State* L,luaA_Type, const void* c_in);
LAC_API int luaA_push_unsigned_char(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_short(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_unsigned_short(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_int(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_unsigned_int(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_long(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_unsigned_long(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_long_long(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_unsigned_long_long(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_float(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_double(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_long_double(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_char_ptr(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_const_char_ptr(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_void_ptr(lua_State* L, luaA_Type, const void* c_in);
LAC_API int luaA_push_void(lua_State* L, luaA_Type, const void* c_in);

LAC_API void luaA_to_bool(lua_State* L, luaA_Type, void* c_out, int index);
LAC_API void luaA_to_char(lua_State* L, luaA_Type, void* c_out, int index);
LAC_API void luaA_to_signed_char(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_unsigned_char(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_short(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_unsigned_short(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_int(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_unsigned_int(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_long(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_unsigned_long(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_long_long(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_unsigned_long_long(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_float(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_double(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_long_double(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_char_ptr(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_const_char_ptr(lua_State* L, luaA_Type,  void* c_out, int index);
LAC_API void luaA_to_void_ptr(lua_State* L, luaA_Type,  void* c_out, int index);

/*
** Structs
*/
#define LUAA_INVALID_MEMBER_NAME NULL

#define luaA_struct(L, type) luaA_struct_type(L, luaA_type(L, type))
#define luaA_struct_member(L, type, member, member_type) luaA_struct_member_type(L, luaA_type(L, type), #member, luaA_type(L, member_type), offsetof(type, member))

#define luaA_struct_push(L, type, c_in) luaA_struct_push_type(L, luaA_type(L, type), c_in)
#define luaA_struct_push_member(L, type, member, c_in) luaA_struct_push_member_offset_type(L, luaA_type(L, type), offsetof(type, member), c_in)
#define luaA_struct_push_member_name(L, type, member, c_in) luaA_struct_push_member_name_type(L, luaA_type(L, type), member, c_in)

#define luaA_struct_to(L, type, c_out, index) luaA_struct_to_type(L, luaA_type(L, type), c_out, index)
#define luaA_struct_to_member(L, type, member, c_out, index) luaA_struct_to_member_offset_type(L, luaA_type(L, type), offsetof(type, member), c_out, index)
#define luaA_struct_to_member_name(L, type, member, c_out, index) luaA_struct_to_member_name_type(L, luaA_type(L, type), member, c_out, index)

#define luaA_struct_has_member(L, type, member) luaA_struct_has_member_offset_type(L, luaA_type(L, type), offsetof(type, member))
#define luaA_struct_has_member_name(L, type, member) luaA_struct_has_member_name_type(L, luaA_type(L, type), member)

#define luaA_struct_typeof_member(L, type, member) luaA_struct_typeof_member_offset_type(L, luaA_type(L, type), offsetof(type, member))
#define luaA_struct_typeof_member_name(L, type, member) luaA_struct_typeof_member_name_type(L, luaA_type(L, type), member)

#define luaA_struct_registered(L, type) luaA_struct_registered_type(L, luaA_type(L, type))
#define luaA_struct_next_member_name(L, type, member) luaA_struct_next_member_name_type(L, luaA_type(L,type), member)

LAC_API void luaA_struct_type(lua_State* L, luaA_Type type);
LAC_API void luaA_struct_member_type(lua_State* L, luaA_Type type, const char* member, luaA_Type member_type, size_t offset);

LAC_API int luaA_struct_push_type(lua_State* L, luaA_Type type, const void* c_in);
LAC_API int luaA_struct_push_member_offset_type(lua_State* L, luaA_Type type, size_t offset, const void* c_in);
LAC_API int luaA_struct_push_member_name_type(lua_State* L, luaA_Type type, const char* member, const void* c_in);

LAC_API void luaA_struct_to_type(lua_State* L, luaA_Type type, void* c_out, int index);
LAC_API void luaA_struct_to_member_offset_type(lua_State* L, luaA_Type type, size_t offset, void* c_out, int index);
LAC_API void luaA_struct_to_member_name_type(lua_State* L, luaA_Type type, const char* member, void* c_out, int index);

LAC_API bool luaA_struct_has_member_offset_type(lua_State* L, luaA_Type type, size_t offset);
LAC_API bool luaA_struct_has_member_name_type(lua_State* L, luaA_Type type, const char* member);

LAC_API luaA_Type luaA_struct_typeof_member_offset_type(lua_State* L, luaA_Type type, size_t offset);
LAC_API luaA_Type luaA_struct_typeof_member_name_type(lua_State* L, luaA_Type type, const char* member);

LAC_API bool luaA_struct_registered_type(lua_State* L, luaA_Type type);

LAC_API const char* luaA_struct_next_member_name_type(lua_State* L, luaA_Type type, const char* member);

/*
** Enums
*/

#define luaA_enum(L, type) luaA_enum_type(L, luaA_type(L, type), SIZEOF(type))
#define luaA_enum_value(L, type, value) luaA_enum_value_type(L, luaA_type(L, type), (const type[]){value}, #value);
#define luaA_enum_value_name(L, type, value, name) luaA_enum_value_type(L, luaA_type(L, type), (const type[]){value}, name);

#define luaA_enum_push(L, type, c_in) luaA_enum_push_type(L, luaA_type(L, type), c_in)
#define luaA_enum_to(L, type, c_out, index) luaA_enum_to_type(L, luaA_type(L, type), c_out, index)

#define luaA_enum_has_value(L, type, value) luaA_enum_has_value_type(L, luaA_type(L, type), (const type[]){value})
#define luaA_enum_has_name(L, type, name) luaA_enum_has_name_type(L, luaA_type(L, type), name)

#define luaA_enum_registered(L, type) luaA_enum_registered_type(L, luaA_type(L, type))
#define luaA_enum_next_value_name(L, type, member) luaA_enum_next_value_name_type(L, luaA_type(L,type), member)

LAC_API void luaA_enum_type(lua_State* L, luaA_Type type, size_t size);
LAC_API void luaA_enum_value_type(lua_State *L, luaA_Type type, const void* value, const char* name);

LAC_API int luaA_enum_push_type(lua_State *L, luaA_Type type, const void* c_in);
LAC_API void luaA_enum_to_type(lua_State* L, luaA_Type type, void *c_out, int index);

LAC_API bool luaA_enum_has_value_type(lua_State* L, luaA_Type type, const void* value);
LAC_API bool luaA_enum_has_name_type(lua_State* L, luaA_Type type, const char* name);

LAC_API bool luaA_enum_registered_type(lua_State *L, luaA_Type type);
LAC_API const char* luaA_enum_next_value_name_type(lua_State* L, luaA_Type type, const char* member);

/*
** Functions
*/

enum {
  LUAA_RETURN_STACK_SIZE   =  256,
  LUAA_ARGUMENT_STACK_SIZE = 2048
};

typedef void (*luaA_Func)(void*, void*, void(*)(), size_t*);

LAC_API int luaA_call(lua_State* L, void* func_ptr);
LAC_API int luaA_call_name(lua_State* L, const char* func_name);

LAC_API void luaA_function_register_type(lua_State* L, void* src_func, luaA_Func auto_func, const char* name, luaA_Type ret_t, int num_args, luaA_Type *args);

#endif
