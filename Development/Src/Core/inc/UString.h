#pragma once

// these are explained later
#include <iterator>
#include <string>
#include <stdexcept>

namespace NGTech
{
	class CORE_API UString {
		// constants used in UTF-8 conversions
		static const unsigned char _lead1 = 0xC0;      //110xxxxx
		static const unsigned char _lead1_mask = 0x1F; //00011111
		static const unsigned char _lead2 = 0xE0;      //1110xxxx
		static const unsigned char _lead2_mask = 0x0F; //00001111
		static const unsigned char _lead3 = 0xF0;      //11110xxx
		static const unsigned char _lead3_mask = 0x07; //00000111
		static const unsigned char _lead4 = 0xF8;      //111110xx
		static const unsigned char _lead4_mask = 0x03; //00000011
		static const unsigned char _lead5 = 0xFC;      //1111110x
		static const unsigned char _lead5_mask = 0x01; //00000001
		static const unsigned char _cont = 0x80;       //10xxxxxx
		static const unsigned char _cont_mask = 0x3F;  //00111111

	public:
		//! size type used to indicate string size and character positions within the string
		typedef size_t size_type;
		//! the usual constant representing: not found, no limit, etc
		static const size_type npos = static_cast<size_type>(~0);

		//! a single 32-bit Unicode character
		typedef unsigned int unicode_char;

		//! a single UTF-16 code point
		typedef unsigned short code_point;

		//! value type typedef for use in iterators
		typedef code_point value_type;

		typedef std::basic_string<code_point> dstring; // data string

		//! string type used for returning UTF-32 formatted data
		typedef std::basic_string<unicode_char> utf32string;

		//! This exception is used when invalid data streams are encountered
		class CORE_API invalid_data : public std::runtime_error { /* i don't know why the beautifier is freaking out on this line */
		public:
			//! constructor takes a string message that can be later retrieved by the what() function
			explicit invalid_data(const std::string& _Message) : std::runtime_error(_Message) {
				/* The thing is, Bob, it's not that I'm lazy, it's that I just don't care. */
			}
		};

		//#########################################################################
		//! base iterator class for UString
		class CORE_API _base_iterator : public std::iterator < std::random_access_iterator_tag, value_type > { /* i don't know why the beautifier is freaking out on this line */
			friend class UString;
		protected:
			_base_iterator();

			void _seekFwd(size_type c);
			void _seekRev(size_type c);
			void _become(const _base_iterator& i);
			bool _test_begin() const;
			bool _test_end() const;
			size_type _get_index() const;
			void _jump_to(size_type index);

			unicode_char _getCharacter() const;
			int _setCharacter(unicode_char uc);

			void _moveNext();
			void _movePrev();

			dstring::iterator mIter;
			UString* mString;
		};

		//#########################################################################
		// FORWARD ITERATORS
		//#########################################################################
		class _const_fwd_iterator; // forward declaration

		//! forward iterator for UString
		class CORE_API _fwd_iterator : public _base_iterator { /* i don't know why the beautifier is freaking out on this line */
			friend class _const_fwd_iterator;
		public:
			_fwd_iterator();
			_fwd_iterator(const _fwd_iterator& i);

			//! pre-increment
			_fwd_iterator& operator++();
			//! post-increment
			_fwd_iterator operator++(int);

			//! pre-decrement
			_fwd_iterator& operator--();
			//! post-decrement
			_fwd_iterator operator--(int);

			//! addition operator
			_fwd_iterator operator+(difference_type n);
			//! subtraction operator
			_fwd_iterator operator-(difference_type n);

			//! addition assignment operator
			_fwd_iterator& operator+=(difference_type n);
			//! subtraction assignment operator
			_fwd_iterator& operator-=(difference_type n);

			//! dereference operator
			value_type& operator*() const;

			//! dereference at offset operator
			value_type& operator[](difference_type n) const;

			//! advances to the next Unicode character, honoring surrogate pairs in the UTF-16 stream
			_fwd_iterator& moveNext();
			//! rewinds to the previous Unicode character, honoring surrogate pairs in the UTF-16 stream
			_fwd_iterator& movePrev();
			//! Returns the Unicode value of the character at the current position (decodes surrogate pairs if needed)
			unicode_char getCharacter() const;
			//! Sets the Unicode value of the character at the current position (adding a surrogate pair if needed); returns the amount of string length change caused by the operation
			int setCharacter(unicode_char uc);
		};



		//#########################################################################
		//! const forward iterator for UString
		class CORE_API _const_fwd_iterator : public _base_iterator { /* i don't know why the beautifier is freaking out on this line */
		public:
			_const_fwd_iterator();
			_const_fwd_iterator(const _const_fwd_iterator& i);
			_const_fwd_iterator(const _fwd_iterator& i);

			//! pre-increment
			_const_fwd_iterator& operator++();
			//! post-increment
			_const_fwd_iterator operator++(int);

			//! pre-decrement
			_const_fwd_iterator& operator--();
			//! post-decrement
			_const_fwd_iterator operator--(int);

