//  Copyright John Maddock 2007.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/math/tools/real_cast.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/roots.hpp>
#include <boost/math/special_functions/fpclassify.hpp>

#include <ostream>
#include <istream>
#include <cmath>
#include <NTL/RR.h>

#ifndef BOOST_MATH_NTL_RR_HPP
#define BOOST_MATH_NTL_RR_HPP

namespace boost{ namespace math{

namespace ntl
{

class RR;

RR ldexp(RR r, int exp);
RR frexp(RR r, int* exp);

class RR
{
public:
   // Constructors:
   RR() {}
   RR(const ::NTL::RR& c) : m_value(c){}
   RR(char c)
   {
      m_value = c;
   }
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
   RR(wchar_t c)
   {
      m_value = c;
   }
#endif
   RR(unsigned char c)
   {
      m_value = c;
   }
   RR(signed char c)
   {
      m_value = c;
   }
   RR(unsigned short c)
   {
      m_value = c;
   }
   RR(short c)
   {
      m_value = c;
   }
   RR(unsigned int c)
   {
      assign_large_int(c);
   }
   RR(int c)
   {
      assign_large_int(c);
   }
   RR(unsigned long c)
   {
      assign_large_int(c);
   }
   RR(long c)
   {
      assign_large_int(c);
   }
#ifdef BOOST_HAS_LONG_LONG
   RR(unsigned long long c)
   {
      assign_large_int(c);
   }
   RR(long long c)
   {
      assign_large_int(c);
   }
#endif
   RR(float c)
   {
      m_value = c;
   }
   RR(double c)
   {
      m_value = c;
   }
   RR(long double c)
   {
      assign_large_real(c);
   }

   // Assignment:
   RR& operator=(char c) { m_value = c; return *this; }
   RR& operator=(unsigned char c) { m_value = c; return *this; }
   RR& operator=(signed char c) { m_value = c; return *this; }
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
   RR& operator=(wchar_t c) { m_value = c; return *this; }
#endif
   RR& operator=(short c) { m_value = c; return *this; }
   RR& operator=(unsigned short c) { m_value = c; return *this; }
   RR& operator=(int c) { assign_large_int(c); return *this; }
   RR& operator=(unsigned int c) { assign_large_int(c); return *this; }
   RR& operator=(long c) { assign_large_int(c); return *this; }
   RR& operator=(unsigned long c) { assign_large_int(c); return *this; }
#ifdef BOOST_HAS_LONG_LONG
   RR& operator=(long long c) { assign_large_int(c); return *this; }
   RR& operator=(unsigned long long c) { assign_large_int(c); return *this; }
#endif
   RR& operator=(float c) { m_value = c; return *this; }
   RR& operator=(double c) { m_value = c; return *this; }
   RR& operator=(long double c) { assign_large_real(c); return *this; }

   // Access:
   NTL::RR& value(){ return m_value; }
   NTL::RR const& value()const{ return m_value; }

   // Member arithmetic:
   RR& operator+=(const RR& other)
   { m_value += other.value(); return *this; }
   RR& operator-=(const RR& other)
   { m_value -= other.value(); return *this; }
   RR& operator*=(const RR& other)
   { m_value *= other.value(); return *this; }
   RR& operator/=(const RR& other)
   { m_value /= other.value(); return *this; }
   RR operator-()const
   { return -m_value; }
   RR const& operator+()const
   { return *this; }

   // RR compatibity:
   const ::NTL::ZZ& mantissa() const
   { return m_value.mantissa(); }
   long exponent() const
   { return m_value.exponent(); }

   static void SetPrecision(long p)
   { ::NTL::RR::SetPrecision(p); }

   static long precision()
   { return ::NTL::RR::precision(); }

   static void SetOutputPrecision(long p)
   { ::NTL::RR::SetOutputPrecision(p); }
   static long OutputPrecision()
   { return ::NTL::RR::OutputPrecision(); }


private:
   ::NTL::RR m_value;

