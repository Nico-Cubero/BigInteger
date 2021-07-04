#ifndef __INTEGER_HPP__
#define __INTEGER_HPP__

#include <cstdint>
#include <vector>
#include <string>
#include <random>
#include <cassert>
#include <exception>
#include <stdexcept>

#define DEFAULT_MAX_N_BITS 1000	// <- Default max num of bits set for random generation
#define N_BITS_PER_WORD 31			// <- Number of usable bits per word

#define MIN(x,y) (x<y?x:y)			// <- Determines the minimum value between x and y
#define MAX(x,y) (x>y?x:y)			// <- Determines the maximum value between x and y

/**
 *	@brief Arbitrary-sized binary two's complement Big Integer
 *
 *	Integer binary value is pieced into 31 bits blocks and stored into 32 bits
 *	words. The most significant bit of a word is used for storing the integer
 *	sign, but only the sign bit from the most-significant word stores the actual
 *	integer sign while the sign bits from the rest words remain to 0.
 *
 *	[01]|[01]{31 bits} [- 0|[01]{31 bits} [+]]
 *
 *	Example:
 *	 37650129374_I: 0|0000000000000000000000000010001 - 0|1000100000111110110000111011110
 *	-37650129374_I: 1|1111111111111111111111111101110 - 0|0111011111000001001111000100010
 *
 */
class Integer {

	private:

		std::vector<int32_t> _words;	///<- 32 bits words storing the Integer binary value
		bool _modified;								///<- Integer has been modified since last Integer decimal value computation

		/** Stores the 10-base Integer representation when conversion to string is
		 * called in order to avoid repeated computations when the Integer value is
		 * not modified
		 */
		std::string _str_fmt_cache;

		static std::default_random_engine _random_engine; ///<- Random engine

		/**
		 * @brief Reading from stream states declaration
		 *
		 *	See operator >> implementation for more information
		 */
		enum Stream_read_state {SKIP, READ_MINUS, READ_DIGIT};

		/////	Private methods /////

		/**	@brief Used for removing redundant words having 0 or -1 value before
			*	the most significant word.
			* @throw std::bad_alloc on memory allocation error
			*/
		void fix();

		/**	@brief Splits current word set into two subsets stored into two new two
			*	Integers where first Integer is comprised by the part_size less
			*	significant words and the second one comprises the rest of words
			*
			*	@param n1 First returning Integer containing the "tam" less significant words.
			*	@param n2 First returning Integer containing the the rest of words.
			*	@param part_size number of words for the first subset.
			* @note Const method that does not modify current Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		void split(Integer & n1, Integer & n2, size_t part_size) const;

		/**	@brief Adds "n2" 32 bits integer to "n1" Integer by starting at the word
			* indexed by index. Resulting value is stored in the current Integer.
		 	*
			*	@param n1 First Integer operand
			*	@param n2 Second 32 bits integer operand.
			*	@param index: n1 word index where the addition starts.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			*
			*	@pre index must be a valid integer  0 <= index < n1._words.size()
		*/
		void add(const Integer & n1, int32_t n2, size_t index);

		/**	@brief Adds two Integers. If negative flag is set to true, the second
		  *	operand n2 is turned negative before the addition. Resulting Integer is
			*	stored into the current Integer.
			*
			*	@param n1 First Integer operand
			*	@param n2 Second Integer operand
			*	@param negative n2 must be converted to negative or not.
			*		Default value is false
			* @throw std::bad_alloc on memory allocation error
			*
			*	@note Current Integer can be passed to n1 or to n2.
			* @overload.
		*/
		void add(const Integer & n1, const Integer & n2, bool negative=false);

		/**	@brief Multiplies two Integers and save the result into the current
		 	*		Integer.
			* @throw std::bad_alloc on memory allocation error
			*
			*	@param n1 First Integer operand
			*	@param n2 Second Integer operand
		*/
		void multiply(const Integer & n1, const Integer & n2);

		/**	@brief Performs integer division between the current Integer and the
			*	divisor d and returns both quotient and remainder into the
			*	passed passed Integer objects.
			*
			*	@param d The Integer divisor of the operation
			*	@param quotient Output Integer quotient
			*	@param remainder Output Integer remainder
			* @throw zero_division_error when division divisor d is zero
			* @throw std::bad_alloc on memory allocation error
			*
			*	@note Const method that does not modify the current Integer
			* @pre d must not be 0
			*
		*/
		void divide(const Integer & d, Integer & quotient, Integer & remainder) const;

		/** @brief Computes the power of current integer to the exponent and stores
			* 	the result into the current Integer.
			*	@param exp Integer with exponent
		  * @pre exp must be positive
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when exp is negative.
		*/
		void power(const Integer & exp);

		/**
			* @brief Converts the current Integer into negative
			* @throw std::bad_alloc on memory allocation error
		*/
		inline void negative() {this->bitwise_not();	this->add(*this,1,0); this->fix();};

		/**	@brief Compares current Integer value to another Integer value.
			*
			*	@param n Integer to be compared to the current Integer.
			*
			*	@return integer
			*	·	0: Both Integer values are equal.
			*	· 1: Current Integer value is greater than the passed one.
			*	· -1: Current Integer value is less than the passed one.
			*
			*	@note Const method that does not modify the current Integer
		*/
		int compare(const Integer & n) const;

		//	Bitwise operations

		/**
			* @brief Apply bitwise NOT operation on the current Integer
			* @throw std::bad_alloc on memory allocation error
		*/
		void bitwise_not();

		/**	@brief Performs bitwise AND operation between current Integer and the
			*	passed Integer (second operand).
			* @throw std::bad_alloc on memory allocation error
			*
			*	@param n Integer second operand
		*/
		void bitwise_and(const Integer & n);

		/**	@brief Performs bitwise AND operation between current Integer and the
			*	passed Integer (second operand).
			* @throw std::bad_alloc on memory allocation error
			*
			*	@param n Integer second operand
		*/
		void bitwise_or(const Integer & n);

		/**	@brief Performs arithmetic bitwise left shift on d bits
			* @throw std::bad_alloc on memory allocation error
			*	@param d Number of bits to shift
		*/
		void bitwise_shift_left(size_t d);

		/**	@brief Performs arithmetic bitwise right shift on d bits
			* @throw std::bad_alloc on memory allocation error
			*
			*	@param d Number of bits to shift
		*/
		void bitwise_shift_right(size_t d);

		///	Utils

		/**	@brief Resizes the number of words on current Integer.
			*
			*	@param n New number of words
			* @throw std::bad_alloc on memory allocation error
		*/
		void resize(size_t n);

		/**
			*	@brief Resets Integer value to the initial value 0.
			* @throw std::bad_alloc on memory allocation error
		*/
		void reset() {_words.resize(1);	_words.back() = 0;	_modified = true;};

		/**	@brief Finds most significant 1 bit index on the current Integer.
			*	@return Most significant 1 bit index.
			*
			*	@note Const method that does not modify the current Integer
		*/
		size_t findFirstOne() const;

		/**	@brief Converts decimal integer representation written in a string into
			*	an Integer value and stores into the current Integer.
			*
			*	@param n String containing the integer decimal representation.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		void string_to_integer(const std::string & n);

		/**	@brief Computes the decimal representation of current Integer and saves
			*	it into the passed string.
			*
			*	@param	n Output String where the computed Integer decimal
			*		representation will be written to.
			*
			* @note Const method that does not modify the current Integer.
		*/
		void integer_to_string(std::string & s) const;

	public:

		////  Exceptions defined ////

		/**
			* Exception class thrown on a zero division.
		*/
		class zero_division_error : public std::exception {
				const char * what() const throw () {
					return "Division by zero";
				}
		};

		/**
			* Exception class thrown on trying to store current Integer value into
			*	a long long integer with less capacity than Integer value.
			*
		*/
		class integer_conversion_error : public std::exception {
				const char * what() const throw () {
					return "Cannot convert to long long integer";
				}
		};

		////	Constructors ////

		/**	@brief Default constructor
			*	Constructs Integer to the default value 0.
		 	*	@overload
		*/
		inline Integer(): _modified(true) {_words.resize(1);};

		/**	@brief Integral Constructor
			*	Constructs an Integer from any integral type.
			* @throw std::bad_alloc on memory allocation error.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
		 	*	@overload
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
				Integer(INT_T n) {

						int32_t aux;

						//integer is splitted into 32 bits	integers if is longer.
						while(n!=0 && (!std::is_signed<INT_T>::value || n!=-1)) {

							if (n >= INT32_MIN && n <= INT32_MAX)
								//last 32 bit integer is readen
								aux = n & (int32_t)0xffffffff;
							else
								//Not last 32 bit integer is readen without the sig bit (31 bits)
								aux = n & (int32_t)0x7fffffff;

							//Store the readen word
							this->resize(this->_words.size() + (size_t)1);
							this->_words.back() = aux;

							//Shift to retrieve next word
							n >>= 31;
						}

						//integer initially stores 0 or -1 value and it is directly inserted.
						if (_words.empty()) {
							this->resize((size_t)1);
							this->_words.back() = n;
						}

					this->_modified=true;
				}

		/**	@brief String constructor
			*	Constructs an Integer from a string containing the decimal representation
			*	of the integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		 	*	@overload
		*/
		inline Integer(const std::string & n) {string_to_integer(n); _modified = true;};