			//! addition operator
			_const_fwd_iterator operator+(difference_type n);
			//! subtraction operator
			_const_fwd_iterator operator-(difference_type n);

			//! addition assignment operator
			_const_fwd_iterator& operator+=(difference_type n);
			//! subtraction assignment operator
			_const_fwd_iterator& operator-=(difference_type n);

			//! dereference operator
			const value_type& operator*() const;

			//! dereference at offset operator
			const value_type& operator[](difference_type n) const;

			//! advances to the next Unicode character, honoring surrogate pairs in the UTF-16 stream
			_const_fwd_iterator& moveNext();
			//! rewinds to the previous Unicode character, honoring surrogate pairs in the UTF-16 stream
			_const_fwd_iterator& movePrev();
			//! Returns the Unicode value of the character at the current position (decodes surrogate pairs if needed)
			unicode_char getCharacter() const;

			//! difference operator
			friend size_type operator-(const _const_fwd_iterator& left, const _const_fwd_iterator& right);
			//! equality operator
			friend bool operator==(const _const_fwd_iterator& left, const _const_fwd_iterator& right);
			//! inequality operator
			friend bool operator!=(const _const_fwd_iterator& left, const _const_fwd_iterator& right);
			//! less than
			friend bool operator<(const _const_fwd_iterator& left, const _const_fwd_iterator& right);
			//! less than or equal
			friend bool operator<=(const _const_fwd_iterator& left, const _const_fwd_iterator& right);
			//! greater than
			friend bool operator>(const _const_fwd_iterator& left, const _const_fwd_iterator& right);
			//! greater than or equal
			friend bool operator>=(const _const_fwd_iterator& left, const _const_fwd_iterator& right);

		};

		//#########################################################################
		// REVERSE ITERATORS
		//#########################################################################
		class _const_rev_iterator; // forward declaration
		//! forward iterator for UString
		class CORE_API _rev_iterator : public _base_iterator { /* i don't know why the beautifier is freaking out on this line */
			friend class _const_rev_iterator;
		public:
			_rev_iterator();
			_rev_iterator(const _rev_iterator& i);

			//! pre-increment
			_rev_iterator& operator++();
			//! post-increment
			_rev_iterator operator++(int);

			//! pre-decrement
			_rev_iterator& operator--();
			//! post-decrement
			_rev_iterator operator--(int);

			//! addition operator
			_rev_iterator operator+(difference_type n);
			//! subtraction operator
			_rev_iterator operator-(difference_type n);

			//! addition assignment operator
			_rev_iterator& operator+=(difference_type n);
			//! subtraction assignment operator
			_rev_iterator& operator-=(difference_type n);

			//! dereference operator
			value_type& operator*() const;

			//! dereference at offset operator
			value_type& operator[](difference_type n) const;
		};
		//#########################################################################
		//! const reverse iterator for UString
		class CORE_API _const_rev_iterator : public _base_iterator { /* i don't know why the beautifier is freaking out on this line */
		public:
			_const_rev_iterator();
			_const_rev_iterator(const _const_rev_iterator& i);
			_const_rev_iterator(const _rev_iterator& i);
			//! pre-increment
			_const_rev_iterator& operator++();
			//! post-increment
			_const_rev_iterator operator++(int);

			//! pre-decrement
			_const_rev_iterator& operator--();
			//! post-decrement
			_const_rev_iterator operator--(int);

			//! addition operator
			_const_rev_iterator operator+(difference_type n);
			//! subtraction operator
			_const_rev_iterator operator-(difference_type n);

			//! addition assignment operator
			_const_rev_iterator& operator+=(difference_type n);
			//! subtraction assignment operator
			_const_rev_iterator& operator-=(difference_type n);

			//! dereference operator
			const value_type& operator*() const;

			//! dereference at offset operator
			const value_type& operator[](difference_type n) const;

			//! difference operator
			friend size_type operator-(const _const_rev_iterator& left, const _const_rev_iterator& right);
			//! equality operator
			friend bool operator==(const _const_rev_iterator& left, const _const_rev_iterator& right);
			//! inequality operator
			friend bool operator!=(const _const_rev_iterator& left, const _const_rev_iterator& right);
			//! less than
			friend bool operator<(const _const_rev_iterator& left, const _const_rev_iterator& right);
			//! less than or equal
			friend bool operator<=(const _const_rev_iterator& left, const _const_rev_iterator& right);
			//! greater than
			friend bool operator>(const _const_rev_iterator& left, const _const_rev_iterator& right);
			//! greater than or equal
			friend bool operator>=(const _const_rev_iterator& left, const _const_rev_iterator& right);
		};
		//#########################################################################

		typedef _fwd_iterator iterator;                     //!< iterator
		typedef _rev_iterator reverse_iterator;             //!< reverse iterator
		typedef _const_fwd_iterator const_iterator;         //!< const iterator
		typedef _const_rev_iterator const_reverse_iterator; //!< const reverse iterator