   template <class V>
   void assign_large_real(const V& a)
   {
      using std::frexp;
      using std::ldexp;
      using std::floor;
      if (a == 0) {
         clear(m_value);
         return;
      }

      if (a == 1) {
         NTL::set(m_value);
         return;
      }

      if (!(boost::math::isfinite)(a))
      {
         throw std::overflow_error("Cannot construct an instance of NTL::RR with an infinite value.");
      }

      int e;
      long double f, term;
      ::NTL::RR t;
      clear(m_value);

      f = frexp(a, &e);

      while(f)
      {
         // extract 30 bits from f:
         f = ldexp(f, 30);
         term = floor(f);
         e -= 30;
         conv(t.x, (int)term);
         t.e = e;
         m_value += t;
         f -= term;
      }
   }

   template <class V>
   void assign_large_int(V a)
   {
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4146)
#endif
      clear(m_value);
      int exp = 0;
      NTL::RR t;
      bool neg = a < V(0) ? true : false;
      if(neg) 
         a = -a;
      while(a)
      {
         t = static_cast<double>(a & 0xffff);
         m_value += ldexp(RR(t), exp).value();
         a >>= 16;
         exp += 16;
      }
      if(neg)
         m_value = -m_value;
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
   }
};

// Non-member arithmetic:
inline RR operator+(const RR& a, const RR& b)
{
   RR result(a);
   result += b;
   return result;
}
inline RR operator-(const RR& a, const RR& b)
{
   RR result(a);
   result -= b;
   return result;
}
inline RR operator*(const RR& a, const RR& b)
{
   RR result(a);
   result *= b;
   return result;
}
inline RR operator/(const RR& a, const RR& b)
{
   RR result(a);
   result /= b;
   return result;
}

// Comparison:
inline bool operator == (const RR& a, const RR& b)
{ return a.value() == b.value() ? true : false; }
inline bool operator != (const RR& a, const RR& b)
{ return a.value() != b.value() ? true : false;}
inline bool operator < (const RR& a, const RR& b)
{ return a.value() < b.value() ? true : false; }
inline bool operator <= (const RR& a, const RR& b)
{ return a.value() <= b.value() ? true : false; }
inline bool operator > (const RR& a, const RR& b)
{ return a.value() > b.value() ? true : false; }
inline bool operator >= (const RR& a, const RR& b)
{ return a.value() >= b.value() ? true : false; }

#if 0
// Non-member mixed compare:
template <class T>
inline bool operator == (const T& a, const RR& b)
{
   return a == b.value();
}
template <class T>
inline bool operator != (const T& a, const RR& b)
{
   return a != b.value();
}
template <class T>
inline bool operator < (const T& a, const RR& b)
{
   return a < b.value();
}
template <class T>
inline bool operator > (const T& a, const RR& b)
{
   return a > b.value();
}
template <class T>
inline bool operator <= (const T& a, const RR& b)
{
   return a <= b.value();
}
template <class T>
inline bool operator >= (const T& a, const RR& b)
{
   return a >= b.value();
}
#endif  // Non-member mixed compare:

// Non-member functions:
/*
inline RR acos(RR a)
{ return ::NTL::acos(a.value()); }
*/
inline RR cos(RR a)
{ return ::NTL::cos(a.value()); }
/*
inline RR asin(RR a)
{ return ::NTL::asin(a.value()); }
inline RR atan(RR a)
{ return ::NTL::atan(a.value()); }
inline RR atan2(RR a, RR b)
{ return ::NTL::atan2(a.value(), b.value()); }
*/
inline RR ceil(RR a)
{ return ::NTL::ceil(a.value()); }
/*
inline RR fmod(RR a, RR b)
{ return ::NTL::fmod(a.value(), b.value()); }
inline RR cosh(RR a)
{ return ::NTL::cosh(a.value()); }
*/
inline RR exp(RR a)
{ return ::NTL::exp(a.value()); }
inline RR fabs(RR a)
{ return ::NTL::fabs(a.value()); }
inline RR abs(RR a)
{ return ::NTL::abs(a.value()); }
inline RR floor(RR a)
{ return ::NTL::floor(a.value()); }
/*
inline RR modf(RR a, RR* ipart)
{
   ::NTL::RR ip;
   RR result = modf(a.value(), &ip);
   *ipart = ip;
   return result;
}
inline RR frexp(RR a, int* expon)
{ return ::NTL::frexp(a.value(), expon); }
inline RR ldexp(RR a, int expon)
{ return ::NTL::ldexp(a.value(), expon); }
*/
inline RR log(RR a)
{ return ::NTL::log(a.value()); }
inline RR log10(RR a)
{ return ::NTL::log10(a.value()); }
/*
inline RR tan(RR a)
{ return ::NTL::tan(a.value()); }
*/
inline RR pow(RR a, RR b)
{ return ::NTL::pow(a.value(), b.value()); }
inline RR pow(RR a, int b)
{ return ::NTL::power(a.value(), b); }
inline RR sin(RR a)
{ return ::NTL::sin(a.value()); }
/*
inline RR sinh(RR a)
{ return ::NTL::sinh(a.value()); }
*/
inline RR sqrt(RR a)
{ return ::NTL::sqrt(a.value()); }
/*
inline RR tanh(RR a)
{ return ::NTL::tanh(a.value()); }
*/
   inline RR pow(const RR& r, long l)
   {
      return ::NTL::power(r.value(), l);
   }
   inline RR tan(const RR& a)
   {
      return sin(a)/cos(a);
   }
   inline RR frexp(RR r, int* exp)
   {
      *exp = r.value().e;
      r.value().e = 0;
      while(r >= 1)
      {
         *exp += 1;
         r.value().e -= 1;
      }
      while(r < 0.5)
      {
         *exp -= 1;
         r.value().e += 1;
      }
      BOOST_ASSERT(r < 1);
      BOOST_ASSERT(r >= 0.5);
      return r;
   }
   inline RR ldexp(RR r, int exp)
   {
      r.value().e += exp;
      return r;
   }

// Streaming:
template <class charT, class traits>
inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const RR& a)
{
   return os << a.value();
}
template <class charT, class traits>
inline std::basic_istream<charT, traits>& operator>>(std::basic_istream<charT, traits>& is, RR& a)
{
   ::NTL::RR v;
   is >> v;
   a = v;
   return is;
}

} // namespace ntl

