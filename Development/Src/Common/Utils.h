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
	* min/max/clamp/lerp
	*
	\******************************************************************************/

	/*
	 */
	template <class Type> ENGINE_INLINE Type minF(Type v0, Type v1) {
		return (v0 < v1) ? v0 : v1;
	}

	template <class Type> ENGINE_INLINE Type maxF(Type v0, Type v1) {
		return (v0 > v1) ? v0 : v1;
	}

	template <class Type> ENGINE_INLINE Type clampF(Type v, Type v0, Type v1) {
		if (v < v0) return v0;
		if (v > v1) return v1;
		return v;
	}

	template <class Type> ENGINE_INLINE Type lerpF(Type v0, Type v1, Type k) {
		return v0 + (v1 - v0) * k;
	}
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

	/******************************************************************************\
	*
	* quickDoubleSort
	*
	\******************************************************************************/

	/*
	 */
	template <class Type, class Data, class Compare> void quick_double_sort(Type *array, Data *data, int size, Compare compare) {
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
					swap(data[l], data[r]);
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
				assert(depth < 1024 && "quick_double_sort(): stack overflow");
				left_stack[depth] = left;
				right_stack[depth++] = r;
			}
			if (l < right) {
				assert(depth < 1024 && "quick_double_sort(): stack overflow");
				left_stack[depth] = l;
				right_stack[depth++] = right;
			}
		}
	}

	/*
	 */
	template <class Type, class Data> void quickDoubleSort(Type *array, Data *data, int size) {
		if (size < 2) return;
		quick_sort_default_compare<Type> compare;
		quick_double_sort(array, data, size, compare);
	}

	template <class Type, class Data, class Compare> void quickDoubleSort(Type *array, Data *data, int size, Compare compare) {
		if (size < 2) return;
		quick_double_sort(array, data, size, compare);
	}

	template <class Type, class Data, class A0, class A1> void quickSort(Type *array, Data *data, int size, int(*func)(A0, A1)) {
		if (size < 2) return;
		quick_sort_function_compare<Type, int(*)(A0, A1)> compare(func);
		quick_double_sort(array, data, size, compare);
	}

	/******************************************************************************\
	*
	* quickSort2
	*
	\******************************************************************************/

	/*
	 */
	template <class Type> struct quick_sort_2_default_compare {
		ENGINE_INLINE int operator()(const Type &t0, const Type &t1) const {
			if (t0 < t1) return 1;
			if (t0 > t1) return -1;
			return 0;
		}
	};

	template <class Type> struct quick_sort_2_default_compare < Type* > {
		ENGINE_INLINE int operator()(const Type *t0, const Type *t1) const {
			if (t0 < t1) return 1;
			if (t0 > t1) return -1;
			return 0;
		}
	};

	/*
	 */
	template <> struct quick_sort_2_default_compare < char > {
		ENGINE_INLINE int operator()(char i0, char i1) const {
			if (i0 < i1) return 1;
			if (i0 > i1) return -1;
			return 0;
		}
	};

	template <> struct quick_sort_2_default_compare < unsigned char > {
		ENGINE_INLINE int operator()(unsigned char i0, unsigned char i1) const {
			if (i0 < i1) return 1;
			if (i0 > i1) return -1;
			return 0;
		}
	};

	/*
	 */
	template <> struct quick_sort_2_default_compare < short > {
		ENGINE_INLINE int operator()(short i0, short i1) const {
			if (i0 < i1) return 1;
			if (i0 > i1) return -1;
			return 0;
		}
	};

	template <> struct quick_sort_2_default_compare < unsigned short > {
		ENGINE_INLINE int operator()(unsigned short i0, unsigned short i1) const {
			if (i0 < i1) return 1;
			if (i0 > i1) return -1;
			return 0;
		}
	};

	/*
	 */
	template <> struct quick_sort_2_default_compare < int > {
		ENGINE_INLINE int operator()(int i0, int i1) const {
			if (i0 < i1) return 1;
			if (i0 > i1) return -1;
			return 0;
		}
	};

	template <> struct quick_sort_2_default_compare < unsigned int > {
		ENGINE_INLINE int operator()(unsigned int i0, unsigned int i1) const {
			if (i0 < i1) return 1;
			if (i0 > i1) return -1;
			return 0;
		}
	};

	/*
	 */
	template <> struct quick_sort_2_default_compare < float > {
		ENGINE_INLINE int operator()(float f0, float f1) const {
			if (f0 < f1) return 1;
			if (f0 > f1) return -1;
			return 0;
		}
	};

	/*
	 */
	template <class Type, class Func> struct quick_sort_2_function_compare {
		quick_sort_2_function_compare(Func func) : func(func) { }
		ENGINE_INLINE int operator()(const Type &t0, const Type &t1) const {
			return func(t0, t1);
		}
		Func func;
	};

	/*
	 */
	template <class Type, class Compare> void quick_sort_2(Type *array, int size, Compare compare) {
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
				while (l < right && compare(array[l], c) > 0) l++;
				while (r > left && compare(array[r], c) < 0) r--;
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
				assert(depth < 1024 && "quick_sort_2(): stack overflow");
				left_stack[depth] = left;
				right_stack[depth++] = r;
			}
			if (l < right) {
				assert(depth < 1024 && "quick_sort_2(): stack overflow");
				left_stack[depth] = l;
				right_stack[depth++] = right;
			}
		}
	}

	/*
	 */
	template <class Type> void quickSort2(Type *array, int size) {
		if (size < 2) return;
		quick_sort_2_default_compare<Type> compare;
		quick_sort_2(array, size, compare);
	}

	template <class Type, class Compare> void quickSort2(Type *array, int size, Compare compare) {
		if (size < 2) return;
		quick_sort_2(array, size, compare);
	}

	template <class Type, class A0, class A1> void quickSort2(Type *array, int size, int(*func)(A0, A1)) {
		if (size < 2) return;
		quick_sort_2_function_compare<Type, int(*)(A0, A1)> compare(func);
		quick_sort_2(array, size, compare);
	}

	/*
	 */
	template <class Iterator> void quickSort2(Iterator begin, Iterator end) {
		int size = end - begin;
		if (size < 2) return;
		quick_sort_2_default_compare<typename Iterator::DataType> compare;
		quick_sort_2(&(*begin), size, compare);
	}

	template <class Iterator, class Compare> void quickSort2(Iterator begin, Iterator end, Compare compare) {
		int size = end - begin;
		if (size < 2) return;
		quick_sort_2(&(*begin), size, compare);
	}

	template <class Iterator, class A0, class A1> void quickSort2(Iterator begin, Iterator end, int(*func)(A0, A1)) {
		int size = end - begin;
		if (size < 2) return;
		quick_sort_2_function_compare<typename Iterator::DataType, int(*)(A0, A1)> compare(func);
		quick_sort_2(&(*begin), size, compare);
	}

	/******************************************************************************\
	*
	* quickDoubleSort2
	*
	\******************************************************************************/

	/*
	 */
	template <class Type, class Data, class Compare> void quick_double_sort_2(Type *array, Data *data, int size, Compare compare) {
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
				while (l < right && compare(array[l], c) > 0) l++;
				while (r > left && compare(array[r], c) < 0) r--;
				if (l < r) {
					swap(array[l], array[r]);
					swap(data[l], data[r]);
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
				assert(depth < 1024 && "quick_double_sort_2(): stack overflow");
				left_stack[depth] = left;
				right_stack[depth++] = r;
			}
			if (l < right) {
				assert(depth < 1024 && "quick_double_sort_2(): stack overflow");
				left_stack[depth] = l;
				right_stack[depth++] = right;
			}
		}
	}

	/*
	 */
	template <class Type, class Data> void quickDoubleSort2(Type *array, Data *data, int size) {
		if (size < 2) return;
		quick_sort_2_default_compare<Type> compare;
		quick_double_sort_2(array, data, size, compare);
	}

	template <class Type, class Data, class Compare> void quickDoubleSort2(Type *array, Data *data, int size, Compare compare) {
		if (size < 2) return;
		quick_double_sort_2(array, data, size, compare);
	}

	template <class Type, class Data, class A0, class A1> void quickDoubleSort2(Type *array, Data *data, int size, int(*func)(A0, A1)) {
		if (size < 2) return;
		quick_sort_2_function_compare<Type, int(*)(A0, A1)> compare(func);
		quick_double_sort_2(array, data, size, compare);
	}

	/******************************************************************************\
	*
	* radixSort
	*
	\******************************************************************************/

	/*
	 */
	template <class Type> struct radix_sort_default_compare;

	/*
	 */
	template <class Type> struct radix_sort_default_compare < Type* > {
		typedef size_t Hash;
		ENGINE_INLINE Hash operator()(Type *p) const {
			return (size_t)p;
		}
	};

	/*
	 */
	template <> struct radix_sort_default_compare < char > {
		typedef unsigned char Hash;
		ENGINE_INLINE Hash operator()(char i) const {
			return i ^ 0x80;
		}
	};

	template <> struct radix_sort_default_compare < unsigned char > {
		typedef unsigned char Hash;
		ENGINE_INLINE Hash operator()(unsigned char i) const {
			return i;
		}
	};

	/*
	 */
	template <> struct radix_sort_default_compare < short > {
		typedef unsigned short Hash;
		ENGINE_INLINE Hash operator()(short i) const {
			return i ^ 0x8000;
		}
	};

	template <> struct radix_sort_default_compare < unsigned short > {
		typedef unsigned short Hash;
		ENGINE_INLINE Hash operator()(unsigned short i) const {
			return i;
		}
	};

	/*
	 */
	template <> struct radix_sort_default_compare < int > {
		typedef unsigned int Hash;
		ENGINE_INLINE Hash operator()(int i) const {
			return i ^ 0x80000000;
		}
	};

	template <> struct radix_sort_default_compare < unsigned int > {
		typedef unsigned int Hash;
		ENGINE_INLINE Hash operator()(unsigned int i) const {
			return i;
		}
	};

	/*
	 */
	template <> struct radix_sort_default_compare < long long > {
		typedef unsigned long long Hash;
		ENGINE_INLINE Hash operator()(long long i) const {
			return i ^ 0x8000000000000000LL;
		}
	};

	template <> struct radix_sort_default_compare < unsigned long long > {
		typedef unsigned long long Hash;
		ENGINE_INLINE Hash operator()(unsigned long long i) const {
			return i;
		}
	};

	/*
	 */
	template <> struct radix_sort_default_compare < float > {
		typedef unsigned int Hash;
		ENGINE_INLINE Hash operator()(float f) const {
			union IntFloat {
				unsigned int i;
				float f;
			};
			IntFloat hash;
			hash.f = f;
			if (hash.i & 0x80000000) return hash.i ^ 0xffffffff;
			return hash.i ^ 0x80000000;
		}
	};

	/*
	 */
	template <class Type, class Ret, class Func> struct radix_sort_function_compare {
		radix_sort_function_compare(Func func) : func(func) { }
		typedef Ret Hash;
		ENGINE_INLINE Hash operator()(const Type &t) const {
			return func(t);
		}
		Func func;
	};

	/*
	 */
	template <class Type, class Compare> class RadixSort {

	public:

		RadixSort(Type *array, int size, Compare compare) {
			reserve(size);
			for (int i = 0; i < size; i++) {
				src[i].hash = compare(array[i]);
				src[i].index = i;
				data[i] = array[i];
			}
			int index[256];
			int distribution[256];
#ifdef USE_BIG_ENDIAN
			for(int j = (int)sizeof(Hash) - 1; j >= 0; j--) {
#else
			for (int j = 0; j < (int)sizeof(Hash); j++) {
#endif
				for (int i = 0; i < 256; i += 4) {
					distribution[i + 0] = 0;
					distribution[i + 1] = 0;
					distribution[i + 2] = 0;
					distribution[i + 3] = 0;
				}
				for (int i = 0; i < size; i++) {
					distribution[get_byte(src[i].hash, j)]++;
				}
				index[0] = 0;
				index[1] = distribution[0];
				index[2] = index[1] + distribution[1];
				index[3] = index[2] + distribution[2];
				for (int i = 4; i < 256; i += 4) {
					index[i + 0] = index[i - 1] + distribution[i - 1];
					index[i + 1] = index[i + 0] + distribution[i + 0];
					index[i + 2] = index[i + 1] + distribution[i + 1];
					index[i + 3] = index[i + 2] + distribution[i + 2];
				}
				for (int i = 0; i < size; i++) {
					dest[index[get_byte(src[i].hash, j)]++] = src[i];
				}
				swap(src, dest);
			}
			for (int i = 0; i < size; i++) {
				array[i] = data[src[i].index];
			}
		}

		static void clear() {
			capacity = 0;
			delete[] src;
			delete[] dest;
			delete[] data;
			src = 0;
			dest = 0;
			data = 0;
		}

		static void allocate(int size) {
			if (size <= capacity) return;
			capacity = size;
			delete[] src;
			delete[] dest;
			delete[] data;
			src = new Data[capacity];
			dest = new Data[capacity];
			data = new Type[capacity];
		}

		static void reserve(int size) {
			if (size <= capacity) return;
			capacity = size * 2;
			delete[] src;
			delete[] dest;
			delete[] data;
			src = new Data[capacity];
			dest = new Data[capacity];
			data = new Type[capacity];
		}

	private:

		typedef typename Compare::Hash Hash;

		ENGINE_INLINE int get_byte(const Hash &hash, int i) const {
			return *((const unsigned char*)&hash + i);
		}

		struct Data {
			Hash hash;
			int index;
		};

		static int capacity;
		static Data *src;
		static Data *dest;
		static Type *data;
	};

	/*
	 */
	template <class Type, class Compare> int RadixSort<Type, Compare>::capacity = 0;
	template <class Type, class Compare> typename RadixSort<Type, Compare>::Data *RadixSort<Type, Compare>::src = 0;
	template <class Type, class Compare> typename RadixSort<Type, Compare>::Data *RadixSort<Type, Compare>::dest = 0;
	template <class Type, class Compare> Type *RadixSort<Type, Compare>::data = 0;

	/*
	 */
	template <class Type> void radixSort(Type *array, int size) {
		if (size < 2) return;
		radix_sort_default_compare<Type> compare;
		RadixSort<Type, radix_sort_default_compare<Type> > radix_sort(array, size, compare);
	}

	template <class Type, class Compare> void radixSort(Type *array, int size, Compare compare) {
		if (size < 2) return;
		RadixSort<Type, Compare> radix_sort(array, size, compare);
	}

	template <class Type, class Ret, class A0, class A1> void radixSort(Type *array, int size, Ret(*func)(A0, A1)) {
		if (size < 2) return;
		radix_sort_function_compare<Type, Ret, Ret(*)(A0, A1)> compare(func);
		RadixSort<Type, radix_sort_function_compare<Type, Ret, Ret(*)(A0, A1)> > radix_sort(array, size, compare);
	}

	/*
	 */
	template <class Iterator> void radixSort(Iterator begin, Iterator end) {
		int size = end - begin;
		if (size < 2) return;
		radix_sort_default_compare<typename Iterator::DataType> compare;
		RadixSort<typename Iterator::DataType, radix_sort_default_compare<typename Iterator::DataType> > radix_sort(&(*begin), size, compare);
	}

	template <class Iterator, class Compare> void radixSort(Iterator begin, Iterator end, Compare compare) {
		int size = end - begin;
		if (size < 2) return;
		RadixSort<typename Iterator::DataType, Compare> radix_sort(&(*begin), size, compare);
	}

	template <class Iterator, class Ret, class A0, class A1> void radixSort(Iterator begin, Iterator end, Ret(*func)(A0, A1)) {
		int size = end - begin;
		if (size < 2) return;
		radix_sort_function_compare<typename Iterator::DataType, Ret, Ret(*)(A0, A1)> compare(func);
		RadixSort<typename Iterator::DataType, radix_sort_function_compare<typename Iterator::DataType, Ret, Ret(*)(A0, A1)> > radix_sort(&(*begin), size, compare);
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