		//!\name Constructors/Destructor
		//@{
		//! default constructor, creates an empty string
		UString();
		//! copy constructor
		UString(const UString& copy);
		//! \a length copies of \a ch
		UString(size_type length, const code_point& ch);
		//! duplicate of nul-terminated sequence \a str
		UString(const code_point* str);
		//! duplicate of \a str, \a length code points long
		UString(const code_point* str, size_type length);
		//! substring of \a str starting at \a index and \a length code points long
		UString(const UString& str, size_type index, size_type length);
		//! duplicate of nul-terminated \c wchar_t array
		UString(const wchar_t* w_str);
		//! duplicate of \a w_str, \a length characters long
		UString(const wchar_t* w_str, size_type length);
		//! duplicate of \a wstr
		UString(const std::wstring& wstr);
		//! duplicate of nul-terminated C-string \a c_str (UTF-8 encoding)
		UString(const char* c_str);
		//! duplicate of \a c_str, \a length characters long (UTF-8 encoding)
		UString(const char* c_str, size_type length);
		//! duplicate of \a str (UTF-8 encoding)
		UString(const std::string& str);

		//! destructor
		~UString();
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name Utility functions
		//@{
		//! Returns the number of code points in the current string
		size_type size() const;
		//! Returns the number of code points in the current string
		size_type length() const;
		//! Returns the number of Unicode characters in the string
		/*! Executes in linear time. */
		size_type length_Characters() const;
		//! returns the maximum number of UTF-16 code points that the string can hold
		size_type max_size() const;
		//! sets the capacity of the string to at least \a size code points
		void reserve(size_type size);
		//! changes the size of the string to \a size, filling in any new area with \a val
		void resize(size_type num, const code_point& val = 0);
		//! exchanges the elements of the current string with those of \a from
		void swap(UString& from);
		//! returns \c true if the string has no elements, \c false otherwise
		bool empty() const;
		//! returns a pointer to the first character in the current string
		const code_point* c_str() const;
		//! returns a pointer to the first character in the current string
		const code_point* data() const;
		//! returns the number of elements that the string can hold before it will need to allocate more space
		size_type capacity() const;
		//! deletes all of the elements in the string
		void clear();
		//! returns a substring of the current string, starting at \a index, and \a num characters long.
		/*! If \a num is omitted, it will default to \c UString::npos, and the substr() function will simply return the remainder of the string starting at \a index. */
		UString substr(size_type index, size_type num = npos) const;
		//! appends \a val to the end of the string
		void push_back(unicode_char val);
		//! appends \a val to the end of the string
		void push_back(wchar_t val);
		//! appends \a val to the end of the string
		/*! This can be used to push surrogate pair code points, you'll just need to push them
		one after the other. */
		void push_back(code_point val);
		//! appends \a val to the end of the string
		/*! Limited to characters under the 127 value barrier. */
		void push_back(char val);
		//! returns \c true if the given Unicode character \a ch is in this string
		bool inString(unicode_char ch) const;
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name Stream variations
		//@{
		//! returns the current string in UTF-8 form within a std::string
		const std::string& asUTF8() const;
		//! returns the current string in UTF-8 form as a nul-terminated char array
		const char* asUTF8_c_str() const;
		//! returns the current string in UTF-32 form within a utf32string
		const utf32string& asUTF32() const;
		//! returns the current string in UTF-32 form as a nul-terminated unicode_char array
		const unicode_char* asUTF32_c_str() const;
		//! returns the current string in the native form of std::wstring
		const std::wstring& asWStr() const;
		//! returns the current string in the native form of a nul-terminated wchar_t array
		const wchar_t* asWStr_c_str() const;
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name Single Character Access
		//@{
		//! returns a reference to the element in the string at index \c loc
		code_point& at(size_type loc);
		//! returns a reference to the element in the string at index \c loc
		const code_point& at(size_type loc) const;
		//! returns the data point \a loc evaluated as a UTF-32 value
		/*! This function will will only properly decode surrogate pairs when \a loc points to the index
		of a lead code point that is followed by a trailing code point. Evaluating the trailing code point
		itself, or pointing to a code point that is a sentinel value (part of a broken pair) will return
		the value of just that code point (not a valid Unicode value, but useful as a sentinel value). */
		unicode_char getChar(size_type loc) const;
		//! sets the value of the character at \a loc to the Unicode value \a ch (UTF-32)
		/*! Providing sentinel values (values between U+D800-U+DFFF) are accepted, but you should be aware
		that you can also unwittingly create a valid surrogate pair if you don't pay attention to what you
		are doing. \note This operation may also lengthen the string if a surrogate pair is needed to
		represent the value given, but one is not available to replace; or alternatively shorten the string
		if an existing surrogate pair is replaced with a character that is representable without a surrogate
		pair. The return value will signify any lengthening or shortening performed, returning 0 if no change
		was made, -1 if the string was shortened, or 1 if the string was lengthened. Any single call can
		only change the string length by + or - 1. */
		int setChar(size_type loc, unicode_char ch);
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name iterator acquisition
		//@{
		//! returns an iterator to the first element of the string
		iterator begin();
		//! returns an iterator to the first element of the string
		const_iterator begin() const;
		//! returns an iterator just past the end of the string
		iterator end();
		//! returns an iterator just past the end of the string
		const_iterator end() const;
		//! returns a reverse iterator to the last element of the string
		reverse_iterator rbegin();
		//! returns a reverse iterator to the last element of the string
		const_reverse_iterator rbegin() const;
		//! returns a reverse iterator just past the beginning of the string
		reverse_iterator rend();
		//! returns a reverse iterator just past the beginning of the string
		const_reverse_iterator rend() const;
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name assign
		//@{
		//! gives the current string the values from \a start to \a end
		UString& assign(iterator start, iterator end);
		//! assign \a str to the current string
		UString& assign(const UString& str);
		//! assign the nul-terminated \a str to the current string
		UString& assign(const code_point* str);
		//! assign the first \a num characters of \a str to the current string
		UString& assign(const code_point* str, size_type num);
		//! assign \a len entries from \a str to the current string, starting at \a index
		UString& assign(const UString& str, size_type index, size_type len);
		//! assign \a num copies of \a ch to the current string
		UString& assign(size_type num, const code_point& ch);
		//! assign \a wstr to the current string (\a wstr is treated as a UTF-16 stream)
		UString& assign(const std::wstring& wstr);
		//! assign \a w_str to the current string
		UString& assign(const wchar_t* w_str);
		//! assign the first \a num characters of \a w_str to the current string
		UString& assign(const wchar_t* w_str, size_type num);
		//! assign \a str to the current string (\a str is treated as a UTF-8 stream)
		UString& assign(const std::string& str);
		//! assign \a c_str to the current string (\a c_str is treated as a UTF-8 stream)
		UString& assign(const char* c_str);
		//! assign the first \a num characters of \a c_str to the current string (\a c_str is treated as a UTF-8 stream)
		UString& assign(const char* c_str, size_type num);
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name append
		//@{
		//! appends \a str on to the end of the current string
		UString& append(const UString& str);
		//! appends \a str on to the end of the current string
		UString& append(const code_point* str);
		//! appends a substring of \a str starting at \a index that is \a len characters long on to the end of the current string
		UString& append(const UString& str, size_type index, size_type len);
		//! appends \a num characters of \a str on to the end of the current string
		UString& append(const code_point* str, size_type num);
		//! appends \a num repetitions of \a ch on to the end of the current string
		UString& append(size_type num, code_point ch);
		//! appends the sequence denoted by \a start and \a end on to the end of the current string
		UString& append(iterator start, iterator end);
		//! appends \a num characters of \a str on to the end of the current string
		UString& append(const wchar_t* w_str, size_type num);
		//! appends \a num repetitions of \a ch on to the end of the current string
		UString& append(size_type num, wchar_t ch);
		//! appends \a num characters of \a str on to the end of the current string  (UTF-8 encoding)
		UString& append(const char* c_str, size_type num);
		//! appends \a num repetitions of \a ch on to the end of the current string (Unicode values less than 128)
		UString& append(size_type num, char ch);
		//! appends \a num repetitions of \a ch on to the end of the current string (Full Unicode spectrum)
		UString& append(size_type num, unicode_char ch);
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name insert
		//@{
		//! inserts \a ch before the code point denoted by \a i
		iterator insert(iterator i, const code_point& ch);
		//! inserts \a str into the current string, at location \a index
		UString& insert(size_type index, const UString& str);
		//! inserts \a str into the current string, at location \a index
		UString& insert(size_type index, const code_point* str) {
			mData.insert(index, str);
			return *this;
		}
		//! inserts a substring of \a str (starting at \a index2 and \a num code points long) into the current string, at location \a index1
		UString& insert(size_type index1, const UString& str, size_type index2, size_type num);
		//! inserts the code points denoted by \a start and \a end into the current string, before the code point specified by \a i
		void insert(iterator i, iterator start, iterator end);
		//! inserts \a num code points of \a str into the current string, at location \a index
		UString& insert(size_type index, const code_point* str, size_type num);
		//! inserts \a num code points of \a str into the current string, at location \a index
		UString& insert(size_type index, const wchar_t* w_str, size_type num);
		//! inserts \a num code points of \a str into the current string, at location \a index
		UString& insert(size_type index, const char* c_str, size_type num);
		//! inserts \a num copies of \a ch into the current string, at location \a index
		UString& insert(size_type index, size_type num, code_point ch);
		//! inserts \a num copies of \a ch into the current string, at location \a index
		UString& insert(size_type index, size_type num, wchar_t ch);
		//! inserts \a num copies of \a ch into the current string, at location \a index
		UString& insert(size_type index, size_type num, char ch);
		//! inserts \a num copies of \a ch into the current string, at location \a index
		UString& insert(size_type index, size_type num, unicode_char ch);
		//! inserts \a num copies of \a ch into the current string, before the code point denoted by \a i
		void insert(iterator i, size_type num, const code_point& ch);
		//! inserts \a num copies of \a ch into the current string, before the code point denoted by \a i
		void insert(iterator i, size_type num, const wchar_t& ch);
		//! inserts \a num copies of \a ch into the current string, before the code point denoted by \a i
		void insert(iterator i, size_type num, const char& ch);
		//! inserts \a num copies of \a ch into the current string, before the code point denoted by \a i
		void insert(iterator i, size_type num, const unicode_char& ch);
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name erase
		//@{
		//! removes the code point pointed to by \a loc, returning an iterator to the next character
		iterator erase(iterator loc);
		//! removes the code points between \a start and \a end (including the one at \a start but not the one at \a end), returning an iterator to the code point after the last code point removed
		iterator erase(iterator start, iterator end);
		//! removes \a num code points from the current string, starting at \a index
		UString& erase(size_type index = 0, size_type num = npos);
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name replace
		//@{
		//! replaces up to \a num1 code points of the current string (starting at \a index1) with \a str
		UString& replace(size_type index1, size_type num1, const UString& str);
		//! replaces up to \a num1 code points of the current string (starting at \a index1) with up to \a num2 code points from \a str
		UString& replace(size_type index1, size_type num1, const UString& str, size_type num2);
		//! replaces up to \a num1 code points of the current string (starting at \a index1) with up to \a num2 code points from \a str beginning at \a index2
		UString& replace(size_type index1, size_type num1, const UString& str, size_type index2, size_type num2);
		//! replaces code points in the current string from \a start to \a end with \a num code points from \a str
		UString& replace(iterator start, iterator end, const UString& str, size_type num = npos);
		//! replaces up to \a num1 code points in the current string (beginning at \a index) with \c num2 copies of \c ch
		UString& replace(size_type index, size_type num1, size_type num2, code_point ch);
		//! replaces the code points in the current string from \a start to \a end with \a num copies of \a ch
		UString& replace(iterator start, iterator end, size_type num, code_point ch);
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name compare
		//@{
		//! compare \a str to the current string
		int compare(const UString& str) const;
		//! compare \a str to the current string
		int compare(const code_point* str) const;
		//! compare \a str to a substring of the current string, starting at \a index for \a length characters
		int compare(size_type index, size_type length, const UString& str) const;
		//! compare a substring of \a str to a substring of the current string, where \a index2 and \a length2 refer to \a str and \a index and \a length refer to the current string
		int compare(size_type index, size_type length, const UString& str, size_type index2, size_type length2) const;
		//! compare a substring of \a str to a substring of the current string, where the substring of \a str begins at zero and is \a length2 characters long, and the substring of the current string begins at \a index and is \a length  characters long
		int compare(size_type index, size_type length, const code_point* str, size_type length2) const;
		//! compare a substring of \a str to a substring of the current string, where the substring of \a str begins at zero and is \a length2 elements long, and the substring of the current string begins at \a index and is \a length characters long
		int compare(size_type index, size_type length, const wchar_t* w_str, size_type length2) const;
		//! compare a substring of \a str to a substring of the current string, where the substring of \a str begins at zero and is \a length2 <b>UTF-8 code points</b> long, and the substring of the current string begins at \a index and is \a length characters long
		int compare(size_type index, size_type length, const char* c_str, size_type length2) const;
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name find & rfind
		//@{
		//! returns the index of the first occurrence of \a str within the current string, starting at \a index; returns \c UString::npos if nothing is found
		/*! \a str is a UTF-16 encoded string, but through implicit casting can also be a UTF-8 encoded string (const char* or std::string) */
		size_type find(const UString& str, size_type index = 0) const;
		//! returns the index of the first occurrence of \a str within the current string and within \a length code points, starting at \a index; returns \c UString::npos if nothing is found
		/*! \a cp_str is a UTF-16 encoded string */
		size_type find(const code_point* cp_str, size_type index, size_type length) const;
		//! returns the index of the first occurrence of \a str within the current string and within \a length code points, starting at \a index; returns \c UString::npos if nothing is found
		/*! \a cp_str is a UTF-8 encoded string */
		size_type find(const char* c_str, size_type index, size_type length) const;
		//! returns the index of the first occurrence of \a str within the current string and within \a length code points, starting at \a index; returns \c UString::npos if nothing is found
		/*! \a cp_str is a UTF-16 encoded string */
		size_type find(const wchar_t* w_str, size_type index, size_type length) const;
		//! returns the index of the first occurrence \a ch within the current string, starting at \a index; returns \c UString::npos if nothing is found
		/*! \a ch is only capable of representing Unicode values up to U+007F (127) */
		size_type find(char ch, size_type index = 0) const;
		//! returns the index of the first occurrence \a ch within the current string, starting at \a index; returns \c UString::npos if nothing is found
		/*! \a ch is only capable of representing Unicode values up to U+FFFF (65535) */
		size_type find(code_point ch, size_type index = 0) const;
		//! returns the index of the first occurrence \a ch within the current string, starting at \a index; returns \c UString::npos if nothing is found
		/*! \a ch is only capable of representing Unicode values up to U+FFFF (65535) */
		size_type find(wchar_t ch, size_type index = 0) const;
		//! returns the index of the first occurrence \a ch within the current string, starting at \a index; returns \c UString::npos if nothing is found
		/*! \a ch can fully represent any Unicode character */
		size_type find(unicode_char ch, size_type index = 0) const;

