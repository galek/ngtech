/* Copyright_stack (C) 2005-2010, Unigine Corp. All right_stacks reserved.
 *
 * File:    Utils.h
 * Desc:    Utils
 * Version: 1.14
 * Author:  Alexander Zaprjagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#pragma once

namespace NGTech
{
	/******************************************************************************\
	*
	* swap
	*
	\******************************************************************************/

	/*
	 */
	template <class Type> ENGINE_INLINE void swap(Type &v0, Type &v1) {
		Type temp = v0;
		v0 = v1;
		v1 = temp;
	}

	/******************************************************************************\
	*
	* quickSort
	*
	\******************************************************************************/

	/*
	 */
	template <class Type> struct quick_sort_default_compare {
		ENGINE_INLINE int operator()(const Type &t0, const Type &t1) const {
			return (t0 < t1);
		}
	};

	template <class Type> struct quick_sort_default_compare < Type* > {
		ENGINE_INLINE int operator()(const Type *t0, const Type *t1) const {
			return (t0 < t1);
		}
	};

	/*
	 */
	template <> struct quick_sort_default_compare < char > {
		ENGINE_INLINE int operator()(char i0, char i1) const {
			return (i0 < i1);
		}
	};

	template <> struct quick_sort_default_compare < unsigned char > {
		ENGINE_INLINE int operator()(unsigned char i0, unsigned char i1) const {
			return (i0 < i1);
		}
	};

	/*
	 */
	template <> struct quick_sort_default_compare < short > {
		ENGINE_INLINE int operator()(short i0, short i1) const {
			return (i0 < i1);
		}
	};

	template <> struct quick_sort_default_compare < unsigned short > {
		ENGINE_INLINE int operator()(unsigned short i0, unsigned short i1) const {
			return (i0 < i1);
		}
	};

	/*
	 */
	template <> struct quick_sort_default_compare < int > {
		ENGINE_INLINE int operator()(int i0, int i1) const {
			return (i0 < i1);
		}
	};

	template <> struct quick_sort_default_compare < unsigned int > {
		ENGINE_INLINE int operator()(unsigned int i0, unsigned int i1) const {
			return (i0 < i1);
		}
	};

	/*
	 */
	template <> struct quick_sort_default_compare < float > {
		ENGINE_INLINE int operator()(float f0, float f1) const {
			return (f0 < f1);
		}
	};

	/*
	 */
	template <class Type, class Func> struct quick_sort_function_compare {
		quick_sort_function_compare(Func func) : func(func) { }
		ENGINE_INLINE int operator()(const Type &t0, const Type &t1) const {
			return func(t0, t1);
		}
		Func func;
	};

	/*
	 */
	template <class Type, class Compare> void quick_sort(Type *array, int size, Compare compare) {
		int depth = 1;
		int left_stack[1024];
		int right_stack[1024];
		left_stack[0] = 0;
		right_stack[0] = size - 1;
		while (depth > 0) {
			int left = left_stack[--depth];
			int right = right_stack[depth];
			int l = left;
			int r = right;
			int m = (l + r) >> 1;
			do {
				const Type &c = array[m];
				while (l < right && compare(array[l], c)) l++;
				while (r > left && compare(c, array[r])) r--;
				if (l < r) {
					swap(array[l], array[r]);
					if (m == l) m = r;
					else if (m == r) m = l;
					l++;
					r--;
				}
				else if (l == r) {
					l++;
					r--;
				}
			} while (l <= r);
			if (left < r) {
				assert(depth < 1024 && "quick_sort(): stack overflow");
				left_stack[depth] = left;
				right_stack[depth++] = r;
			}
			if (l < right) {
				assert(depth < 1024 && "quick_sort(): stack overflow");
				left_stack[depth] = l;
				right_stack[depth++] = right;
			}
		}
	}

	/*
	 */
	template <class Type> void quickSort(Type *array, int size) {
		if (size < 2) return;
		quick_sort_default_compare<Type> compare;
		quick_sort(array, size, compare);
	}

	template <class Type, class Compare> void quickSort(Type *array, int size, Compare compare) {
		if (size < 2) return;
		quick_sort(array, size, compare);
	}

	template <class Type, class A0, class A1> void quickSort(Type *array, int size, int(*func)(A0, A1)) {
		if (size < 2) return;
		quick_sort_function_compare<Type, int(*)(A0, A1)> compare(func);
		quick_sort(array, size, compare);
	}

	/*
	 */
	template <class Iterator> void quickSort(Iterator begin, Iterator end) {
		int size = end - begin;
		if (size < 2) return;
		quick_sort_default_compare<typename Iterator::DataType> compare;
		quick_sort(&(*begin), size, compare);
	}

	template <class Iterator, class Compare> void quickSort(Iterator begin, Iterator end, Compare compare) {
		int size = end - begin;
		if (size < 2) return;
		quick_sort(&(*begin), size, compare);
	}

	template <class Iterator, class A0, class A1> void quickSort(Iterator begin, Iterator end, int(*func)(A0, A1)) {
		int size = end - begin;
		if (size < 2) return;
		quick_sort_function_compare<typename Iterator::DataType, int(*)(A0, A1)> compare(func);
		quick_sort(&(*begin), size, compare);
	}


	/*
	*/
	template <class Index>
	void MeshConvertIndicesTo(int *dest, const int *src, int size) {
		const int *s = src;
		Index *d = (Index*)dest;
		for (int i = 0; i < size; i++) {
			*d++ = (Index)*s++;
		}
	}
}