/* Copyright (C) 2005-2010, Unigine Corp. All rights reserved.
 *
 * File:    Vector.h
 * Desc:    Vector container
 * Version: 1.21
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

#ifndef __VECTOR_H__
#define __VECTOR_H__


/*
 */
template <class Type,class Counter = int> class Vector {
		
	public:
		
		// Vector::Iterator
		class Iterator {
				
			private:
				
				friend class Vector;
				
				Iterator(Type *ptr) : ptr(ptr) { }
				
			public:
				
				typedef Type DataType;
				
				Iterator() : ptr(0) { }
				Iterator(const Iterator &it) : ptr(it.ptr) { }
				~Iterator() { }
				
				Iterator &operator=(const Iterator &it) {
					ptr = it.ptr;
					return *this;
				}
				
				ENGINE_INLINE int operator==(const Iterator &it) const { return ptr == it.ptr; }
				ENGINE_INLINE int operator!=(const Iterator &it) const { return ptr != it.ptr; }
				
				ENGINE_INLINE Type &operator*() { return *ptr; }
				ENGINE_INLINE const Type &operator*() const { return *ptr; }
				
				ENGINE_INLINE Type *operator->() { return ptr; }
				ENGINE_INLINE const Type *operator->() const { return ptr; }
				
				ENGINE_INLINE Type &get() { return *ptr; }
				ENGINE_INLINE const Type &get() const { return *ptr; }
				
				ENGINE_INLINE Iterator &operator++() {
					ptr++;
					return *this;
				}
				ENGINE_INLINE Iterator operator++(int) {
					Iterator temp = *this;
					ptr++;
					return temp;
				}
				
				Iterator operator+(int n) const { return Iterator(ptr + n); }
				Iterator operator-(int n) const { return Iterator(ptr - n); }
				Iterator &operator+=(int n) { ptr += n; return *this; }
				Iterator &operator-=(int n) { ptr -= n; return *this; }
				
				int operator-(const Iterator &it) const { return (int)(ptr - it.ptr); }
				
			private:
				
				Type *ptr;
		};
		
		// Vector
		Vector() : length(0), capacity(0), data(0) {
			
		}
		Vector(const Vector &v) : length(0), capacity(0), data(0) {
			append(v);
		}
		Vector(const Type &v,int size) : length(0), capacity(0), data(0) {
			resize(size);
			for(int i = 0; i < size; i++) {
				data[i] = v;
			}
		}
		Vector(const Type *v,int size) : length(0), capacity(0), data(0) {
			append(v,size);
		}
		explicit Vector(int size) : length(0), capacity(0), data(0) {
			resize(size);
		}
		~Vector() {
			destroy();
		}
		
		Vector &operator=(const Vector &v) {
			if(this == &v) return *this;
			resize(v.length);
			for(int i = 0; i < length; i++) {
				data[i] = v.data[i];
			}
			return *this;
		}
		void swap(Vector &v) {
			if(this == &v) return;
			Counter l = length; length = v.length; v.length = l;
			Counter c = capacity; capacity = v.capacity; v.capacity = c;
			Type *d = data; data = v.data; v.data = d;
		}
		
		ENGINE_INLINE Type &operator[](int index) {
			assert(index >= 0 && (Counter)index < length && "Vector::operator[](): bad index");
			return data[index];
		}
		ENGINE_INLINE const Type &operator[](int index) const {
			assert(index >= 0 && (Counter)index < length && "Vector::operator[](): bad index");
			return data[index];
		}
		
		ENGINE_INLINE void set(int index, const Type &t) {
			assert(index >= 0 && (Counter)index < length && "Vector::set(): bad index");
			data[index] = t;
		}
		ENGINE_INLINE Type &get(int index) {
			assert(index >= 0 && (Counter)index < length && "Vector::get(): bad index");
			return data[index];
		}
		ENGINE_INLINE const Type &get(int index) const {
			assert(index >= 0 && (Counter)index < length && "Vector::get(): bad index");
			return data[index];
		}
		
		ENGINE_INLINE Type *get() { return data; }
		ENGINE_INLINE const Type *get() const { return data; }
		
		ENGINE_INLINE Iterator begin() const { return Iterator(data); }
		ENGINE_INLINE Iterator end() const { return Iterator(data + length); }
		
		ENGINE_INLINE int size() const { return length; }
		ENGINE_INLINE int empty() const { return (length == 0); }
		
		void resize(int size) {
			allocate(size);
			length = size;
		}
		
		void allocate(int size) {
			if(size <= (capacity & CAPACITY_MASK)) return;
			int dynamic = (capacity & DYNAMIC_FLAG);
			capacity = size | DYNAMIC_FLAG;
			Type *new_data = new Type[size];
			for(int i = 0; i < length; i++) {
				new_data[i] = data[i];
			}
			if(dynamic) delete [] data;
			data = new_data;
		}
		
		void reserve(int size) {
			if(size <= (capacity & CAPACITY_MASK)) return;
			int dynamic = (capacity & DYNAMIC_FLAG);
			capacity = (size * 2) | DYNAMIC_FLAG;
			Type *new_data = new Type[size * 2];
			for(int i = 0; i < length; i++) {
				new_data[i] = data[i];
			}
			if(dynamic) delete [] data;
			data = new_data;
		}
		
		void clear() {
			length = 0;
		}
		void destroy() {
			int dynamic = (capacity & DYNAMIC_FLAG);
			length = 0;
			capacity = 0;
			if(dynamic) delete [] data;
			data = 0;
		}
		
		template <class T> Iterator find(const T &t) const {
			for(int i = 0; i < length; i++) {
				if(data[i] == t) return Iterator(data + i);
			}
			return Iterator(data + length);
		}
		
		ENGINE_INLINE Type &append() {
			if(length + 1 > (capacity & CAPACITY_MASK)) {
				reserve(length + 1);
			}
			return data[(int)(length++)];
		}
		ENGINE_INLINE void append(const Type &t) {
			if(length + 1 > (capacity & CAPACITY_MASK)) {
				reserve(length + 1);
			}
			data[(int)(length++)] = t;
		}
		void append(int pos,const Type &t) {
			assert(pos >= 0 && pos <= length && "Vector::append(): bad position");
			if(length + 1 <= (capacity & CAPACITY_MASK)) {
				for(int i = length - 1; i >= pos; i--) {
					data[i + 1] = data[i];
				}
				data[pos] = t;
				length++;
				return;
			}
			int dynamic = (capacity & DYNAMIC_FLAG);
			capacity = (length * 2 + 1) | DYNAMIC_FLAG;
			Type *new_data = new Type[length * 2 + 1];
			for(int i = 0; i < pos; i++) {
				new_data[i] = data[i];
			}
			new_data[pos] = t;
			for(int i = pos; i < length; i++) {
				new_data[i + 1] = data[i];
			}
			if(dynamic) delete [] data;
			data = new_data;
			length++;
		}
		ENGINE_INLINE void append(const Iterator &it, const Type &t) {
			append(it - begin(),t);
		}
		
		void append(const Vector &v) {
			if(length + v.length > (capacity & CAPACITY_MASK)) {
				reserve(length + v.length);
			}
			for(int i = 0; i < v.length; i++) {
				data[(int)(length++)] = v.data[i];
			}
		}
		void append(const Type *v,int size) {
			if(length + size > (capacity & CAPACITY_MASK)) {
				reserve(length + size);
			}
			for(int i = 0; i < size; i++) {
				data[(int)(length++)] = v[i];
			}
		}
		
		ENGINE_INLINE void remove() {
			assert(length > 0 && "Vector::remove(): bad length");
			length--;
		}
		void remove(int pos,int size = 1) {
			assert(pos >= 0 && pos < length && "Vector::remove(): bad position");
			assert(size >= 0 && pos + size <= length && "Vector::remove(): bad size");
			for(int i = pos; i < length - size; i++) {
				data[i] = data[i + size];
			}
			length -= size;
		}
		ENGINE_INLINE void remove(const Iterator &it) {
			remove(it - begin());
		}
		
		ENGINE_INLINE void removeFast(int pos) {
			assert(pos >= 0 && pos < length && "Vector::removeFast(): bad position");
			data[pos] = data[(int)(length - 1)];
			length--;
		}
		ENGINE_INLINE void removeFast(const Iterator &it) {
			removeFast(it - begin());
		}
		
	protected:
		
		enum {
			DYNAMIC_FLAG = (Counter)(1 << (sizeof(Counter) * 8 - 1)),
			CAPACITY_MASK = ~(Counter)DYNAMIC_FLAG,
		};
		
		Counter length;
		Counter capacity;
		Type *data;
};