		//! returns the location of the first occurrence of \a str in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type rfind(const UString& str, size_type index = 0) const;
		//! returns the location of the first occurrence of \a str in the current string, doing a reverse search from \a index, searching at most \a num characters; returns \c UString::npos if nothing is found
		size_type rfind(const code_point* cp_str, size_type index, size_type num) const;
		//! returns the location of the first occurrence of \a str in the current string, doing a reverse search from \a index, searching at most \a num characters; returns \c UString::npos if nothing is found
		size_type rfind(const char* c_str, size_type index, size_type num) const;
		//! returns the location of the first occurrence of \a str in the current string, doing a reverse search from \a index, searching at most \a num characters; returns \c UString::npos if nothing is found
		size_type rfind(const wchar_t* w_str, size_type index, size_type num) const;
		//! returns the location of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type rfind(char ch, size_type index = 0) const;
		//! returns the location of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type rfind(code_point ch, size_type index) const;
		//! returns the location of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type rfind(wchar_t ch, size_type index = 0) const;
		//! returns the location of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type rfind(unicode_char ch, size_type index = 0) const;
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name find_first/last_(not)_of
		//@{
		//! Returns the index of the first character within the current string that matches \b any character in \a str, beginning the search at \a index and searching at most \a num characters; returns \c UString::npos if nothing is found
		size_type find_first_of(const UString &str, size_type index = 0, size_type num = npos) const;
		//! returns the index of the first occurrence of \a ch in the current string, starting the search at \a index; returns \c UString::npos if nothing is found
		size_type find_first_of(code_point ch, size_type index = 0) const;
		//! returns the index of the first occurrence of \a ch in the current string, starting the search at \a index; returns \c UString::npos if nothing is found
		size_type find_first_of(char ch, size_type index = 0) const;
		//! returns the index of the first occurrence of \a ch in the current string, starting the search at \a index; returns \c UString::npos if nothing is found
		size_type find_first_of(wchar_t ch, size_type index = 0) const;
		//! returns the index of the first occurrence of \a ch in the current string, starting the search at \a index; returns \c UString::npos if nothing is found
		size_type find_first_of(unicode_char ch, size_type index = 0) const;