namespace tools
{

template<>
inline int digits<boost::math::ntl::RR>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(boost::math::ntl::RR))
{
   return ::NTL::RR::precision();
}

template <>
inline float real_cast<float, boost::math::ntl::RR>(boost::math::ntl::RR t)
{
   double r;
   conv(r, t.value());
   return static_cast<float>(r);
}
template <>
inline double real_cast<double, boost::math::ntl::RR>(boost::math::ntl::RR t)
{
   double r;
   conv(r, t.value());
   return r;
}

namespace detail{

template<class I>
void convert_to_long_result(NTL::RR const& r, I& result)
{
   result = 0;
   I last_result(0);
   NTL::RR t(r);
   double term;
   do
   {
      conv(term, t);
      last_result = result;
      result += static_cast<I>(term);
      t -= term;
   }while(result != last_result);
}

}

template <>
inline long double real_cast<long double, boost::math::ntl::RR>(boost::math::ntl::RR t)
{
   long double result(0);
   detail::convert_to_long_result(t.value(), result);
   return result;
}
template <>
inline boost::math::ntl::RR real_cast<boost::math::ntl::RR, boost::math::ntl::RR>(boost::math::ntl::RR t)
{
   return t;
}
template <>
inline unsigned real_cast<unsigned, boost::math::ntl::RR>(boost::math::ntl::RR t)
{
   unsigned result;
   detail::convert_to_long_result(t.value(), result);
   return result;
}
template <>
inline int real_cast<int, boost::math::ntl::RR>(boost::math::ntl::RR t)
{
   unsigned result;
   detail::convert_to_long_result(t.value(), result);
   return result;
}

template <>
inline boost::math::ntl::RR max_value<boost::math::ntl::RR>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(boost::math::ntl::RR))
{
   static bool has_init = false;
   static NTL::RR val;
   if(!has_init)
   {
      val = 1;
      val.e = NTL_OVFBND-20;
      has_init = true;
   }
   return val;
}

template <>
inline boost::math::ntl::RR min_value<boost::math::ntl::RR>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(boost::math::ntl::RR))
{
   static bool has_init = false;
   static NTL::RR val;
   if(!has_init)
   {
      val = 1;
      val.e = -NTL_OVFBND+20;
      has_init = true;
   }
   return val;
}

template <>
inline boost::math::ntl::RR log_max_value<boost::math::ntl::RR>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(boost::math::ntl::RR))
{
   static bool has_init = false;
   static NTL::RR val;
   if(!has_init)
   {
      val = 1;
      val.e = NTL_OVFBND-20;
      val = log(val);
      has_init = true;
   }
   return val;
}

