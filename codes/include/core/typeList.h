/*
 *  TypeList.h
 *	metohd from loki
 *  Created by liang pengcheng on 08-5-9.
 *  Copyright 2008 Quartz Game Studio. All rights reserved.
 *
 */

#ifndef FXTYPELIST_H
#define FXTYPELIST_H

#define TYPELIST_1(T1) tng::Typelist<T1, NullType>

#define TYPELIST_2(T1, T2) tng::Typelist<T1, TYPELIST_1(T2) >

#define TYPELIST_3(T1, T2, T3) tng::Typelist<T1, TYPELIST_2(T2, T3) >

#define TYPELIST_4(T1, T2, T3, T4) \
	tng::Typelist<T1, TYPELIST_3(T2, T3, T4) >

#define TYPELIST_5(T1, T2, T3, T4, T5) \
	tng::Typelist<T1, TYPELIST_4(T2, T3, T4, T5) >

#define TYPELIST_6(T1, T2, T3, T4, T5, T6) \
	tng::Typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6) >

#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
	tng::Typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >

#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
	tng::Typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
	tng::Typelist<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
	tng::Typelist<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >


//--------------------------------------------------
namespace tng
{
	class NullType
	{
	};
	class EmptyType {};
	/**
	class template Typelist
	The building block of typelists of any Length
	Use it through the TYPELIST_NN macros
	Defines nested types:
	Head (first element, a non-typelist type by convention)
	Tail (second element, can be another typelist)
	*/
	template <class T, class U>
	struct Typelist
	{
		typedef T Head;
		typedef U Tail;
	};



	/**
	Computes the Length of a typelist
	Invocation (TList is a typelist):
	Length<TList>::value
	returns a compile-time constant containing the Length of TList, not counting
	the end terminator (which by convention is NullType)
	*/
	template <class TList> struct Length;

	template <>
	struct Length<NullType>
	{
		enum { value = 0 };
	};


	template <class TList>
	struct Length
	{
	private:
		typedef typename TList::Head Head;
		typedef typename TList::Tail Tail;

	public:
		enum { value = 1 + Length<Tail>::value };
	};

	/**
	TypeAt<TList, index>::Result
	returns the type in position 'index' in TList
	If you pass an out-of-bounds index, the result is a compile-time error
	*/
	template <class TList, unsigned int index> struct TypeAt;

	template <class Head, class Tail>
	struct TypeAt<Typelist<Head, Tail>, 0>
	{
		typedef Head Result;
	};

	template <class Head, class Tail, unsigned int i>
	struct TypeAt<Typelist<Head, Tail>, i>
	{
		typedef typename TypeAt<Tail, i - 1>::Result Result;
	};
}


#endif