		//! returns the index of the first character within the current string that does not match any character in \a str, beginning the search at \a index and searching at most \a num characters; returns \c UString::npos if nothing is found
		size_type find_first_not_of(const UString& str, size_type index = 0, size_type num = npos) const;
		//! returns the index of the first character within the current string that does not match \a ch, starting the search at \a index; returns \c UString::npos if nothing is found
		size_type find_first_not_of(code_point ch, size_type index = 0) const;
		//! returns the index of the first character within the current string that does not match \a ch, starting the search at \a index; returns \c UString::npos if nothing is found
		size_type find_first_not_of(char ch, size_type index = 0) const;
		//! returns the index of the first character within the current string that does not match \a ch, starting the search at \a index; returns \c UString::npos if nothing is found
		size_type find_first_not_of(wchar_t ch, size_type index = 0) const;
		//! returns the index of the first character within the current string that does not match \a ch, starting the search at \a index; returns \c UString::npos if nothing is found
		size_type find_first_not_of(unicode_char ch, size_type index = 0) const;

		//! returns the index of the first character within the current string that matches any character in \a str, doing a reverse search from \a index and searching at most \a num characters; returns \c UString::npos if nothing is found
		size_type find_last_of(const UString& str, size_type index = npos, size_type num = npos) const;
		//! returns the index of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_of(code_point ch, size_type index = npos) const;
		//! returns the index of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_of(char ch, size_type index = npos) const {
			return find_last_of(static_cast<code_point>(ch), index);
		}
		//! returns the index of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_of(wchar_t ch, size_type index = npos) const;
		//! returns the index of the first occurrence of \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_of(unicode_char ch, size_type index = npos) const;