/*
 */
template <class Type,int Capacity = 128,class Counter = int> class VectorStack : public Vector<Type,Counter> {
		
	public:
		
		VectorStack() {
			Vector<Type,Counter>::capacity = Capacity;
			Vector<Type,Counter>::data = stack_data;
		}
		explicit VectorStack(int size) {
			Vector<Type,Counter>::capacity = Capacity;
			Vector<Type,Counter>::data = stack_data;
			Vector<Type,Counter>::resize(size);
		}
		VectorStack(const Vector<Type,Counter> &v) {
			Vector<Type,Counter>::capacity = Capacity;
			Vector<Type,Counter>::data = stack_data;
			Vector<Type,Counter>::append(v);
		}
		VectorStack(const Type &v,int size) {
			Vector<Type,Counter>::capacity = Capacity;
			Vector<Type,Counter>::data = stack_data;
			Vector<Type,Counter>::resize(size);
			for(int i = 0; i < size; i++) {
				Vector<Type,Counter>::data[i] = v;
			}
		}
		VectorStack(const Type *v,int size) {
			Vector<Type,Counter>::capacity = Capacity;
			Vector<Type,Counter>::data = stack_data;
			Vector<Type,Counter>::append(v,size);
		}
		~VectorStack() { }
		
		VectorStack &operator=(const VectorStack &v) {
			if(this == &v) return *this;
			Vector<Type,Counter>::resize(v.length);
			for(int i = 0; i < Vector<Type,Counter>::length; i++) {
				Vector<Type,Counter>::data[i] = v.data[i];
			}
			return *this;
		}
		
	private:
		
		Type stack_data[Capacity];
};

#endif /* __VECTOR_H__ */