		/**	@brief C-string constructor
			*	Constructs an Integer from a c-string (char array ended with \0)
			*	containing the decimal representation of the integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		 	*	@overload
		*/
		inline Integer(const char * n): Integer(std::string(n)) {};

		/**	@brief Copy constructor
			*	Copies constructor which constucts an Integer from another Integer
			* @throw std::bad_alloc on memory allocation error
		 	*	@overload
		*/
		Integer(const Integer & n) {
			this->_words = n._words;
			this->_str_fmt_cache = n._str_fmt_cache;
			this->_modified = n._modified;

			//Check postcondition
			#ifdef DEBUG
				assert((*this).compare(n) == 0);
			#endif
		}

		//// Observers ////

		/**	@brief Checks whether the Integer has a negative value or not.
			*	@note Const method that does not modify the current Integer.
		*/
		inline bool is_negative() const {return _words.back() < 0;};

		/**	@brief Checks whether the Integer value is zero or not.
			*	@note Const method that does not modify the current Integer.
		*/
		inline bool is_zero() const {return _words.back() == 0;};

		/**	@brief Checks whether the Integer value is even or not.
			*	@note Const method that does not modify the current Integer
		*/
		inline bool is_even() const {return (_words[0] & 0x00000001) == 0;};

		/** @brief Counts the actual number of bits taken by the Integer value.
			*	@note Const method that does not modify the current Integer
		*/
		inline size_t n_bits() const {return this->findFirstOne() + 1;};

		/**
			* @brief Gets current random engine used for Integer random value generation.
			*	@note The random engine is generic for the whole class
		*/
		inline static const std::default_random_engine & get_random_engine() {
			return Integer::_random_engine;
		}

		//// Helper functions /////

		/** @brief Replaces current Integer value by another pseudo-randomly
			* generated value. Generated value will take n_bits_max bits
			*	as max.
			*
			* @param n_bits_max Max number of bits to generate
			* @throw std::bad_alloc on memory allocation error
		*/
		void set_random_value(size_t n_bits_max = DEFAULT_MAX_N_BITS);

		//// Operators ////

		/**	@brief Integer assignment operator.
			*	Copies Integer second operand value to the current one.
			*
			*	@param n Integer whose value will be copied to the current one.
			* @overload
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator=(const Integer & n) {
			this->_words = n._words;
			this->_str_fmt_cache = n._str_fmt_cache;
			this->_modified = n._modified;

			//Check postcondition
			#ifdef DEBUG
				assert((*this).compare(n) == 0);
			#endif

			return *this;
		}

		/**	@brief integral type integer assignment operator.
			*	Copies integer value from any integral type to the current Integer.
			*
			*	@param n integral data type value to be copied to the current one.
			*	@return Current Integer after the operation.
			*	@overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		const Integer & operator=(INT_T n) {

			this->_words.clear();

			/*whether integer has more bits than int32_t, it must be splitted into
			 *32-bits blocks*/
				int32_t aux;

				//Split integer into 32-bits blocks until redundant block (0 or -1) is found
				while(n!=0 && (!std::is_signed<INT_T>::value || n!=-1)) {

					/*Check if last block will be extracted so it must be extracted with
					 *the sign bit*/
					if ((n>=0 && n<=INT32_MAX) || (n<0 && n>=INT32_MIN))
						aux= n & (int32_t)-1;
					else
						aux= n & (int32_t)0x7fffffff;

					this->resize(this->_words.size() + (size_t)1);
					this->_words.back() = aux;

					n >>= 31;	//Shift right to take the next block
				}

				//Add the integer if originally was 0 or -1 (considered by the loop redundant)
				if (_words.empty()) {
					this->resize((size_t)1);
					this->_words.back() = n;
				}

			_modified=true;

			//Check postcondition
			#ifdef DEBUG

				assert(!(this->_words.size() > 1) ||
										(this->_words.back() != 0 && this->_words.back() != -1));

				for (size_t i = 0; i < this->_words.size()-1; i++)
					assert((this->_words[i] & 0x80000000) == 0);

			#endif