		//! returns the index of the last character within the current string that does not match any character in \a str, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_not_of(const UString& str, size_type index = npos, size_type num = npos) const;
		//! returns the index of the last occurrence of a character that does not match \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_not_of(code_point ch, size_type index = npos) const;
		//! returns the index of the last occurrence of a character that does not match \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_not_of(char ch, size_type index = npos) const;
		//! returns the index of the last occurrence of a character that does not match \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_not_of(wchar_t ch, size_type index = npos) const;
		//! returns the index of the last occurrence of a character that does not match \a ch in the current string, doing a reverse search from \a index; returns \c UString::npos if nothing is found
		size_type find_last_not_of(unicode_char ch, size_type index = npos) const;
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name Operators
		//@{
		//! less than operator
		bool operator<(const UString& right) const;
		//! less than or equal operator
		bool operator<=(const UString& right) const;
		//! greater than operator
		bool operator>(const UString& right) const;
		//! greater than or equal operator
		bool operator>=(const UString& right) const;
		//! equality operator
		bool operator==(const UString& right) const;
		//! inequality operator
		bool operator!=(const UString& right) const;
		//! assignment operator, implicitly casts all compatible types
		UString& operator=(const UString& s);
		//! assignment operator
		UString& operator=(code_point ch);
		//! assignment operator
		UString& operator=(char ch);
		//! assignment operator
		UString& operator=(wchar_t ch);
		//! assignment operator
		UString& operator=(unicode_char ch);
		//! code point dereference operator
		code_point& operator[](size_type index);
		//! code point dereference operator
		const code_point& operator[](size_type index) const;
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name Implicit Cast Operators
		//@{
		//! implicit cast to std::string
		operator std::string() const;
		//! implicit cast to std::wstring
		operator std::wstring() const;
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name UTF-16 character encoding/decoding
		//@{
		//! returns \c true if \a cp does not match the signature for the lead of follow code point of a surrogate pair in a UTF-16 sequence
		static bool _utf16_independent_char(code_point cp);
		//! returns \c true if \a cp matches the signature of a surrogate pair lead character
		static bool _utf16_surrogate_lead(code_point cp);
		//! returns \c true if \a cp matches the signature of a surrogate pair following character
		static bool _utf16_surrogate_follow(code_point cp);
		//! estimates the number of UTF-16 code points in the sequence starting with \a cp
		static size_t _utf16_char_length(code_point cp);
		//! returns the number of UTF-16 code points needed to represent the given UTF-32 character \a cp
		static size_t _utf16_char_length(unicode_char uc);
		//! converts the given UTF-16 character buffer \a in_cp to a single UTF-32 Unicode character \a out_uc, returns the number of code points used to create the output character (2 for surrogate pairs, otherwise 1)
		/*! This function does it's best to prevent error conditions, verifying complete
		surrogate pairs before applying the algorithm. In the event that half of a pair
		is found it will happily generate a value in the 0xD800 - 0xDFFF range, which is
		normally an invalid Unicode value but we preserve them for use as sentinel values. */
		static size_t _utf16_to_utf32(const code_point in_cp[2], unicode_char& out_uc);
		//! writes the given UTF-32 \a uc_in to the buffer location \a out_cp using UTF-16 encoding, returns the number of code points used to encode the input (always 1 or 2)
		/*! This function, like its counterpart, will happily create invalid UTF-16 surrogate pairs. These
		invalid entries will be created for any value of \c in_uc that falls in the range U+D800 - U+DFFF.
		These are generally useful as sentinel values to represent various program specific conditions.
		\note This function will also pass through any single UTF-16 code point without modification,
		making it a safe method of ensuring a stream that is unknown UTF-32 or UTF-16 is truly UTF-16.*/
		static size_t _utf32_to_utf16(const unicode_char& in_uc, code_point out_cp[2]);
		//@}

