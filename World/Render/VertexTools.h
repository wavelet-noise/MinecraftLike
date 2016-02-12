// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef VertexTools_h__
#define VertexTools_h__

#include <boost/preprocessor.hpp>


#define __VERTEX_SEQ_TUPLE(seq) BOOST_PP_CAT(__VERTEX_SEQ_TUPLE_X seq, 0)
#define __VERTEX_SEQ_TUPLE_X(x, y) ((x, y)) __VERTEX_SEQ_TUPLE_Y
#define __VERTEX_SEQ_TUPLE_Y(x, y) ((x, y)) __VERTEX_SEQ_TUPLE_X
#define __VERTEX_SEQ_TUPLE_X0
#define __VERTEX_SEQ_TUPLE_Y0

#define __VERTEX_DECL_VERTEX(r, data, elem) \
    BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem);

#define __VERTEX_SEQ_MEMBER(x, y) x::y

#define __VERTEX_MAKE_STR(x) __VERTEX_MAKE_STR_1(x)
#define __VERTEX_MAKE_STR_1(x) #x

#define __VERTEX_DECL_VERTEX_ATTRIBUTE(r, data, elem) \
{ \
  sizeof(__VERTEX_SEQ_MEMBER(data, BOOST_PP_TUPLE_ELEM(2, 1, elem))), \
  offsetof(data, BOOST_PP_TUPLE_ELEM(2, 1, elem)), \
  __VERTEX_MAKE_STR(BOOST_PP_TUPLE_ELEM(2, 1, elem))\
},

//Attribute \

#define VERTEX(val) \
\
struct BOOST_PP_SEQ_HEAD(val)\
{\
  BOOST_PP_SEQ_FOR_EACH(__VERTEX_DECL_VERTEX, ~, __VERTEX_SEQ_TUPLE(BOOST_PP_SEQ_TAIL(val)))\
  static const std::vector<Attribute> &Get()\
  {\
    static std::vector<Attribute> mAttributeInfo\
    { \
      BOOST_PP_SEQ_FOR_EACH(__VERTEX_DECL_VERTEX_ATTRIBUTE, BOOST_PP_SEQ_HEAD(val), __VERTEX_SEQ_TUPLE(BOOST_PP_SEQ_TAIL(val))) \
    };\
    return mAttributeInfo;\
  }\
};
// \
// const std::vector<Attribute> \
//   BOOST_PP_SEQ_HEAD(val)::mAttributeInfo\
// { \
//   BOOST_PP_SEQ_FOR_EACH(__VERTEX_DECL_VERTEX_ATTRIBUTE, BOOST_PP_SEQ_HEAD(val), __VERTEX_SEQ_TUPLE(BOOST_PP_SEQ_TAIL(val))) \
// };

//const std::vector<int> Game::mV{ 1, 2, 3 };

#endif // VertexTools_h__