			return *this;
		}

		/**	@brief String assignment operator
			*	Copies integer written as decimal into a string into the current Integer
			*	by converting into two's complement binary.
			*
			*	@param n String containing decimal Integer representation.
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator=(const std::string & n) {
			string_to_integer(n);
			_modified = true;

			return *this;
		}

		/**	@brief C-String assignment operator
			*	Copies integer written as decimal into a c-string (char array ended with
			*	\0) into the current Integer by converting into two's complement binary.
			*
			*	@param n C-string containing decimal Integer representation.
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator=(const char * n) {
			return (*this) = std::string(n);
		}

		/**	@brief Integer add operator
			*	Adds a passed Integer value to the current Integer and returns the result.
			*
			*	@param n	Integer second operand to be added to the current Integer
			*
			*	@return Integer containing the value of the addition result.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator+(const Integer & n) const {
			Integer r;
			r.add(*this, n);
			r._modified = true;

			return r;
		}

		/**	@brief Integral type integer add operator
			*	Adds a integral type integer to the current Integer and returns the
			*	resulting Integer
			*
			*	@param	n	integral type integer to be added to the current Integer
			*
			*	@return Integer containing the value of the addition result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline Integer operator+(INT_T n) const {
			Integer ret, opn(n);

			ret.add(*this, opn);
			ret._modified = true;

			return ret;
		}

		/**	@brief Integral type add operator.
			*	Adds integral type integer value to an Integer value and returns
			*	Integer containing the result of the operation
			*
			*	@param n	integral type as first operand
			* @param i Integer object as second operand
			*
			*	@return Integer containing the value of the addition result.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline Integer operator+(INT_T n, const Integer & i) {
			Integer ret, opn(n);

			ret.add(opn, i);
			ret._modified = true;

			return ret;
		}

		/**	@brief String add operator.
			* Adds decimal integer writen on a string to the current Integer.
			*	Result is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal representation of the integer
			*				to be added to the current Integer.
			*
			*	@return Integer containing the value of the addition result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator+(const std::string & n) const {
			Integer ret, opn(n);

			ret.add(*this, opn);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string add operator.
			* Adds current Integer value to a decimal integer writen on a C-string
			* (char array ended with |0). Result is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal integer to be added to the
			*				current Integer
			*
			*	@return Integer containing the value of the addition result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator+(const char * n) const {
			return (*this) + std::string(n);
		}

		/**	@brief String add operator.
			* Adds a Integer value to a decimal integer writen on a string.
			*	Result is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal integer to be the first operand
			*					of the addition.
			* @param i Integer containing the second operand.
			*
			*	@return Integer containing the value of the addition result.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator+(const std::string & n, const Integer & i) {
			Integer ret, opn(n);

			ret.add(opn, i);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string add operator.
			*	Adds a Integer value to an integer value on a string and returns the
		  *  result on a Integer object
			*
			*	-	n: char string containing first operand
			* - i: Integer second operand
			*
			*	@return Integer containing the resulting value of the addition.
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator+(const char * n, const Integer & i) {
			return std::string(n) + i;
		}

		/**	@brief Integer add assignment operator
			*	Adds Integer object value to the current Integer and saves the result
			*	into the current Integer. Current Integer is returned after the addition.
			*
			*	@param	n	Integer to be added to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator+=(const Integer & n) {
			this->add(*this, n);
			this->_modified = true;
			return *this;
		}

		/**	@brief Integral type add assignment operator
			*	Adds integral type integer value to the current Integer and saves the
			*	result into the current Integer. Current Integer is returned after the
			*	addition.
			*
			*	@param	n	integral type integer to be added to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline const Integer & operator+=(INT_T n) {
			Integer opn(n);

			this->add(*this, opn);
			this->_modified = true;
			return *this;
		}

		/**	@brief String add assignment operator
			*	Adds decimal integer value written on a string to the current Integer
			*	and saves the	result into the current Integer. Current Integer is
			*	returned after the addition.
			*
			*	@param	n	string containing the decimal representation of the integer
			*	to be added to the current Integer.
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator+=(const std::string & n) {
			Integer opn(n);

			this->add(*this, opn);
			this->_modified = true;
			return *this;
		}
		/**	@brief C-string add assignment operator
			*	Adds decimal integer value written on a C-string (char array ended
			*	with \0) to the current Integer and saves the	result into the current
			*	Integer. Current Integer is returned after the addition.
			*
			*	@param	n	C-string containing the decimal representation of the integer
			*	to be added to the current Integer.
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator+=(const char * n) {
			return (*this) += std::string(n);
		}

		// Unary operators

		/**	@brief Prefix increment operator
			*	Increments current Integer value
			*	in 1 unit. Current Integer is returned after the operation.
			*	@return Resulting current Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator++() {
			this->add(*this,1,0);
			this->fix();
			this->_modified = true;

			return *this;
		}

		/**	@brief Posfix increment operator
			*	Increments current Integer value
			*	in 1 unit. Current Integer before the operation is returned.
			*	@return Resulting current Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator++(int) {
			Integer r(*this);

			this->add(*this,1,0);
			this->fix();
			this->_modified = true;

			r._modified = true;

			return r;
		}

		/**	@brief Prefix decrement operator
			*	Decrements current Integer value
			*	in 1 unit. Current Integer is returned after the operation.
			*	@return Resulting current Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator--() {
			this->add(*this,-1,0);
			this->fix();
			this->_modified = true;

			return *this;
		}

		/**	@brief Posfix decrement operator
			*	Decrements current Integer value
			*	in 1 unit. Current Integer is returned before the operation.
			*	@return Resulting current Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator--(int) {
			Integer r(*this);

			this->add(*this,-1,0);
			this->fix();
			this->_modified = true;

			r._modified = true;

			return r;
		}

		/** @brief Unnary positive operator.
		 *	Computes current integer value.
		 *  @note Const method that does not modify the current Integer.
		 *  @return Resulting current Integer
		 * 	@throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator+() const {return *this;}

		/** @brief Unnary negative operator.
		 *	Returns Integer object containing negative value of the current Integer.
		 *  @note Const method that does not modify the current Integer.
		 *  @return Resulting current Integer
		 * @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator-() const {Integer r(*this); r.negative(); return r;}

		// Arithmetic operators

		/**	@brief Integer deduct operator
			*	Substracts passed Integer value to the current Integer. Resulting Integer
			*	is returned.
			*
			*	@param n	Integer second operand to be subtracted to the current Integer
			*
			*	@return Integer containing the value of the subtraction result.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator-(const Integer & n) const {
			Integer r;
			r.add(*this,n,true);
			r._modified = true;

			return r;
		}

		/**	@brief Integral type integer deduct operator
			*	Substract integral type integer value to the current Integer and returns
			*	the resulting Integer into a new Integer object.
			*
			*	@param	n	integral type integer to be subtracted to the current Integer
			*
			*	@return Integer containing the value of the substraction result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline Integer operator-(INT_T n) const {
			Integer ret, opn(n);
			ret.add(*this, opn, true);
			ret._modified = true;

			return ret;
		}

		/**	@brief Integral type deduct operator.
			*	Substract passed Integer value to an integral type integer and returns
			*	Integer containing the result of the operation.
			*
			*	@param n	integral type integer as first operand
			* @param i Integer object integer as second operand
			*
			*	@return Integer containing the value of the substraction result.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			* @warning friend operator
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline Integer operator-(INT_T n, const Integer & i) {
			Integer ret, opn(n);
			ret.add(opn, i, true);
			ret._modified = true;

			return ret;
		};

		/**	@brief String deduct operator.
			* Substracts a decimal integer value written on a string to the current
			*	Integer. Result is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal integer to be subtracted to the
			*				current Integer
			*
			*	@return Integer containing the value of the substraction result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator-(const std::string & n) const {
			Integer ret, opn(n);
			ret.add(*this, opn, true);
			ret._modified = true;

			return ret;
		};

		/**	@brief C-string deduct operator.
			* Substracts a decimal integer value written on a C-string (char array
			*	ended with \0) to the current Integer. Result is returned in a new
			*	Integer object.
			*
			*	@param	n	C-string containing the decimal integer to be subtracted to
			*				the current Integer
			*
			*	@return Integer containing the value of the addition result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator-(const char * n) const {
			return (*this) - std::string(n);
		}

		/**	@brief String deduct operator.
			* Substracts Integer value to a decimal integer writen on a string.
			*	Result is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal representation of the integer
			*				to be the first operand.
			* @param i Integer containing the second operand to be subtracted to first
			*				operand.
			*
			*	@return Integer containing the value of the substraction result.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator-(const std::string & n, const Integer & i) {
			Integer ret, opn(n);
			ret.add(opn, i, true);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string deduct operator.
			* Substracts Integer value to a decimal integer writen on a C-string (char
			*	array ended with \0). Result is returned in a new Integer object.
			*
			*	@param	n	C-string containing the decimal integer to be the first operand.
			* @param i Integer containing the second operand
			*
			*	@return Integer containing the value of the substraction result.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator-(const char * n, const Integer & i) {
			return std::string(n) - i;
		}


		/**	@brief Integer deduct assignment operator
			*	Substracts Integer object value to the current Integer and saves the result
			*	into the current Integer. Current Integer is returned after the addition.
			*
			*	@param	n	Integer to be subtracted to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator-=(const Integer & n) {
			this->add(*this,n,true);
			this->_modified = true;

			return *this;
		}

		/**	@brief Integral type integer deduct assignment operator
			*	Substracts integral type integer value to the current Integer and saves the
			*	result into the current Integer. Current Integer is returned after the
			*	addition.
			*
			*	@param	n	integral type integer to be subtracted to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline const Integer & operator-=(INT_T n) {
			Integer opn(n);

			this->add(*this, opn, true);
			this->_modified = true;

			return *this;
		}

		/**	@brief String deduct assignment operator
			*	Substracts decimal integer value written on a string to the current
			*	Integer and saves the	result into the current Integer. Current Integer
			*	is returned after the addition.
			*
			*	@param	n	string containing the decimal representation of the integer
			*	to be substracted to the current Integer.
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator-=(const std::string & n) {
			Integer opn(n);

			this->add(*this, opn, true);
			this->_modified = true;

			return *this;
		}

		/**	@brief C-string subtract assignment operator
			*	Subtracts decimal integer value written on a C-string (char array ended
			*	with \0) to the current Integer and saves the	result into the current
			*	Integer. Current Integer is returned after the addition.
			*
			*	@param	n	C-string containing the decimal representation of the integer
			*	to be substracted to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator-=(const char * n) {
			return (*this) -= std::string(n);
		}

		/**	@brief Integer multiply operator
			*	Multiplies a passed Integer value to the current Integer. Resulting Integer
			*	is returned.
			*
			*	@param n	Integer second operand to be multiplied to the current Integer
			*
			*	@return Integer containing the value of the multiplication result.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator*(const Integer & n) const {
			Integer r;
			r.multiply(*this,n);
			r._modified = true;

			return r;
		}

		/**	@brief Integral type integer multiply operator
			*	Multiplies an integral type integer value to the current Integer and returns
			*	the resulting Integer into a new Integer object.
			*
			*	@param	n	integral type integer to be multiplied to the current Integer
			*
			*	@return Integer containing the value of the multiplication result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline Integer operator*(INT_T n) const {
			Integer ret, opn(n);

			ret.multiply(*this, opn);
			ret._modified = true;

			return ret;
		}

		/**	@brief Integral type multiply operator.
			*	Multiplies the passed Integer value to an integral type integer and
			*	returns Integer containing the result of the operation.
			*
			*	@param n integral type as first operand
			* @param i Integer object as second operand
			*
			*	@return Integer containing the value of the multiplication result.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline Integer operator*(INT_T n, const Integer & i) {
			Integer ret, opn(n);
			ret.multiply(opn, i);
			ret._modified = true;

			return ret;
		};

		/**	@brief String multiply operator.
			* Multiplies a decimal integer value written on a string to the current
			*	Integer. Result is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal integer to be multiplied to the
			*				current Integer
			*
			*	@return Integer containing the value of the multiplication result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator*(const std::string & n) const {
			Integer ret, opn(n);

			ret.multiply(*this, opn);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string Multiply operator.
			* Multiplies a decimal integer value written on a C-string (char array
			*	ended with \0) to the current Integer. Result is returned in a new
			*	Integer object.
			*
			*	@param	n	C-string containing the decimal integer to be multiplied to
			*				the current Integer
			*
			*	@return Integer containing the value of the multiplication result.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator*(const char * n) const {
			return (*this) * std::string(n);
		}

		/**	@brief String multiply operator.
			* Multiplies Integer value to a decimal integer writen on a string.
			*	Result is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal integer to be the first operand.
			* @param i Integer containing the second operand
			*
			*	@return Integer containing the value of the multiplication result.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator*(const std::string & n, const Integer & i) {
			Integer ret, opn(n);
			ret.multiply(opn, i);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string multiply operator.
			* Multiplies Integer value to a decimal integer writen on a C-string (char
			*	array ended with \0). Result is returned in a new Integer object.
			*
			*	@param	n	C-string containing the decimal integer to be the first operand.
			* @param i Integer containing the second operand.
			*
			*	@return Integer containing the value of the multiplication result.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator*(const char * n, const Integer & i) {
			return std::string(n) * i;
		}

		/**	@brief Integer multiply assignment operator
			*	Multiplies Integer object value to the current Integer and saves the result
			*	into the current Integer. Current Integer is returned after the
			*	multiplication.
			*
			*	@param	n	Integer to be multiplied to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator*=(const Integer & n) {
			this->multiply(*this,n);
			this->_modified = true;

			return *this;
		}

		/**	@brief Integral type integer multiply assignment operator
			*	Multiplies integral type integer value to the current Integer and saves the
			*	result into the current Integer. Current Integer is returned after the
			*	multiplication.
			*
			*	@param	n	integral type integer to be multiplied to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline const Integer & operator*=(INT_T n) {
			Integer opn(n);

			this->multiply(*this, opn);
			this->_modified = true;

			return *this;
		}

		/**	@brief String multiply assignment operator
			*	Multiplies decimal integer value written on a string to the current
			*	Integer and saves the	result into the current Integer. Current Integer
			*	is returned after the multiplication.
			*
			*	@param	n	string containing the decimal representation of the integer
			*	to be multiplied to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator*=(const std::string & n) {
			Integer opn(n);

			this->multiply(*this, opn);
			this->_modified = true;

			return *this;
		}

		/**	@brief C-string multiply assignment operator
			*	Multiplies decimal integer value written on a C-string (char array ended
			*	with \0) to the current Integer and saves the	result into the current
			*	Integer. Current Integer is returned after the multiplication.
			*
			*	@param	n	C-string containing the decimal representation of the integer
			*	to be multiplied to the current Integer
			*
			*	@return Current Integer after the operation.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator*=(const char * n) {
			return (*this) *= std::string(n);
		}

		/**	@brief Integer Divide operator
			*	Divides the passed Integer value to the current Integer. Integer
			*	containing quotient is returned.
			*
			*	@param n	Integer divisor
			*
			*	@return Integer containing the division quotient.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		inline Integer operator/(const Integer & d) const {
			Integer q,r;
			this->divide(d,q,r);
			q._modified = true;

			return q;
		}

		/**	@brief integral type divide operator
			*	Divides an integral type integer value to the current Integer and
			*	returns the Integer containing quotient.
			*
			*	@param	n	integral type integer divisor
			*
			*	@return Integer containing the division quotient.
			*	@note Const method that does not modify the current Integer.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline Integer operator/(INT_T d) const {
			Integer div(d), q, r;

			this->divide(div, q, r);
			q._modified = true;

			return q;
		}

		/**	@brief Integral type divide operator.
			*	Divides the passed Integer value to an integral type integer and
			*	returns Integer containing the quotient.
			*
			*	@param n integral type as dividend
			* @param i Integer object as divisor
			*
			*	@return Integer containing the division quotient.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			* @warning friend operator
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline Integer operator/(INT_T n, const Integer & i) {
			Integer opn(n), q, r;

			opn.divide(i, q, r);
			q._modified = true;

			return q;
		};

		/**	@brief String division operator.
			* Divides a decimal integer value written on a string to the current
			*	Integer. Quotient is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal representation of the
			*	integer divisor
			*
			*	@return Integer containing the division quotient.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator/(const std::string & d) const {
			Integer div(d), q, r;

			this->divide(div, q, r);
			q._modified = true;

			return q;
		}

		/**	@brief C-string divide operator.
			* Divides a decimal integer value written on a C-string (char array
			*	ended with \0) to the current Integer. Quotient is returned in a new
			*	Integer object.
			*
			*	@param	n	C-string containing the decimal representation of the
			*	integer divisor
			*
			*	@return Integer containing the quotient.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator/(const char * d) const {
			return (*this) / std::string(d);
		}

		/**	@brief String division operator.
			* Divides Integer value to a decimal integer writen on a string.
			*	Quotient is returned in a new Integer object.
			*
			*	@param	n	string containing the decimal integer to be the first operand.
			* @param i Integer containing the second operand to be multiplied to first
			*				operand.
			*
			*	@return Integer containing the quotient.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator/(const std::string & n, const Integer & i) {
			Integer opn(n), q, r;

			opn.divide(i, q, r);
			q._modified = true;

			return q;
		}

		/**	@brief C-string divide operator.
			* Divides Integer value to a decimal integer writen on a C-string (char
			*	array ended with \0). Quotient is returned in a new Integer object.
			*
			*	@param	n	C-string containing the decimal integer to be the dividend.
			* @param i Integer containing the second operand to be divisor
			*
			*	@return Integer containing the quotient.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator/(const char * n, const Integer & i) {
			return std::string(n) / i;
		}

		/**	@brief Integer divide assignment operator
			*	Divides Integer object value to the current Integer and saves the
			*	quotient into the current Integer. Current Integer is returned after the
			*	division.
			*
			*	@param	n	Integer to be divisor
			*
			*	@return Current Integer after the operation and division quotient.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		inline const Integer & operator/=(const Integer & d) {
			Integer r;

			this->divide(d,*this,r);
			this->_modified = true;

			return *this;
		}

		/**	@brief Integral type integer divide assignment operator
			*	Divides current Integer to the integral type integer value and saves the
			*	quotient into the current Integer. Current Integer is returned after the
			*	division.
			*
			*	@param	n	integral type integer to be divisor
			*
			*	@return Current Integer after the operation and division quotient.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline const Integer & operator/=(INT_T d) {
			Integer ret, div(d);

			this->divide(div, *this, ret);
			this->_modified = true;

			return *this;
		}

		/**	@brief String divide assignment operator
			*	Divides current Integer to the a decimal integer value written on a string
			*	and saves the	quotient into the current Integer. Current Integer
			*	is returned after the division.
			*
			*	@param	n	string containing division divisor.
			*
			*	@return Current Integer after the operation and division quotient.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator/=(const std::string & d) {
			Integer ret, div(d);

			this->divide(div, *this, ret);
			this->_modified = true;

			return *this;
		}

		/**	@brief C-string divide assignment operator
			*	Divides current Integer to the a decimal integer value written on a C-string
			*	(char array ended with |0) and saves the	quotient into the current
			*	Integer. Current Integer is returned after the division.
			*
			*	@param	n	C-string containing division divisor.
			*
			*	@return Current Integer after the operation and division quotient.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator/=(const char * d) {
			return (*this) /= std::string(d);
		}

		/**	@brief Integer Module operator
			*	Divides the passed Integer value to the current Integer. Integer
			*	containing module is returned.
			*
			*	@param n	Integer divisor
			*
			*	@return Integer containing the division module.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		inline Integer operator%(const Integer & d) const {
			Integer q,r;

			this->divide(d,q,r);
			r._modified = true;

			return r;
		}

		/**	@brief integral type module operator
			*	Divides an integral type integer value to the current Integer and returns
			*	Integer containing the division module.
			*
			*	@param	n	integral type integer divisor
			*
			*	@return Integer containing the division module.
			*	@note Const method that does not modify the current Integer.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline Integer operator%(INT_T d) const {
			Integer div(d), q, r;

			this->divide(div, q, r);
			r._modified = true;

			return r;
		}

		/**	@brief Integral type integer module operator.
			*	Divides the passed Integer value to an integral type integer and
			*	returns Integer containing the division module.
			*
			*	@param n integral type integer as dividend
			* @param i Integer object as divisor
			*
			*	@return Integer containing the module.
			*	@note Const method that does not modify the current Integer.
			*	@overload
			* @warning friend operator
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline Integer operator%(INT_T n, const Integer & i) {
			Integer opn(n), q, r;

			opn.divide(i, q, r);
			r._modified = true;

			return r;
		}

		/**	@brief String module operator.
			* Divides a decimal integer value written on a string to the current
			*	Integer. Division module is returned.
			*
			*	@param	n	string containing the decimal representation of the integer divisor
			*
			*	@return Integer containing the module.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator%(const std::string & d) const {
			Integer div(d), q, r;

			this->divide(div, q, r);
			r._modified = true;

			return r;
		}

		/**	@brief C-string module operator.
			* Divides a decimal integer value written on a C-string (char array
			*	ended with \0) to the current Integer. Division module is returned.
			*
			*	@param	n	C-string containing the decimal representation of the integer
			*		divisor
			*
			*	@return Integer containing the module.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator%(const char * d) const {
			return (*this) % std::string(d);
		}

		/**	@brief String division operator.
			* Divides Integer value to a decimal integer writen on a string.
			*	Division module is returned.
			*
			*	@param	n	string containing the decimal representation of the integer to be division dividend.
			* @param i Integer containing the division divisor.
			*
			*	@return Integer containing the dividend.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator%(const std::string & n, const Integer & i) {
			Integer opn(n), q, r;

			opn.divide(i, q, r);
			r._modified = true;

			return r;
		}

		/**	@brief C-string divide operator.
			* Divides Integer value to a decimal integer writen on a C-string (char
			*	array ended with \0). Division module is returned.
			*
			*	@param	n	C-string containing the decimal representation of the integer to be the dividend.
			* @param i Integer containing the division divisor
			*
			*	@return Integer containing the module.
			* @overload
			* @warning friend operator
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator%(const char * n, const Integer & i) {
			return std::string(n) % i;
		}

		/**	@brief Integer module assignment operator
			*	Divides Integer object value to the current Integer and saves the
			*	module into the current Integer. Current Integer is returned after the
			*	division.
			*
			*	@param	n	Integer to be divisor
			*
			*	@return Current Integer after the operation and division module.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		inline const Integer & operator%=(const Integer & d) {
			Integer q;

			this->divide(d,q,*this);
			this->_modified = true;

			return *this;
		}

		/**	@brief Integral type integer module assignment operator
			*	Divides current Integer to the integral type integer value and saves the
			*	module into the current Integer. Current Integer is returned after the
			*	division.
			*
			*	@param	n	integral type integer to be divisor
			*
			*	@return Current Integer after the operation and division module.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline const Integer & operator%=(INT_T d) {
			Integer div(d), q;

			this->divide(div, q, *this);
			this->_modified = true;

			return *this;
		}

		/**	@brief String module assignment operator
			*	Divides current Integer to the a decimal integer value written on a string
			*	and saves the	module into the current Integer. Current Integer
			*	is returned after the division.
			*
			*	@param	n	string containing the decimal representation of the integer
			*		to be the division divisor.
			*
			*	@return Current Integer after the operation and division module.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator%=(const std::string & d) {
			Integer div(d), q;

			this->divide(div, q, *this);
			this->_modified = true;

			return *this;
		}

		/**	@brief C-string module assignment operator
			*	Divides current Integer to the a decimal integer value written on a C-string
			*	(char array ended with |0) and saves the module into the current
			*	Integer. Current Integer is returned after the division.
			*
			*	@param	n	C-string containing division divisor.
			*
			*	@return Current Integer after the operation and division module.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw zero_division_error when division divisor d is zero
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator%=(const char * d) {
			return (*this) %= std::string(d);
		}

		/** @brief Integer pow
		 *	Computes the power of an integer to an exponent and saves result into
		 *		the current Integer.
		 *
		 *	@param exp Integer containing the exponent
		 *  @return Power of the current Integer to the exponent exp
		 *  @pre exp must be positive
		 *  @overload
		 * 	@throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when exp is negative.
		*/
		inline Integer pow (const Integer & exp) {
			this->power(exp);
			this->_modified = true;

			return *this;
		}

		/** @brief Integral type integer pow
		 *	Computes the power of an integer to an exponent stored on an
		 *	integral type integer and saves result into the current Integer.
		 *
		 *	@param exp Integral type integer containing the exponent
		 *  @return The result of power the current Integer to the exponent exp
		 *  @pre exp must be positive
		 *  @overload
		 *	@note Following types can be passed as integral type bool, char,
		 *		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
		 *		long, long long.
		 * 	@throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when exp is negative.
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline Integer pow (INT_T exp) {
			Integer exp_int(exp);
			this->power(exp_int);
			this->_modified = true;

			return *this;
		}

		/** @brief String pow
		 *	Computes the power of an integer to an exponent stored on an
		 *	string containing the decimal representation of the exponent.
		 *
		 *	@param exp String containing the decimal representation of the exponent.
		 *  @return The result of power the current Integer to the exponent exp
		 *  @pre exp must be positive
		 *  @overload
		 * 	@throw std::bad_alloc on memory allocation error
		 *  @throw std::invalid_argument when exp is negative.
		 *  @throw std::invalid_argument when input string contains a
		 *	non-valid integer decimal character.
		*/
		inline Integer pow (const std::string & exp) {
			Integer exp_int(exp);
			this->power(exp_int);

			this->_modified = true;

			return *this;
		}

		/** @brief C-string pow
		 *	Computes the power of an integer to an exponent stored on an
		 *	c-string (char array ended with \0) containing the decimal
		 *	representation of the exponent and stores result into the current Integer.
		 *
		 *	@param exp C-string containing the decimal representation of the exponent.
		 *  @return The result of power the current Integer to the exponent exp
		 *  @pre exp must be positive
		 *  @overload
		 * 	@throw std::bad_alloc on memory allocation error
		 *  @throw std::invalid_argument when exp is negative.
		 *  @throw std::invalid_argument when input string contains a
		 *	non-valid integer decimal character.
		*/
		inline Integer pow (const char * exp) {
			this->pow(std::string(exp));
			return *this;
		}

		/** @brief Computes absolute value of the current Integer and stores it into
			*	the current Integer.
			* @return Absolute value of current Integer
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer abs() {
			if (this->is_negative())
				this->negative();

			return *this;
		}

		/// Static functions

		/** @brief Computes absolute value of the current Integer and returns it.
			*
		 	* @param n Integer for which absolute value will be computed
		 	* @return Integer object containing computed absolute value.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline static Integer abs(const Integer & n) {
			Integer i(n);
			i.abs();
			return i;
		}

		/** @brief Computes power of a base to an exponent.
			*
			* @param base Base of the power.
			*	@param exp Exponent of the power.
			* @pre exp must be positive
 		 	* @throw std::bad_alloc on memory allocation error
 		 	* @throw std::invalid_argument when exp is negative.
			* @return The result of the power.
		*/
		inline static Integer pow(const Integer & base, const Integer & exp) {
			Integer ret(base);
			ret.pow(exp);

			return ret;
		}

		// Relational operators

		/**	@brief Integer greater than operator
			*	Determines whether current Integer value is greater than
			*	the passed one.
			*
			*	@param n Integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than the passed Integer value or not.
			*	@note Const method that does not modify the current Integer.
		*/
		inline bool operator>(const Integer & n) const {return this->compare(n)>0;};

		/**	@brief Integral type integer greater than operator
			*	Determines whether current Integer value is greater than
			*	a passed integral type integer.
			*
			*	@param n integral type integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than the passed integral type integer or not.
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline bool operator>(INT_T n) const {return this->compare(Integer(n)) > 0;};

		/**	@brief Integral type integer greater than operator
			*	Determines whether an integral type integer is greater than
			*	Integer object value.
			*
			*	@param n integral type integer first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether integral type integer is greater than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @warning friend operator
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline bool operator>(INT_T n, const Integer & i) {
			return Integer(n).compare(i) > 0;
		}

		/**	@brief string greater than operator
			*	Determines whether current Integer value is greater than
			*	a passed decimal integer written in a string.
			*
			*	@param n string containing integer that will be compared with the
			*		current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator>(const std::string & n) const {return this->compare(Integer(n)) > 0;};

		/**	@brief C-string greater than operator
			*	Determines whether current Integer value is greater than
			*	a passed decimal integer written in a c-string (char array ended with \0).
			*
			*	@param n c-string containing integer that will be compared with the
			*		current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator>(const char * n) const {return this->compare(std::string(n)) > 0;};

		/**	@brief String greater than operator
			*	Determines whether a decimal integer written in a string is greater than
			*	a Integer object value.
			*
			*	@param n string first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether string is greater than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator>(const std::string & n, const Integer & i) {
			return Integer(n).compare(i) > 0;
		}

		/**	@brief C-string greater than operator
			*	Determines whether a decimal integer written in a c-string (char array
			*	ended with \0) is greater than a Integer object value.
			*
			*	@param n C-string first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether C-string is greater than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator>(const char * n, const Integer & i) {
			return std::string(n) > i;
		}

		/**	@brief Integer greater than or equal operator
			*	Determines whether current Integer value is greater than or equal than
			*	the passed one.
			*
			*	@param n Integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than or equal than the passed Integer value or not.
			*	@note Const method that does not modify the current Integer.
			*	@overload
		*/
		inline bool operator>=(const Integer & n) const {return this->compare(n) >= 0;};

		/**	@brief Integral type integer greater than or equal operator
			*	Determines whether current Integer value is greater than or equal than
			*	a passed integral type integer.
			*
			*	@param n integral type integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than or equal than the passed integral type integer or not.
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @overload
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline bool operator>=(INT_T n) const {return this->compare(Integer(n)) >= 0;};

		/**	@brief Integral type integer greater than or equal operator
			*	Determines whether an integral type integer is greater than or equal than
			*	a passed Integer.
			*
			*	@param n integral type integer first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than or equal than the passed integral type integer or not.
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @overload
			* @warning friend operator
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline bool operator>=(INT_T n, const Integer & i) {
			return Integer(n).compare(i) >= 0;
		}

		/**	@brief String greater than or equal operator
			*	Determines whether current Integer value is greater or equal than
			*	the value of a decimal integer written in a string.
			*
			*	@param n string containing the integer that will be compared with the
			*		current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	or equal than the passed Integer value or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator>=(const std::string & n) const {return this->compare(Integer(n)) >= 0;};

		/**	@brief C-string greater than or equal operator
			*	Determines whether current Integer value is greater than or equal than
			*	a passed decimal integer written in a c-string (char array ended with \0).
			*
			*	@param n c-string containing integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than or equal than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator>=(const char * n) const {return this->compare(std::string(n)) >= 0;};

		/**	@brief String greater than or equal operator
			*	Determines whether a decimal integer written in a string is greater than
			*	or equal than a Integer object value.
			*
			*	@param n string first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether string is greater than or equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @warning friend operator
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator>=(const std::string & n, const Integer & i) {
			return Integer(n).compare(i) >= 0;
		}

		/**	@brief C-string greater than or equal operator
			*	Determines whether a decimal integer written in a c-string (char array
			*	ended with \0) is greater than or equal than a Integer object value.
			*
			*	@param n C-string first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether C-string is greater than or equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @warning friend operator
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator>=(const char * n, const Integer & i) {
			return std::string(n) >= i;
		}

		/**	@brief Integer less than operator
			*	Determines whether current Integer value is less than
			*	the passed one.
			*
			*	@param n Integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is less
			*	than the passed Integer value or not.
			*	@note Const method that does not modify the current Integer.
		*/
		inline bool operator<(const Integer & n) const {return this->compare(n)<0;};

		/**	@brief Integral type integer less than operator
			*	Determines whether current Integer value is less than
			*	a passed integral type integer.
			*
			*	@param n integral type integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is less
			*	than the passed integral type integer or not.
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline bool operator<(INT_T n) const {return this->compare(Integer(n)) < 0;};

		/**	@brief Integral type integer less than operator
			*	Determines whether an integral type integer is less than
			*	Integer object value.
			*
			*	@param n integral type integer first term to be compared with second term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether integral type integer is less than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @warning friend operator
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline bool operator<(INT_T n, const Integer & i) {
			return Integer(n).compare(i) < 0;
		}

		/**	@brief string less than operator
			*	Determines whether current Integer value is less than
			*	a passed decimal integer written in a string.
			*
			*	@param n string containing integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is less
			*	than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator<(const std::string & n) const {return this->compare(Integer(n)) < 0;};

		/**	@brief C-string less than operator
			*	Determines whether current Integer value is less than
			*	a passed decimal integer written in a c-string (char array ended with \0).
			*
			*	@param n c-string containing integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is less
			*	than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator<(const char * n) const {return this->compare(std::string(n)) < 0;};

		/**	@brief String less than operator
			*	Determines whether a decimal integer written in a string is less than
			*	a Integer object value.
			*
			*	@param n string containing decimal integer as first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether string is less than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator<(const std::string & n, const Integer & i) {
			return Integer(n).compare(i) < 0;
		}

		/**	@brief C-string less than operator
			*	Determines whether a decimal integer written in a c-string (char array
			*	ended with \0) is less than a Integer object value.
			*
			*	@param n C-string containing decimal integer as first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether C-string is less than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator<(const char * n, const Integer & i) {
			return std::string(n) < i;
		}

		/**	@brief Integer less than or equal operator
			*	Determines whether current Integer value is less than or equal than
			*	the passed one.
			*
			*	@param n Integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is less
			*	than or equal than the passed Integer value or not.
			*	@note Const method that does not modify the current Integer.
			*	@overload
		*/
		inline bool operator<=(const Integer & n) const {return this->compare(n) <= 0;};

		/**	@brief Integral type integer less than or equal operator
			*	Determines whether current Integer value is less than or equal than
			*	a passed integral type integer.
			*
			*	@param n integral type integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is greater
			*	than or equal than the passed integral type integer or not.
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @overload
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline bool operator<=(INT_T n) const {return this->compare(Integer(n)) <= 0;};

		/**	@brief Integral type integer less than or equal operator
			*	Determines whether an integral type integer is less than or equal than
			*	a passed Integer.
			*
			*	@param n integral type integer first comparison term.
			*	@param i Integer second comparison term.
			*
			*	@return bool indicating whether current Integer value is less
			*	than or equal than the passed integral type integer or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @warning friend operator
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline bool operator<=(INT_T n, const Integer & i) {
			return Integer(n).compare(i) <= 0;
		}

		/**	@brief String less than or equal operator
			*	Determines whether current Integer value is less or equal than
			*	the value of a decimal integer written in a string.
			*
			*	@param n string containing the integer that will be compared with the
			*		current Integer.
			*
			*	@return bool indicating whether current Integer value is less
			*	or equal than the passed Integer value or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator<=(const std::string & n) const {return this->compare(Integer(n)) <= 0;};

		/**	@brief C-string less than or equal operator
			*	Determines whether current Integer value is less than or equal than
			*	a passed decimal integer written in a c-string (char array ended with \0).
			*
			*	@param n c-string containing integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is less
			*	than or equal than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator<=(const char * n) const {return this->compare(std::string(n)) <= 0;};

		/**	@brief String less than or equal operator
			*	Determines whether a decimal integer written in a string is less than
			*	or equal than a Integer object value.
			*
			*	@param n string containing the decimal integer as first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether string is less than or equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @warning friend operator
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator<=(const std::string & n, const Integer & i) {
			return Integer(n).compare(i) <= 0;
		}

		/**	@brief C-string less than or equal operator
			*	Determines whether a decimal integer written in a c-string (char array
			*	ended with \0) is less than or equal than a Integer object value.
			*
			*	@param n C-string first term to be compared with second term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether C-string is less than or equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @warning friend operator
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator<=(const char * n, const Integer & i) {
			return std::string(n) <= i;
		}

		/**	@brief Integer equal than operator
			*	Determines whether current Integer value is equal than
			*	the passed one.
			*
			*	@param n Integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is equal
			*	than the passed Integer value or not.
			*	@note Const method that does not modify the current Integer.
		*/
		inline bool operator==(const Integer & n) const {return this->compare(n) == 0;};

		/**	@brief Integral type integer equal than operator
			*	Determines whether current Integer value is equal than
			*	a passed integral type integer.
			*
			*	@param n integral type integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is equal
			*	than the passed integral type integer or not.
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline bool operator==(INT_T n) const {return this->compare(Integer(n)) == 0;};

		/**	@brief Integral type integer equal than operator
			*	Determines whether integral type integer is equal than
			*	Integer object value.
			*
			*	@param n integral type integer first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether integral type integer is equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			* @warning friend operator
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline bool operator==(INT_T n, const Integer & i) {
			return Integer(n).compare(i) == 0;
		}

		/**	@brief string equal than operator
			*	Determines whether current Integer value is equal than
			*	a passed decimal integer written in a string.
			*
			*	@param n string containing integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is equal
			*	than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator==(const std::string & n) const {return this->compare(Integer(n)) == 0;};

		/**	@brief C-string equal than operator
			*	Determines whether current Integer value is equal than
			*	a passed decimal integer written in a c-string (char array ended with \0).
			*
			*	@param n c-string containing integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is equal
			*	than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator==(const char * n) const {return this->compare(std::string(n)) == 0;};

		/**	@brief String equal than operator
			*	Determines whether a decimal integer written in a string is equal than
			*	a Integer object value.
			*
			*	@param n string containing integer as first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether string is equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @warning friend operator
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator==(const std::string & n, const Integer & i) {
			return Integer(n).compare(i) == 0;
		}

		/**	@brief C-string equal than operator
			*	Determines whether a decimal integer written in a c-string (char array
			*	ended with \0) is equal than a Integer object value.
			*
			*	@param n C-string containing integer as first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether C-string is equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @warning friend operator
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator==(const char * n, const Integer & i) {
			return std::string(n) == i;
		}

		/**	@brief Integer not equal than operator
			*	Determines whether current Integer value is not equal than
			*	the passed one.
			*
			*	@param n Integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is not equal
			*	than the passed Integer value or not.
			*	@note Const method that does not modify the current Integer.
		*/
		inline bool operator!=(const Integer & n) const {return this->compare(n) != 0;};

		/**	@brief Integral type integer not equal than operator
			*	Determines whether current Integer value is not equal than
			*	a passed integral type integer.
			*
			*	@param n integral type integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is not equal
			*	than the passed integral type integer or not.
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline bool operator!=(INT_T n) const {return this->compare(Integer(n)) != 0;};

		/**	@brief Integral type integer not equal than operator
			*	Determines whether integral type integer is not equal than
			*	Integer object value.
			*
			*	@param n integral type integer first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether integral type integer is not equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @warning friend operator
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline bool operator!=(INT_T n, const Integer & i) {
			return Integer(n).compare(i) != 0;
		}

		/**	@brief string not equal than operator
			*	Determines whether current Integer value is not equal than
			*	a passed decimal integer written in a string.
			*
			*	@param n string containing integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is not equal
			*	than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator!=(const std::string & n) const {return this->compare(Integer(n)) != 0;};

		/**	@brief C-string not equal than operator
			*	Determines whether current Integer value is not equal than
			*	a passed decimal integer written in a c-string (char array ended with \0).
			*
			*	@param n c-string containing integer that will be compared with the current Integer.
			*
			*	@return bool indicating whether current Integer value is not equal
			*	than the passed string or not.
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline bool operator!=(const char * n) const {return this->compare(std::string(n)) != 0;};

		/**	@brief String not equal than operator
			*	Determines whether a decimal integer written in a string is not equal than
			*	a Integer object value.
			*
			*	@param n string containing decimal integer as first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether string is not equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @warning friend operator
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator!=(const std::string & n, const Integer & i) {
			return Integer(n).compare(i) != 0;
		}

		/**	@brief C-string not equal than operator
			*	Determines whether a decimal integer written in a c-string (char array
			*	ended with \0) is not equal than a Integer object value.
			*
			*	@param n C-string first term.
			*	@param i Integer second term.
			*
			*	@return bool indicating whether C-string is not equal than
			*	Integer
			*	@note Const method that does not modify the current Integer.
			* @overload
			* @warning friend operator
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline bool operator!=(const char * n, const Integer & i) {
			return std::string(n) != i;
		}

		// Logic operators

		/**	@brief Bitwise not operator
			*	Perform a bitwise NOT operation over the current Integer and returns
			*	the resulting Integer.
			*	@note Const method that does not modify the current Integer.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator~() const {
			Integer r(*this);
			r.bitwise_not();
			r._modified = true;

			return r;
		}

		/**	@brief Integer Bitwise AND operator
			*	Performs bitwise AND operation between the current Integer and
			*	a passed one. Resulting Integer is returned.
			*
			* @param n Integer that will be bitwised AND with current Integer
			*
			*	@note Const method that does not modify the current Integer.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator&(const Integer & n) const {
			Integer r(*this);
			r.bitwise_and(n);
			r._modified = true;

			return r;
		}

		/**	@brief Integral type integer Bitwise AND operator
			*	Performs bitwise AND operation between the current Integer and
			*	a integral type integer. Resulting Integer is returned.
			*
			* @param n integral type integer that will be bitwised AND with current Integer
			*
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline Integer operator&(INT_T n) const {
			Integer ret(*this), aux(n);

			ret.bitwise_and(aux);
			ret._modified = true;

			return ret;
		}

		/**	@brief Integral type integer Bitwise AND operator
			*	Performs bitwise AND operation between a integral type integer and a
			*	Integer object. Resulting Integer is returned.
			*
			* @param n integral type integer first operand
			* @param i Integer second operand
			*
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline Integer operator&(INT_T n, const Integer & i) {
			Integer ret(n);

			ret.bitwise_and(i);
			ret._modified = true;

			return ret;
		}

		/**	@brief String Bitwise AND operator
			*	Performs bitwise AND operation between the current Integer and
			*	a decimal integer written in a string. Resulting Integer is returned.
			*
			* @param n string that will be bitwised AND with current Integer
			*
			*	@note Const method that does not modify the current Integer.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator&(const std::string & n) const {
			Integer ret(*this), aux(n);

			ret.bitwise_and(aux);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string Bitwise AND operator
			*	Performs bitwise AND operation between the current Integer and
			*	a decimal integer written in a C-string (char array ended with \0).
			*	Resulting Integer is returned.
			*
			* @param n c-string that will be bitwised AND with current Integer
			*
			*	@note Const method that does not modify the current Integer.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator&(const char * n) const {
			return (*this) & std::string(n);
		}

		/**	@brief String Bitwise AND operator
			*	Performs bitwise AND operation between a decimal integer written in a
			*	string and a Integer object. Resulting Integer is returned.
			*
			* @param n string first operand
			* @param i Integer second operand
			*
			*	@note Const method that does not modify the current Integer.
			*	@warning friend operator
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator&(const std::string & n, const Integer & i) {
			Integer ret(n);

			ret.bitwise_and(i);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string Bitwise AND operator
			*	Performs bitwise AND operation between a decimal integer written in a
			*	C-string (char array ended with \0) and a Integer object.
			*	Resulting Integer is returned.
			*
			* @param n c-string first operand
			* @param i Integer second operand
			*
			*	@note Const method that does not modify the current Integer.
			*	@warning friend operator
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator&(const char * n, const Integer & i) {
			return std::string(n) & i;
		}

		/**	@brief Integer Bitwise AND and assign operator
			*	Performs bitwise AND operation between the current Integer and
			*	a passed one and stores the result into the current Integer. Current
			*	Integer is returned after the operation.
			*
			* @param n Integer that will be bitwised AND with current Integer
			*
			* @overload
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator&=(const Integer & n) {
			this->bitwise_and(n);
			this->_modified = true;

			return *this;
		}

		/**	@brief Integral type integer Bitwise AND and assign operator
			*	Performs bitwise AND operation between the current Integer and
			*	a passed integral type integer and stores the result into the current
			*	Integer. Current Integer is returned after the operation.
			*
			* @param n integral type integer that will be bitwised AND with current
			*	Integer
			*
			* @overload
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline const Integer & operator&=(INT_T n) {
			this->bitwise_and(Integer(n));
			this->_modified = true;

			return *this;
		}

		/**	@brief String Bitwise AND and assign operator
			*	Performs bitwise AND operation between the current Integer and
			*	a decimal integer written in a string and stores the result into the current
			*	Integer. Current Integer is returned after the operation.
			*
			* @param n string that will be bitwised AND with current Integer
			*
			*	@return The resulting Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator&=(const std::string & n) {
			this->bitwise_and(Integer(n));
			this->_modified = true;

			return *this;
		}

		/**	@brief C-string Bitwise AND and assign operator
			*	Performs bitwise AND operation between the current Integer and
			*	a decimal integer written in a c-string (char array ended with |0) and
			*	stores the result into the current Integer. Current Integer is returned
			*	after the operation.
			*
			* @param n c-string that will be bitwised AND with current Integer
			*
			*	@return The resulting Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator&=(const char * n) {
			return (*this) &= std::string(n);
		}

		/**	@brief Integer Bitwise OR operator
			*	Performs bitwise OR operation between the current Integer and
			*	a passed one. Resulting Integer is returned.
			*
			* @param n Integer that will be bitwised OR with current Integer
			*
			*	@note Const method that does not modify the current Integer.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator|(const Integer & n) const {
			Integer r(*this);
			r.bitwise_or(n);
			r._modified = true;

			return r;
		}

		/**	@brief Integral type integer Bitwise OR operator
			*	Performs bitwise OR operation between the current Integer and
			*	a integral type integer. Resulting Integer is returned.
			*
			* @param n integral type integer that will be bitwised OR with current Integer
			*
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline Integer operator|(INT_T n) const {
			Integer ret(*this), aux(n);
			ret.bitwise_or(aux);
			ret._modified = true;

			return ret;
		}

		/**	@brief Integral type integer Bitwise OR operator
			*	Performs bitwise OR operation between a integral type integer and a
			*	Integer object. Resulting Integer is returned.
			*
			* @param n integral type integer first operand
			* @param i Integer second operand
			*
			*	@note Const method that does not modify the current Integer.
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		friend inline Integer operator|(INT_T n, const Integer & i) {
			Integer ret(n);

			ret.bitwise_or(i);
			ret._modified = true;

			return ret;
		}

		/**	@brief String Bitwise OR operator
			*	Performs bitwise OR operation between the current Integer and
			*	a decimal integer written in a string. Resulting Integer is returned.
			*
			* @param n string that will be bitwised OR with current Integer
			*
			*	@note Const method that does not modify the current Integer.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator|(const std::string & n) const {
			Integer ret(*this), aux(n);
			ret.bitwise_or(aux);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string Bitwise OR operator
			*	Performs bitwise OR operation between the current Integer and
			*	a decimal integer written in a C-string (char array ended with \0).
			*	Resulting Integer is returned.
			*
			* @param n c-string that will be bitwised OR with current Integer
			*
			*	@note Const method that does not modify the current Integer.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline Integer operator|(const char * n) const {
			return (*this) | std::string(n);
		}

		/**	@brief String Bitwise OR operator
			*	Performs bitwise OR operation between a decimal integer written in a
			*	string and a Integer object. Resulting Integer is returned.
			*
			* @param n string first operand
			* @param i Integer second operand
			*
			*	@note Const method that does not modify the current Integer.
			*	@warning friend operator
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator|(const std::string & n, const Integer & i) {
			Integer ret(n);

			ret.bitwise_or(i);
			ret._modified = true;

			return ret;
		}

		/**	@brief C-string Bitwise OR operator
			*	Performs bitwise OR operation between a decimal integer written in a
			*	C-string (char array ended with \0) and a Integer object.
			*	Resulting Integer is returned.
			*
			* @param n c-string first operand
			* @param i Integer second operand
			*
			*	@note Const method that does not modify the current Integer.
			*	@warning friend operator
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		friend inline Integer operator|(const char * n, const Integer & i) {
			return std::string(n) | i;
		}

		/**	@brief Integer Bitwise OR and assign operator
			*	Performs bitwise OR operation between the current Integer and
			*	a passed one and stores the result into the current Integer. Current
			*	Integer is returned after the operation.
			*
			* @param n Integer that will be bitwised OR with current Integer
			*
			* @overload
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator|=(const Integer & n) {
			this->bitwise_or(n);
			this->_modified = true;

			return *this;
		}

		/**	@brief Integral type integer Bitwise OR and assign operator
			*	Performs bitwise OR and assign operation between the current Integer and
			*	a passed integral type integer and stores the result into the current
			*	Integer. Current Integer is returned after the operation.
			*
			* @param n integral type integer that will be bitwised OR with current
			*	Integer
			*
			* @overload
			*	@note Following types can be passed as integral type bool, char,
			*		char8_t (since C++20), char16_t, char32_t, wchar_t, short, int,
			*		long, long long.
			*	@return The resulting Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		template <class INT_T,
						class = typename std::enable_if<std::is_integral<INT_T>::value>::type>
		inline const Integer & operator|=(INT_T n) {
			this->bitwise_or(Integer(n));
			this->_modified = true;

			return *this;
		}

		/**	@brief String Bitwise OR and assign operator
			*	Performs bitwise OR operation between the current Integer and
			*	a decimal integer written in a string and stores the result into the current
			*	Integer. Current Integer is returned after the operation.
			*
			* @param n string that will be bitwised OR with current Integer
			*
			*	@return The resulting Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator|=(const std::string & n) {
			this->bitwise_or(Integer(n));
			this->_modified = true;

			return *this;
		}

		/**	@brief C-string Bitwise OR and assign operator
			*	Performs bitwise OR operation between the current Integer and
			*	a decimal integer written in a c-string (char array ended with |0) and
			*	stores the result into the current Integer. Current Integer is returned
			*	after the operation.
			*
			* @param n c-string that will be bitwised OR with current Integer
			*
			*	@return The resulting Integer.
			* @overload
			* @throw std::bad_alloc on memory allocation error
			* @throw std::invalid_argument when input string contains a
			*	non-valid integer decimal character.
		*/
		inline const Integer & operator|=(const char * n) {
			return (*this) |= std::string(n);
		}

		/**	@brief Bitwise shift left operator
			*	Performs an arithmetic bitwise left shift of "d" bits on the current
		 	*	Integer. Resulting Integer is returned.
			*
			*	@param	d The number of bits to shift.
			*
			*	@return The resulting Integer.
			*	@note Const method that does not modify the current Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator<<(size_t d) const {
			Integer r(*this);
			r.bitwise_shift_left(d);
			r._modified = true;

			return r;
		}

		/**	@brief Bitwise shift left and assign operator
			*	Performs an arithmetic bitwise left shift of "d" bits on the current
		 	*	Integer returning the  modified current Integer.
			*
			*	@param	d The number of bits to shift.
			*
			*	@return The current Integer after applying the operation.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator<<=(size_t d) {
			this->bitwise_shift_left(d);
			this->_modified = true;

			return *this;
		}

		/**	@brief Bitwise shift right operator
			*	Performs an arithmetic bitwise right shift of "d" bits on the current
		 	*	Integer. Resulting Integer is returned.
			*
			*	@param	d The number of bits to shift.
			*
			*	@return The resulting Integer.
			*	@note Const method that does not modify the current Integer.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline Integer operator>>(size_t d) const {
			Integer r(*this);
			r.bitwise_shift_right(d);
			r._modified = true;

			return r;
		}

		/**	@brief Bitwise shift right and assign operator
			*	Performs an arithmetic bitwise right shift of "d" bits on the current
		 	*	Integer returning the modified current Integer.
			*
			*	@param	d The number of bits to shift.
			*
			*	@return The current Integer after applying the operation.
			* @throw std::bad_alloc on memory allocation error
		*/
		inline const Integer & operator>>=(size_t d) {
			this->bitwise_shift_right(d);
			this->_modified = true;

			return *this;
		}

		//	Input/Output operators

		/**	@brief Returns a string with the decimal representation of the Integer.
		 	*	@overload
			*	@return string
		*/
		inline const std::string & getString() {
			if (this->_modified) {
				this->integer_to_string(this->_str_fmt_cache);
				this->_modified = false;
			}

			return this->_str_fmt_cache;
		}

		/**	@brief Returns string containing the decimal representation of the Integer.
			*	@return string
			* @note Const method that does not modify current Integer
		 	*	@overload
		*/
		inline std::string getString() const {
			std::string s;
			this->integer_to_string(s);

			return s;
		}

		/**	@brief Returns long long int type value containing the Integer value.
			*	@warning Integer value should fit into the long long int type capacity,
			*		else integer_conversion_error is thrown.
		 	*	@note Const method that not modifies the object
			* @throw integer_conversion_error when Integer value exceeds long long int
			*		capacity.
			*	@return long long int
		*/
		long long int getInt() const;

		/**	@brief Returns string containing two's complement binary Integer
			*		representation.
			*	@note Const method that does not modify current Integer.
			*	@return string
		*/
		const std::string getBinary(char delimit='\0') const;

		/**	@brief Extractor operator
			*	Sends the decimal representation of the Integer to an output stream.
			*
			*	@param stream Output stream
			*	@param n Integer to be sent to the stream
			*	@return Output stream
		*/
		inline friend std::ostream & operator<<(std::ostream & stream, const Integer & n) {
			stream << n.getString();
			return stream;
		}

		/**	@brief Insertion operator.
			*	Reads a decimal integer from an input stream and store it into the
			*	input Integer.
			*
			*	Characters are readen from input strem until endline character (\n),
			*	null character (\0), the end-of-file character (EOF) or a non-valid
			*	character (non-numeric characters or the minus sign at the beginning)
			*	is found.
			*
			*	@patam stream The input stream
			*	@param	n The Integer where the readen integer will be loaded.
			*
			*	@return Input stream
			* @throw std::bad_alloc on memory allocation error
		*/
		friend std::istream & operator>>(std::istream & stream, Integer & n);

		/**	@brief string cast operator
			*	Converts current Integer into a string containing the decimal
			*		representation of the current Integer.
		 	*	@overload
			*	@note Const method that does not modify current Integer.
			*	@return string
		*/
		inline operator std::string() const {return this->getString();};

		/**	string cast operator which returns a string with the 10-base
		  * Integer's decimal representation.
		 	*	Warning: Overloaded function
			*	Returns: const string
		*/
		//inline operator const std::string() const {return this->getString();};

		/**	string cast operator which returns a char string with the 10-base
			* Integer's decimal representation.
			*	Warning: Overloaded function
			*	Returns: string
		*/
		//inline operator char*() const {return this->getString().c_str();};

		/**	@brief long long int cast operator
			*	Converts current Integer into a long long int containing the Integer
			*	value.
			*	@warning Integer value should fit into the long long int type capacity
		 	*	@note Const method that not modifies the object
			* @note The returned value can be casted to any integral type.
			*	@throw integer_conversion_error when Integer value exceeds long long int
			*		capacity.
			*	@return long long int
		*/
		inline operator long long int() {return this->getInt();};

		// Utils operators

	 /** @brief Access operator
		 * Gets the decimal digit located at index i in the decimal representation
		 *	of the Integer.
		 * @overload
		 * @return unsigned integer
		*/
		inline unsigned operator[] (size_t i) {

			//Compute string containing the 10-base representation of the integer
			if (this->_modified) {
				this->integer_to_string(this->_str_fmt_cache);
				this->_modified = false;
			}

			if (i >= this->_str_fmt_cache.length())
				throw std::out_of_range("Digit index out of range");

			return (unsigned) this->_str_fmt_cache[this->_str_fmt_cache.length() - i - 1] - '0';
		}

		/** @brief Access operator
 		 * Gets the decimal digit located at i on Integer decimal representation.
 		 * @note Overloaded function for const Integer
		 * @note const method that does not modify current Integer.
 		 * @return unsigned integer
 		*/
		inline unsigned operator[] (size_t i) const {

			//Compute string containing the 10-base representation of the integer
			std::string s;
			integer_to_string(s);


			if (i >= s.length())
				throw std::out_of_range("Digit index out of range");

			return (unsigned) s[s.length() - i - 1] - '0';
		}

		/** @brief Generates a random Integer having n_bits bits as maximum
			*	@param n_bits Max number of bits for the generated random Integer.
			*		Default is set to constant DEFAULT_MAX_N_BITS
			* @return Integer containing value randomly generated
			*	@note static method
			* @throw std::bad_alloc on memory allocation error
		*/
		static Integer rand(size_t n_bits=DEFAULT_MAX_N_BITS) {
			Integer i;
			i.set_random_value(n_bits);

			return i;
		}

		// Random generator setter

		/**
			* @brief Sets current globaal random engine for all Integer objects.
			*	@param random_engine Random engine to be set.
		*/
		inline static void set_random_engine(const std::default_random_engine & random_engine) {
			Integer::_random_engine = random_engine;
		}

		/////	Destructor	/////

		/**	@brief Destructor
			*	Deletes the current Integer.
		*/
		~Integer() {_words.clear();};

};