		//////////////////////////////////////////////////////////////////////////

		//!\name UTF-8 character encoding/decoding
		//@{
		//! returns \c true if \a cp is the beginning of a UTF-8 sequence
		static bool _utf8_start_char(unsigned char cp);
		//! estimates the number of UTF-8 code points in the sequence starting with \a cp
		static size_t _utf8_char_length(unsigned char cp);
		//! returns the number of UTF-8 code points needed to represent the given UTF-32 character \a cp
		static size_t _utf8_char_length(unicode_char uc);

		//! converts the given UTF-8 character buffer to a single UTF-32 Unicode character, returns the number of bytes used to create the output character (maximum of 6)
		static size_t _utf8_to_utf32(const unsigned char in_cp[6], unicode_char& out_uc);
		//! writes the given UTF-32 \a uc_in to the buffer location \a out_cp using UTF-8 encoding, returns the number of bytes used to encode the input
		static size_t _utf32_to_utf8(const unicode_char& in_uc, unsigned char out_cp[6]);

		//! verifies a UTF-8 stream, returning the total number of Unicode characters found
		static size_type _verifyUTF8(const unsigned char* c_str);
		//! verifies a UTF-8 stream, returning the total number of Unicode characters found
		static size_type _verifyUTF8(const std::string& str);
		//@}

	private:
		//template<class ITER_TYPE> friend class _iterator;
		dstring mData;

		//! buffer data type identifier
		enum BufferType {
			bt_none,
			bt_string,
			bt_wstring,
			bt_utf32string
		};

		//! common constructor operations
		void _init();