template <>
inline boost::math::ntl::RR log_min_value<boost::math::ntl::RR>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(boost::math::ntl::RR))
{
   static bool has_init = false;
   static NTL::RR val;
   if(!has_init)
   {
      val = 1;
      val.e = -NTL_OVFBND+20;
      val = log(val);
      has_init = true;
   }
   return val;
}

template <>
inline boost::math::ntl::RR epsilon<boost::math::ntl::RR>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(boost::math::ntl::RR))
{
   return ldexp(boost::math::ntl::RR(1), 1-boost::math::policies::digits<boost::math::ntl::RR, boost::math::policies::policy<> >());
}

} // namespace tools

//
// The number of digits precision in RR can vary with each call
// so we need to recalculate these with each call:
//
namespace constants{

template<> inline boost::math::ntl::RR pi<boost::math::ntl::RR>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(boost::math::ntl::RR))
{
    NTL::RR result;
    ComputePi(result);
    return result;
}
template<> inline boost::math::ntl::RR e<boost::math::ntl::RR>(BOOST_MATH_EXPLICIT_TEMPLATE_TYPE_SPEC(boost::math::ntl::RR))
{
    NTL::RR result;
    result = 1;
    return exp(result);
}

} // namespace constants

namespace ntl{
   //
   // These are some fairly brain-dead versions of the math
   // functions that NTL fails to provide.
   //


   //
   // Inverse trig functions:
   //
   struct asin_root
   {
      asin_root(RR const& target) : t(target){}

      std::tr1::tuple<RR, RR, RR> operator()(RR const& p)
      {
         RR f0 = sin(p);
         RR f1 = cos(p);
         RR f2 = -f0;
         f0 -= t;
         return std::tr1::make_tuple(f0, f1, f2);
      }
   private:
      RR t;
   };

   inline RR asin(RR z)
   {
      double r;
      conv(r, z.value());
      return boost::math::tools::halley_iterate(
         asin_root(z), 
         RR(std::asin(r)), 
         RR(-boost::math::constants::pi<RR>()/2),
         RR(boost::math::constants::pi<RR>()/2),
         NTL::RR::precision());
   }

   struct acos_root
   {
      acos_root(RR const& target) : t(target){}

      std::tr1::tuple<RR, RR, RR> operator()(RR const& p)
      {
         RR f0 = cos(p);
         RR f1 = -sin(p);
         RR f2 = -f0;
         f0 -= t;
         return std::tr1::make_tuple(f0, f1, f2);
      }
   private:
      RR t;
   };

   inline RR acos(RR z)
   {
      double r;
      conv(r, z.value());
      return boost::math::tools::halley_iterate(
         acos_root(z), 
         RR(std::acos(r)), 
         RR(-boost::math::constants::pi<RR>()/2),
         RR(boost::math::constants::pi<RR>()/2),
         NTL::RR::precision());
   }

   struct atan_root
   {
      atan_root(RR const& target) : t(target){}

      std::tr1::tuple<RR, RR, RR> operator()(RR const& p)
      {
         RR c = cos(p);
         RR ta = tan(p);
         RR f0 = ta - t;
         RR f1 = 1 / (c * c);
         RR f2 = 2 * ta / (c * c);
         return std::tr1::make_tuple(f0, f1, f2);
      }
   private:
      RR t;
   };

   inline RR atan(RR z)
   {
      double r;
      conv(r, z.value());
      return boost::math::tools::halley_iterate(
         atan_root(z), 
         RR(std::atan(r)), 
         -boost::math::constants::pi<RR>()/2,
         boost::math::constants::pi<RR>()/2,
         NTL::RR::precision());
   }

   inline RR sinh(RR z)
   {
      return (expm1(z.value()) - expm1(-z.value())) / 2;
   }

   inline RR cosh(RR z)
   {
      return (exp(z) + exp(-z)) / 2;
   }

   inline RR tanh(RR z)
   {
      return sinh(z) / cosh(z);
   }

   inline RR fmod(RR x, RR y)
   {
      // This is a really crummy version of fmod, we rely on lots
      // of digits to get us out of trouble...
      RR factor = floor(x/y);
      return x - factor * y;
   }

} // namespace ntl

} // namespace math
} // namespace boost

#endif // BOOST_MATH_REAL_CONCEPT_HPP