/////	Literal	/////

/** Template used for determining in a static way whether an Integer literal
	*	holds the expected format.
*/
template<bool IS_FIRST, char... LITERAL> struct literal_exp;

template<> struct literal_exp<false> {
	static const int value = 0;
};

template<bool IS_FIRST, char FIRST, char... REST> struct literal_exp<IS_FIRST, FIRST, REST...> {
	static_assert(FIRST >= '0' && FIRST <= '9' || IS_FIRST && FIRST == '-' && sizeof...(REST) > 0,
								"Invalid Integer literal");
	static const int value = literal_exp<false, REST...>::value;
};

/** @brief Integer literal operator
	* Construct Integer object from an Integer literal
	* @note Integer literal should match following regex -?[0-9]+_I
	*		Examples:
	*		· 1550346240562184622430_I
	*		· -469713264789116470036467703647_I
	* @return Integer containing value stated on Integer literal
*/
template<char... LITERAL> constexpr Integer operator"" _I() {
	// Check the literal is formed by valid characters
	static_assert(sizeof...(LITERAL) > 0, "Invalid Integer literal");
	literal_exp<true, LITERAL...>::value;

	constexpr const char literal[] = {LITERAL ..., 0};
	return Integer(literal);
}

#endif /// <- End of __INTEGER_HPP__
