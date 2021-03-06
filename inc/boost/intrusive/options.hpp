/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2007
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_OPTIONS_HPP
#define BOOST_INTRUSIVE_OPTIONS_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/intrusive_fwd.hpp>
#include <boost/intrusive/link_mode.hpp>
#include <boost/intrusive/detail/mpl.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/static_assert.hpp>


namespace boost {
namespace intrusive {

/// @cond

struct default_tag;
struct member_tag;

namespace detail{

template <class ValueTraits>
struct eval_value_traits
{
   typedef typename ValueTraits::value_traits type;
};

template <class T>
struct external_bucket_traits_is_true
{
   static const bool value = external_bucket_traits_bool<T>::value == 3;
};

template <class BucketTraits>
struct eval_bucket_traits
{
   typedef typename BucketTraits::bucket_traits type;
};

template<class T, class BaseHook>
struct get_base_value_traits
{
   typedef detail::base_hook_traits
      < T
      , typename BaseHook::boost_intrusive_tags::node_traits
      , BaseHook::boost_intrusive_tags::link_mode
      , typename BaseHook::boost_intrusive_tags::tag
      , BaseHook::boost_intrusive_tags::hook_type> type;
};

template<class T, class MemberHook>
struct get_member_value_traits
{
   typedef typename MemberHook::member_value_traits type;
};

template<class T, class SupposedValueTraits>
struct get_value_traits
{
   typedef SupposedValueTraits supposed_value_traits;
   //...if it's a base hook
   typedef typename detail::eval_if_c
      < internal_base_hook_bool_is_true<supposed_value_traits>::value
      //...get it's internal value traits using
      //the provided T value type.
      , get_base_value_traits<T, supposed_value_traits>
      //...else use it's internal value traits tag
      //(member hooks and custom value traits are in this group)
      , detail::eval_if_c
         < internal_member_value_traits<supposed_value_traits>::value
         , get_member_value_traits<T, supposed_value_traits>
         , detail::identity<supposed_value_traits>
         >
      >::type type;
};

template<class BaseHook>
struct get_base_node_traits
{
   typedef typename BaseHook::boost_intrusive_tags::node_traits type;
};

template<class MemberHook>
struct get_member_node_traits
{
   typedef typename MemberHook::member_value_traits::node_traits type;
};

template<class ValueTraits>
struct get_explicit_node_traits
{
   typedef typename ValueTraits::node_traits type;
};


template<class SupposedValueTraits>
struct get_node_traits
{
   typedef SupposedValueTraits supposed_value_traits;
   //...if it's a base hook
   typedef typename detail::eval_if_c
      < internal_base_hook_bool_is_true<supposed_value_traits>::value
      //...get it's internal value traits using
      //the provided T value type.
      , get_base_node_traits<supposed_value_traits>
      //...else use it's internal value traits tag
      //(member hooks and custom value traits are in this group)
      , detail::eval_if_c
         < internal_member_value_traits<supposed_value_traits>::value
         , get_member_node_traits<supposed_value_traits>
         , get_explicit_node_traits<supposed_value_traits>
         >
      >::type type;
};


}  //namespace detail{


//!This type indicates that no option is being used
//!and that the default options should be used
struct none
{
    template<class Base>
    struct pack : Base
    { };
};

/// @endcond

//!This option setter specifies if the intrusive
//!container stores its size as a member to
//!obtain constant-time size() member.
template<bool Enabled>
struct constant_time_size
{
/// @cond
    template<class Base>
    struct pack : Base
    {
        static const bool constant_time_size = Enabled;
    };
/// @endcond
};

//!This option setter specifies the type that
//!the container will use to store its size.
template<class SizeType>
struct size_type
{
/// @cond
    template<class Base>
    struct pack : Base
    {
        typedef SizeType size_type;
    };
/// @endcond
};

//!This option setter specifies the strict weak ordering
//!comparison functor for the value type
template<class Compare>
struct compare
{
/// @cond
    template<class Base>
    struct pack : Base
    {
        typedef Compare compare;
    };
/// @endcond
};

//!This option setter for scapegoat containers specifies if
//!the intrusive scapegoat container should use a non-variable
//!alpha value that does not need floating-point operations.
//!
//!If activated, the fixed alpha value is 1/sqrt(2). This
//!option also saves some space in the container since 
//!the alpha value and some additional data does not need
//!to be stored in the container.
//!
//!If the user only needs an alpha value near 1/sqrt(2), this
//!option also improves performance since avoids logarithm
//!and division operations when rebalancing the tree.
template<bool Enabled>
struct floating_point
{
/// @cond
    template<class Base>
    struct pack : Base
    {
        static const bool floating_point = Enabled;
    };
/// @endcond
};

//!This option setter specifies the equality
//!functor for the value type
template<class Equal>
struct equal
{
/// @cond
    template<class Base>
    struct pack : Base
    {
        typedef Equal equal;
    };
/// @endcond
};

//!This option setter specifies the hash
//!functor for the value type
template<class Hash>
struct hash
{
/// @cond
    template<class Base>
    struct pack : Base
    {
        typedef Hash hash;
    };
/// @endcond
};

//!This option setter specifies the relationship between the type
//!to be managed by the container (the value type) and the node to be
//!used in the node algorithms. It also specifies the linking policy.
template<typename ValueTraits>
struct value_traits
{
/// @cond
    template<class Base>
    struct pack : Base
    {
        typedef ValueTraits value_traits;
    };
/// @endcond
};

//!This option setter specifies the member hook the
//!container must use.
template< typename Parent
        , typename MemberHook
        , MemberHook Parent::* PtrToMember>
struct member_hook
{
/// @cond
   typedef char Parent::* GenericPtrToMember;
   typedef detail::member_hook_traits
      < Parent
      , MemberHook
      , PtrToMember
      > member_value_traits;
   template<class Base>
   struct pack : Base
   {
      typedef member_value_traits value_traits;
   };
/// @endcond
};

//!This option setter specifies that the container
//!must use the specified base hook
template<typename BaseHook>
struct base_hook
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      typedef BaseHook value_traits;
   };
/// @endcond
};