		///////////////////////////////////////////////////////////////////////
		// Scratch buffer
		//! auto cleans the scratch buffer using the proper delete for the stored type
		void _cleanBuffer() const;

		//! create a std::string in the scratch buffer area
		void _getBufferStr() const;
		//! create a std::wstring in the scratch buffer area
		void _getBufferWStr() const;
		//! create a utf32string in the scratch buffer area
		void _getBufferUTF32Str() const;

		void _load_buffer_UTF8() const;
		void _load_buffer_WStr() const;
		void _load_buffer_UTF32() const;

		mutable BufferType m_bufferType; // identifies the data type held in m_buffer
		mutable size_t m_bufferSize; // size of the CString buffer

		// multi-purpose buffer used everywhere we need a throw-away buffer
		union {
			mutable void* mVoidBuffer;
			mutable std::string* mStrBuffer;
			mutable std::wstring* mWStrBuffer;
			mutable utf32string* mUTF32StrBuffer;
		}
		m_buffer;
	};

	//! string addition operator \relates UString
	inline UString operator+(const UString& s1, const UString& s2) {
		return UString(s1).append(s2);
	}
	//! string addition operator \relates UString
	inline UString operator+(const UString& s1, UString::code_point c) {
		return UString(s1).append(1, c);
	}
	//! string addition operator \relates UString
	inline UString operator+(const UString& s1, UString::unicode_char c) {
		return UString(s1).append(1, c);
	}
	//! string addition operator \relates UString
	inline UString operator+(const UString& s1, char c) {
		return UString(s1).append(1, c);
	}
	//! string addition operator \relates UString
	inline UString operator+(const UString& s1, wchar_t c) {
		return UString(s1).append(1, c);
	}
	//! string addition operator \relates UString
	inline UString operator+(UString::code_point c, const UString& s2) {
		return UString().append(1, c).append(s2);
	}
	//! string addition operator \relates UString
	inline UString operator+(UString::unicode_char c, const UString& s2) {
		return UString().append(1, c).append(s2);
	}
	//! string addition operator \relates UString
	inline UString operator+(char c, const UString& s2) {
		return UString().append(1, c).append(s2);
	}
	//! string addition operator \relates UString
	inline UString operator+(wchar_t c, const UString& s2) {
		return UString().append(1, c).append(s2);
	}

	// (const) forward iterator common operators
	inline UString::size_type operator-(const UString::_const_fwd_iterator& left, const UString::_const_fwd_iterator& right) {
		return (left.mIter - right.mIter);
	}
	inline bool operator==(const UString::_const_fwd_iterator& left, const UString::_const_fwd_iterator& right) {
		return left.mIter == right.mIter;
	}
	inline bool operator!=(const UString::_const_fwd_iterator& left, const UString::_const_fwd_iterator& right) {
		return left.mIter != right.mIter;
	}
	inline bool operator<(const UString::_const_fwd_iterator& left, const UString::_const_fwd_iterator& right) {
		return left.mIter < right.mIter;
	}
	inline bool operator<=(const UString::_const_fwd_iterator& left, const UString::_const_fwd_iterator& right) {
		return left.mIter <= right.mIter;
	}
	inline bool operator>(const UString::_const_fwd_iterator& left, const UString::_const_fwd_iterator& right) {
		return left.mIter > right.mIter;
	}
	inline bool operator>=(const UString::_const_fwd_iterator& left, const UString::_const_fwd_iterator& right) {
		return left.mIter >= right.mIter;
	}

	// (const) reverse iterator common operators
	// NB: many of these operations are evaluated in reverse because this is a reverse iterator wrapping a forward iterator
	inline UString::size_type operator-(const UString::_const_rev_iterator& left, const UString::_const_rev_iterator& right) {
		return (right.mIter - left.mIter);
	}
	inline bool operator==(const UString::_const_rev_iterator& left, const UString::_const_rev_iterator& right) {
		return left.mIter == right.mIter;
	}
	inline bool operator!=(const UString::_const_rev_iterator& left, const UString::_const_rev_iterator& right) {
		return left.mIter != right.mIter;
	}
	inline bool operator<(const UString::_const_rev_iterator& left, const UString::_const_rev_iterator& right) {
		return right.mIter < left.mIter;
	}
	inline bool operator<=(const UString::_const_rev_iterator& left, const UString::_const_rev_iterator& right) {
		return right.mIter <= left.mIter;
	}
	inline bool operator>(const UString::_const_rev_iterator& left, const UString::_const_rev_iterator& right) {
		return right.mIter > left.mIter;
	}
	inline bool operator>=(const UString::_const_rev_iterator& left, const UString::_const_rev_iterator& right) {
		return right.mIter >= left.mIter;
	}

	//! std::ostream write operator \relates UString
	inline std::ostream& operator << (std::ostream& os, const UString& s) {
		return os << s.asUTF8();
	}

	//! std::wostream write operator \relates UString
	inline std::wostream& operator << (std::wostream& os, const UString& s) {
		return os << s.asWStr();
	}

} // namespace NGTech