//!This option setter specifies the type of
//!a void pointer. This will instruct the hook
//!to use this type of pointer instead of the
//!default one
template<class VoidPointer>
struct void_pointer
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      typedef VoidPointer void_pointer;
   };
/// @endcond
};

//!This option setter specifies the type of
//!the tag of a base hook. A type can not have two
//!base hooks of the same type, so a tag can be used
//!to differentiate two base hooks with otherwise same type
template<class Tag>
struct tag
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      typedef Tag tag;
   };
/// @endcond
};

//!This option setter specifies the link mode
//!(normal_link, safe_link or auto_unlink)
template<link_mode_type LinkType>
struct link_mode
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      static const link_mode_type link_mode = LinkType;
   };
/// @endcond
};

//!This option setter specifies if the hook
//!should be optimized for size instead of for speed.
template<bool Enabled>
struct optimize_size
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      static const bool optimize_size = Enabled;
   };
/// @endcond
};

//!This option setter specifies if the list container should
//!use a linear implementation instead of a circular one.
template<bool Enabled>
struct linear
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      static const bool linear = Enabled;
   };
/// @endcond
};

//!This option setter specifies if the list container should
//!use a linear implementation instead of a circular one.
template<bool Enabled>
struct cache_last
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      static const bool cache_last = Enabled;
   };
/// @endcond
};

//!This option setter specifies the bucket traits
//!class for unordered associative containers. When this option is specified,
//!instead of using the default bucket traits, a user defined holder will be defined
template<class BucketTraits>
struct bucket_traits
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      typedef BucketTraits bucket_traits;
   };
/// @endcond
};

//!This option setter specifies if the unordered hook
//!should offer room to store the hash value.
//!Storing the hash in the hook will speed up rehashing
//!processes in applications where rehashing is frequent,
//!rehashing might throw or the value is heavy to hash.
template<bool Enabled>
struct store_hash
{
/// @cond
    template<class Base>
    struct pack : Base
    {
        static const bool store_hash = Enabled;
    };
/// @endcond
};

//!This option setter specifies if the bucket array will be always power of two.
//!This allows using masks instead of the default modulo operation to determine
//!the bucket number from the hash value, leading to better performance.
//!In debug mode, if power of two buckets mode is activated, the bucket length
//!will be checked to through assertions to assure the bucket length is power of two.
template<bool Enabled>
struct power_2_buckets
{
/// @cond
   template<class Base>
   struct pack : Base
   {
      static const bool power_2_buckets = Enabled;
   };
/// @endcond
};

/// @cond

template<class Prev, class Next>
struct do_pack
{
   //Use "pack" member template to pack options
   typedef typename Next::template pack<Prev> type;
};

template<class Prev>
struct do_pack<Prev, none>
{
   //Avoid packing "none" to shorten template names
   typedef Prev type;
};


template
   < class DefaultOptions
   , class O1         = none
   , class O2         = none
   , class O3         = none
   , class O4         = none
   , class O5         = none
   , class O6         = none
   , class O7         = none
   , class O8         = none
   , class O9         = none
   , class Option10   = none
   >
struct pack_options
{
   // join options
   typedef
      typename do_pack
      <  typename do_pack
         <  typename do_pack
            <  typename do_pack
               <  typename do_pack
                  <  typename do_pack
                     <  typename do_pack
                        <  typename do_pack
                           <  typename do_pack
                              <  typename do_pack
                                 < DefaultOptions
                                 , O1
                                 >::type
                              , O2
                              >::type
                           , O3
                           >::type
                        , O4
                        >::type
                     , O5
                     >::type
                  , O6
                  >::type
               , O7
               >::type
            , O8
            >::type
         , O9
         >::type
      , Option10
      >::type 
   type;
};

struct hook_defaults
   :  public pack_options
      < none
      , void_pointer<void*>
      , link_mode<safe_link>
      , tag<default_tag>
      , optimize_size<false>
      , store_hash<false>
      , linear<false>
      >::type
{};

/// @endcond

}  //namespace intrusive {
}  //namespace boost {

#include <boost/intrusive/detail/config_end.hpp>

#endif   //#ifndef BOOST_INTRUSIVE_OPTIONS_HPP
