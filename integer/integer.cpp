#include "integer.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <limits>
#include <type_traits>
#include <cassert>
#include <stdexcept>
#include <cstdint>

/**
	*	Constant for easy and secure access to Integer words
*/
#define GETBLOCK(N,I) ((i)>=(N)._words.size() || (I)<0)? (N)._words[(I)] : ((I)>=(N)._words.size() && (N).is_negative()? -1 : 0)

//Initializes the random engine
std::default_random_engine Integer::_random_engine(
									std::chrono::system_clock::now().time_since_epoch().count());

void Integer::fix() {

	if (_words.size() > 1) {

		/*	Find the first significant word (every word having 0 or -1 value
		 *		at beginning is redundant, a non-significant word, so they must be
		 *	removed) */
		int i = _words.size()-1;

		while (i>0 && (_words[i]==0 && !this->is_negative() ||
										((_words[i]==-1 ||
											_words[i]==INT32_MAX) && this->is_negative()) ) )
			i--;

		resize(i+1);
	}
	else if (_words.empty()) {
		resize(1);
		_words.back() = 0;
	}

	//Check postcondition
	#ifdef DEBUG
		assert(!(this->_words.size() > 1) || (this->_words.back() != 0 &&
																									this->_words.back() != -1));

		for (size_t i = 0; i < this->_words.size()-1; i++)
			assert((this->_words[i] & 0x80000000) == 0);
	#endif

	return;
}

void Integer::split(Integer & n1, Integer & n2, size_t part_size) const {

	//Check precondition
	#ifdef DEBUG
		assert(part_size > 0);
	#endif

	//Extract the first word partition and stores into n1
	n1._words.clear();
	if (part_size < this->_words.size())
		n1._words.insert( n1._words.begin(), this->_words.begin(),
																					this->_words.begin() + part_size );
	else
		n1._words.insert( n1._words.begin(), this->_words.begin(),
																													this->_words.end());
	n1.fix();

	//Extract the second word partition (remain words) and store into n2
	n2._words.clear();
	if (n1._words.size() == this->_words.size())
		n2._words.push_back(0);
	else
		n2._words.insert( n2._words.begin(), this->_words.begin() + part_size,
																												this->_words.end() );
	n2.fix();

	//Check postcondition
	#ifdef DEBUG
		assert(!(part_size <= this->_words.size()) ||
								(n1._words.size() <= part_size &&
								n2._words.size() <= MAX(this->_words.size() - n1._words.size(), 1)));
	#endif

}

void Integer::add(const Integer & n1, int32_t n2, size_t index) {

	//Check the precondition
	#ifdef DEBUG
		assert(index >= 0 && index < n1._words.size());
	#endif

	int64_t carry = n2;

	//Add cumulated carry computed in every partial addition
	size_t i;
	for (i = index; i < n1._words.size(); i++) {

		//Add carry cumulated to each n1 Integer word
		carry += n1._words[i];

		//	Extracts word from the carry and allocates an extra word is needed
		if (i >= this->_words.size())
			this->resize(i + 1);

		this->_words[i] = ((int32_t)carry & (int32_t)0x7fffffff);

		carry>>=31;
	}

	//Remove redundant remain words
	if (i < this->_words.size())
		this->resize(i);

	//Extract last word
	if (carry != 0 && carry != -1) {
			this->resize(i + 1);
			this->_words[i] = (carry & (int32_t)-1);
	}


	if (carry < 0)
		//Set the negative sign on the current Integer if needed
		_words.back() |= 0x80000000;
}

void Integer::add(const Integer & n1, const Integer & n2, bool negative) {

	#ifdef DEBUG
		Integer n1_org(n1), n2_org(n2);
	#endif

	Integer result;
	Integer n2_aux(n2);

	if (negative == true)
		n2_aux.negative();

	size_t size;

	//Determine longer and shorter integer
	const Integer & bigger = n1._words.size() >= n2._words.size()? n1 : n2_aux;
	const Integer & smaller = n1._words.size() < n2._words.size()? n1 : n2_aux;

	size = smaller._words.size();

	//Perform addition block by block
	size_t i(0);

	result.add(bigger,smaller._words[i],i);

	for (i=1; i<size;i++) {
			result.add(result,smaller._words[i],i);
	}

	//Remove reduntant blocks
	result.fix();

	//Save result into the current Integer
	this->_words.swap(result._words);

	//Check postconditions
	#ifdef DEBUG
		// n1 + n2 = this => this >= n1 && this >= n2
		assert( ((negative ^ n2_org.is_negative()) || n1_org.is_negative()) ||
			(this->compare(n1_org) >= 0 && this->compare(Integer::abs(n2_org)) >= 0));

		// n1 - n2 = this => this <= n1
		assert( !(negative ^ n2_org.is_negative() && !n1_org.is_negative()) ||
																			this->compare(n1_org) <= 0);

		// n2 - n1 = this => this <= n2
		assert( !(!(negative ^ n2_org.is_negative()) && n1_org.is_negative()) ||
																			this->compare(Integer::abs(n2_org)) <= 0);

		// - n1 - n2 = this => this <= n1 && this <= n
		assert( !((negative ^ n2_org.is_negative()) && n1_org.is_negative()) ||
			(this->compare(n1_org) <= 0 && this->compare(n2_org) <= 0));

		// On n1 + 0 = this => n1 = this, on n1 - 0 = this => n1 = this
		assert(! (n2_org.compare(0) == 0) || this->compare(n1_org) == 0);

		// On 0 + n2 = this => n2 = this, on 0 - n2 = this => -n2 = this
		assert(! (n1_org.compare(0) == 0) ||
								this->compare(n2_org) == 0 && !negative ||
								this->compare(-n2_org) == 0 && negative);

		// When deduct, n1 < n2 => this < 0
		assert(!((negative ^ n2_org.is_negative() && !n1_org.is_negative()) &&
										n1_org.compare(Integer::abs(n2_org)) == -1) ||
												(this->is_negative() && this->compare(0) == -1));

		// When deduct, n1 == n2 => this == 0
		assert(!((negative ^ n2_org.is_negative() ^ n1_org.is_negative()) &&
											Integer::abs(n1_org).compare(Integer::abs(n2_org)) == 0) ||
												(this->is_zero() && this->compare(0) == 0));

		// When deduct, n1 > n2 => this > 0
		assert(!((negative ^ n2_org.is_negative()) && !n1_org.is_negative() &&
												n1_org.compare(Integer::abs(n2_org)) == 1) ||
										(!(this->is_zero() || this->is_negative())  &&
											this->compare(0) == 1));
	#endif
}

void Integer::multiply(const Integer & n1, const Integer & n2) {

	// Prepare postcondition check
	#ifdef DEBUG
		Integer n1_org(n1), n2_org(n2);
	#endif

	const Integer minus(-1);

	//Product by 0
	if (n1.is_zero() || n2.is_zero())
		this->reset();
	else if (n1 == minus) {
		//Product -1 * n2
		//Turn n2 into negative and store it into the current Integer
		*this = n2;
		this->negative();
	}
	else if (n2 == minus) {
		// Product n1 * -1
		//Turn n1 into negative and store it into the current Integer
		*this = n1;
		this->negative();
	}
	else {

		/****************************
		 *	Algorithm of Karashutba *
		*****************************/

		//Perform direct multiplication whether integer are small enough
		if (n1._words.size() == 1 && n2._words.size() == 1) {

			/*The resulting Integer may take 64 bits as max so
			* it's required to split it into 32 bits-blocks*/

			int64_t result = ((int64_t)n1._words.back()) * n2._words.back();

			this->resize(2);
			for (size_t i = 0; i < this->_words.size(); i++) {
				this->_words[i] = (result & (int32_t)0x7fffffff);
				result >>= 31;
			}

			if (result < 0)
				this->_words.back() |= 0x80000000;

			this->fix();

		}
		else {
			//Integers are not small enough so a subdivision is performed

			//number of blocks of subdivisions
			size_t size = MAX(n1._words.size() , n2._words.size());
			size = (size/2) + (size%2);


			Integer w,x;
			Integer y,z;

			Integer r;
			Integer p;
			Integer q;

			//Splits operands into two sub-integers of size length
			n1.split(x,w,size);
			n2.split(z,y,size);

			//Compute r, p y q recursively
			r.multiply(w+x, y+z);
			p.multiply(w,y);
			q.multiply(x,z);

			/*Solve partial multiplication:
			 *result = p*2^(tam*32) + (r-q-p)*2^(32*tam / 2) + q
			 */
			this->add((p<<(2*size*31)), ((r-p-q)<<(31*size)) );
			this->add(*this, q);
		}

	}

	// Check postcondition
	#ifdef DEBUG
		// On n1 > 1 and n2 > 1, this > n1 and this > n2
		assert(!(n1_org.compare(1) == 1 && n2_org.compare(1) == 1) ||
											(this->compare(n1_org) == 1 && this->compare(n2_org) == 1));

		// On n1 == 1 => this == n2
		assert(!(n1_org.compare(1) == 0) || (this->compare(n2_org) == 0));

		// On n2 == 1 => this == n1
		assert(!(n2_org.compare(1) == 0) || (this->compare(n1_org) == 0));

		// On n1 = 0 or n2 = 0, this = 0
		assert(!(n1_org.compare(0) == 0 || n2_org.compare(0) == 0) ||
											(this->is_zero() && this->compare(0) == 0));

		// On n2 == -1 => this = -n1
		assert( !(n2_org.compare(-1) == 0) || this->compare(-n1_org) == 0);

		// On n1 == -1 => this = -n2
		assert( !(n1_org.compare(-1) == 0) || this->compare(-n2_org) == 0);

		// On n1 < 0 ^ n2 < 0 => this <= 0
		assert( !(n1_org.is_negative() ^ n2_org.is_negative()) || this->is_negative() || this->is_zero());

		// On ¬ (n1_org < 0 ^ n2 < 0) => this >= 0
		assert( (n1_org.is_negative() ^ n2_org.is_negative()) ||
					( (!this->is_negative() || this->is_zero()) && this->compare(0) >= 0) );

	#endif

	return;
}

void Integer::divide(const Integer & d, Integer & quotient, Integer & remainder) const{

	// Keep original dividend value for postcondition check
	#ifdef DEBUG
		Integer original(*this);
		Integer d_org(d);
	#endif

	//Zero divission by zero check
	if (d.is_zero()) {
		throw zero_division_error();
	}

	bool org_sign = this->is_negative();
	bool sign = this->is_negative() ^ d.is_negative();

	Integer aux(d);

	//Prepare operation
	remainder = *this;
	quotient.reset(); // quotient=0

	//Operation are performed in the integers positive values
	if (remainder.is_negative())
		remainder.negative();

	if (aux.is_negative())
		aux.negative();

	// First bit index in which the operation will start
	int index = remainder.findFirstOne() - aux.findFirstOne();

	/*Shift aux Integer to make the most significant
	 *1 bit from both integer to match the same ìndex
	 */
	if (index > 0)
		aux.bitwise_shift_left(index);

	int comp;

	//Perform division
	while(index >= 0) {

		quotient.bitwise_shift_left(1);

		// Perform deduction whether the dividend is greater than remainder
		comp = aux.compare(remainder);

		if (comp < 1) {
				remainder.add(remainder,aux,true);
				quotient.add(quotient,1);
		}

		aux.bitwise_shift_right(1);
		index--;
	}

	//Recover sign to resulting quotient
	if (sign)
		quotient.negative();

	//Remainder is always positive
	if (org_sign)
		remainder.negative();

	//Check postcondition
	#ifdef DEBUG
		// original!= 0 => 0 <= remainder < this
		assert(Integer::abs(remainder).compare(0) >= 0 && Integer::abs(remainder).compare(Integer::abs(d_org)) == -1);

		// |original| < |d| => q == 0 and remainder == original
		assert(!(Integer::abs(original).compare(Integer::abs(d_org)) == -1) ||
											quotient.is_zero() && remainder.compare(original) == 0);

		// On original = 0 => q = 0 and remainder == 0
		assert(!original.is_zero() || (quotient.is_zero() && remainder.is_zero()));

		// On original = d => q = 1 and remainder = 0
		assert(!(original.compare(d_org) == 0) || (quotient.compare(1) == 0 && remainder.is_zero()));

		// On d = 1 => original = q and remainder = 0
		assert(!(d.compare(1) == 0) || (original.compare(quotient) == 0 && remainder.is_zero()));
	#endif

}

void Integer::power(const Integer & exp) {

	// Keep original dividend value for postcondition check
	#ifdef DEBUG
		Integer original(*this);
	#endif

	//Precondition
	if (exp.is_negative())
		throw std::invalid_argument("exp must be positive or 0");

	Integer result(1);
	Integer aux(*this);
	Integer exp_aux(exp);

	while (!exp_aux.is_zero()) {
		/* this.pow(exp) can be expressed as
		 * this.pow(exp) = this.pow(exp/2) * this.pow(exp/2) when exp is even
		 * and this.pow(exp) = this.pow(exp/2) * this.pow(exp/2) * this when odd.
		 * In this way, recursive computation is applied
		*/

		if (!exp_aux.is_even()) {
			result *= aux; // On odd exp
		}

		aux *= aux;

		exp_aux.bitwise_shift_right(1); // exp_aux = exp_aux / 2
	}

	//The result is copied on the current integer in a faster way
	this->_words.swap(result._words);

	//Check postcondition
	#ifdef DEBUG
		// On exp = 0 => this = 1
		assert(!exp.is_zero() || this->compare(1) == 0);

		// On original = 1 => this = 1
		assert(!original.compare(1) == 0 || this->compare(1) == 0);

		// On original = 0 and exp != 0 => this = 0
		assert(!(original.compare(0) == 0 && !exp.is_zero()) || this->compare(0) == 0);

		// On original > 1 and exp > 1 => this > original
		assert(!(original.compare(1) == 1 && exp.compare(1) == 1) ||
																this->compare(original) == 1);

		// On original < 0 and exp.is_even() => this > 0 and this > -original
		assert(!(original.compare(0) == -1 && exp.compare(1) == 1 && exp.is_even()) ||
													(this->compare(0) == 1 && this->compare(-original) == 1));

		// On original < 0 and ¬exp.is_even() => this < 0 and this < original
		assert(!(original.compare(0) == -1 && exp.compare(1) == 1 && !exp.is_even()) ||
													(this->compare(0) == -1 && this->compare(original) == -1));
	#endif

	return;
}

void Integer::bitwise_not() {

	//Perform bitwise NOT on each block.
	for (size_t i = 0; i < _words.size() - 1; i++)
		_words[i] = ((~_words[i]) & 0x7fffffff);

	this->_words.back() = ~this->_words.back();

	this->fix(); // Remove redundant words

	//Check postcondition
	#ifdef DEBUG
		assert(!(this->_words.size() > 1) ||
									(this->_words.back() != 0 && this->_words.back() != -1));

		for (size_t i = 0; i < this->_words.size()-1; i++)
			assert((this->_words[i] & 0x80000000) == 0);
	#endif
}

void Integer::bitwise_and(const Integer & n) {

	/*Resize current Integer to the smallest size of blocks as the
	 * non-existing blocks of smaller Integer are supposed to be zero
	 * (bitwise AND with any other block will result zero)*/
	const size_t size = MIN(this->_words.size(), n._words.size());

	if (n._words.size() < this->_words.size() && !n.is_negative())
		/* On performing bitwise AND between the additional words from this and the
		 * non-existing words from n (considered having 0 value), all the words from
		 * this portion results 0 and it can be removed, so this is resized to n
		 * size whether n is positive. On n being negative, the additional words
		 * will be bitwised AND with 1's bits for the heading non-existing block of
		 * n, so the additional words remains with the same value in the resulting
		 * Integer.
		 */
		this->resize( n._words.size() );
	else if (n._words.size() > this->_words.size() && this->is_negative()) {
		/* On considering additional words on n being bitwised AND with 0's bits
		 * (for the non-existing words from this), the additional words can be discarded
		 * whether this is positive. On this being negative, the additional words on
		 * n are considered to be bitwised AND with 1's bits, so the additional
		 * words on n are appended to the resulting Integer.
		 */
		this->resize(n._words.size());

		for (size_t i = size; i < n._words.size(); i++)
			this->_words[i] = n._words[i];
	}

	//Perform bitwise AND word by word
	for (size_t i=0; i < size; i++)
		this->_words[i] &= n._words[i];

	this->fix(); //Remove redundant words at begining
}

void Integer::bitwise_or(const Integer & n) {

	/*The exceeding blocks of longest integer do not require to be procesed as the
	* non-existing blocks of the smallest integer does not compute in the final
	*	result*/
	size_t size = MIN(this->_words.size(), n._words.size());
	bool sign = this->is_negative() | n.is_negative();
	size_t i;

	//Perform bitwise OR block by block
	for (i = 0; i < size; i++)
		this->_words[i] |= (n._words[i] & 0x7fffffff);

	/* If n is longer and this is positive, the additional words of n are directly
	 *	removed as the non-existing words from n are considered to have 0 value,
	 *	otherwise, if n is shorter and negative, the non-existing blocks are considered
	 *	to have all bits at 1 and the exceeding words from this are redundant.
	*/
	if (this->_words.size() < n._words.size() && !this->is_negative() ||
						(this->_words.size() > n._words.size() && n.is_negative())) {
		this->resize(n._words.size());

		//Copy the rest of blocks whether n is longer
		for (;i < n._words.size(); i++)
			this->_words[i] = n._words[i];
	}

	// bitwise sign bit
	if (sign)
		this->_words.back() |= 0x80000000;

	this->fix(); // Remove redundant words if still needed

	//Check postcondition
	#ifdef DEBUG
		assert(!(this->_words.size() > 1) ||
									(this->_words.back() != 0 && this->_words.back() != -1));

		for (size_t i = 0; i < this->_words.size()-1; i++)
			assert((this->_words[i] & 0x80000000) == 0);
	#endif

}

void Integer::bitwise_shift_left(size_t d) {

	size_t f_shift = d / 31;
	size_t p_shift = d % 31;
	int64_t aux;

	//Partial block shifting is carried out
	if (p_shift>0) {

		int i = _words.size()-1;

		//Shift the most significant block
		aux = _words.back();
		aux <<= p_shift;
		_words[i] = aux & (int32_t)0x7fffffff;
		aux >>= 31;

		//Store the shifted block if is not redundant
		if (aux != 0 && aux != -1) {
			this -> resize(this->_words.size() + 1);
			this -> _words.back() = (aux & (int32_t)0x7fffffff);
		}

		if (aux<0)
			this -> _words.back() |= 0x80000000;

		i--;

		//Shift the rest of blocks
		for (;i>=0; i--) {

			aux = _words[i];
			aux <<= p_shift;

			//Split the shifted integer into two 32 bits integer excluding the sign bit
			_words[i] = aux & (int32_t)0x7fffffff;
			aux >>= 31;

			_words[i + 1] += aux & (int32_t)0x7fffffff;
		}

	}

	//Add extra required zero blocks at last
	if (f_shift > 0 && _words.back() != 0) {
		_words.insert(_words.begin(), f_shift, 0);
	}

	this->fix(); // Remove redundant words

	//Check postcondition
	#ifdef DEBUG
	assert(!(this->_words.size() > 1) || (this->_words.back() != 0 &&
																									this->_words.back() != -1));

		for (size_t i = 0; i < this->_words.size()-1; i++)
			assert((this->_words[i] & 0x80000000) == 0);
	#endif

}

void Integer::bitwise_shift_right(size_t d) {

	//Compute the full blocks to be removed and the partial shift on every block
	size_t f_shift = d /31;
	size_t p_shift = d % 31;
	int64_t aux;

	//Remove full blocks shifted to the right
	if (f_shift>0) {
		_words.erase(_words.begin(),_words.begin() + MIN(f_shift,_words.size()));
	}

	//Perform partial shifting on each block
	if (p_shift>0 && _words.empty()==false) {

		//Shift in the first block
		if (_words.size() == 1) {
			aux = _words[0];
			aux >>= p_shift;
			_words[0] = aux;
		}
		else {
			aux = _words[0];
			aux >>= p_shift;
			_words[0] = aux & 0x7fffffff;
		}

		//Shift in the rest of block
		for (size_t i=1;i<_words.size();i++) {
				aux = _words[i];
				aux <<= (31 - p_shift);

				//Split 64-bit auxiliar block shifted into two 32-bits blocks integers
				_words[i-1] += (aux & (int32_t)0x7fffffff);
				aux>>=31;
				_words[i] = aux & (int32_t)0x7fffffff;
		}

		//Add negative sign to the most signicant block if required
		if (aux<0)
			_words.back() |= 0x80000000;

	}

	fix(); //Remove redundant blooks

}

int Integer::compare(const Integer & n) const {

	//Resolve trivial cases
	if (this->_words.size() > n._words.size())
			return this->is_negative()? -1: 1;
	else if (this->_words.size() < n._words.size())
			return n.is_negative()? 1: -1;
	else if (this->is_negative() && !n.is_negative())
			return -1;
	else if (!this->is_negative() && n.is_negative())
			return 1;
	else {
			/*Both integers have same number of words and same sign and
			 comparison can be performed word by word */
			for (int i = this->_words.size() - 1; i>=0; i--) {

				if (this->_words[i] > n._words[i])
					return 1;
				else if (this->_words[i] < n._words[i])
					return -1;
			}

			//All the words are equal so Integers have the same value
			return 0;
	}

}

void Integer::resize(size_t n) {

		//Precondition
		#ifdef DEBUG
			assert(n > 0);
		#endif

		size_t org_size = this -> _words.size();
		int32_t sign = !_words.empty()? (_words.back() & 0x80000000) : 0;

		if (sign) {
			_words.back() &= 0x7fffffff;

			// Determines whether the sign should be kept when size is decreased
			if (org_size > n) {

				for (size_t i = n; i < org_size; i++) {
					if (this->_words[i] != 0x7fffffff) {
						sign = (int32_t)0;
						break;
					}
				}

			}

		}

		_words.resize(n, sign? INT32_MAX: 0);

		//Place the bit sign into the new most significant block
		if (sign)
				_words.back() |= sign;

		//Postcondition
		#ifdef DEBUG
			assert(_words.size() == n);
			for(size_t i = 0; i < _words.size() - 1; i++)
				assert(_words[i] >= 0);

			assert(!sign || _words.back() <= 0);
		#endif
}

void Integer::string_to_integer(const std::string & n) {

	this->reset();

	Integer pow_ten(1); //powers of 10 auxiliar integer
	Integer aux;

	for (int i = n.length() - 1; i>=0; i--) {

		if (n[i] >= '0' && n[i] <= '9') {

			//Read integer decimal digit by decimal digit
			aux.multiply(pow_ten, n[i] - '0');
			this->add(*this, aux);

			pow_ten.multiply(pow_ten,10);

		}
		else if (i == 0 && n[i]=='-') {
			//Read negative sign
			this->negative();
		}
		else {
			// Not valid character is found
			std::string message("Input string ");
			message += '"';
			message += n;
			message += '"';
			message += " does not contain valid digits";

			throw std::invalid_argument(message);
		}
	}

	//Postcondición
	#ifdef DEBUG
		assert(this->getString() == n || this->is_zero());
	#endif

	return;
}

void Integer::integer_to_string(std::string & s) const{

	std::string string_aux;
	s.clear();

	Integer aux(*this);
	Integer r;
	Integer ten(10);

	//Work with the positive representation of integer
	if (this->is_negative())
		aux.negative();

	//Retrieve every digit by succesive divisions by 10
	aux.divide(ten, aux, r);

	while (aux.is_zero() == false) {
		string_aux += (int)r + '0';
		aux.divide(ten,aux,r);
	}

	// Integer value is 0
	if (string_aux.empty() || !r.is_zero())
		string_aux += (int)r + '0';

	if (this->is_negative())
		string_aux += '-';

	//Postprocess to obtain the actual decimal representation of the Integer.
	for (int i = string_aux.length()-1; i>=0; i--)
		s.push_back(string_aux[i]);

}

const std::string Integer::getBinary(char delimit) const {

		std::string result;
		uint32_t aux;

		//Write sign bit
		if (this->is_negative())
			result += "1|";
		else
			result += "0|";

		//Write bits of each blocks
		for (int i = _words.size() - 1; i >= 0; i--) {

				aux = 0x40000000; //32-bit integer having bit 1 at index 31.

				for (int t = 30; t >= 0; t--) {

					//Write rest of bits
					if ((_words[i] & aux) > 0)
						result += '1';
					else
						result += '0';

					aux >>= 1;
				}

				//Write word-splitter char
				if (i > 0 && delimit != '\0')
					result += delimit;

		}

		return result;
}

size_t Integer::findFirstOne() const {

		/*If negative, the most significant bit 1 is placed on the 31th bit of
		 *most significant block*/
		if (is_negative())
			return _words.size() * N_BITS_PER_WORD;

		int32_t aux = _words.back();
		size_t index_aux = 0;

		/*Find most significant bit 1 on the most significant value by performing
		 *sucesive shifts*/
		while (aux != 0) {
				aux >>= 1;
				index_aux ++;
		}

		//Add all the previous blocks bits
		return index_aux + ((_words.size()-1) * N_BITS_PER_WORD ) -1;
}

void Integer::set_random_value(size_t n_bits_max) {

	// Generates the number of bits to generate
	size_t n_bits = Integer::_random_engine() % n_bits_max + 1;

	// Allocate required number of words to storage
	const size_t remain_bits = n_bits % N_BITS_PER_WORD;
	const size_t n_blocks = (n_bits / N_BITS_PER_WORD) + (size_t)(remain_bits> 0);

	this->resize(n_blocks);

	//Generates pseudo-random words
	const uint32_t word_val_limit (0x80000000);

	// Required to remove leftover bits on most significant word.
	uint32_t first_word_mask (1);
	first_word_mask = (first_word_mask << remain_bits) - 1;

	for (size_t i = 0; i < n_blocks; i++) {
		this->_words[i] = Integer::_random_engine() % word_val_limit;
	}

	this->_words.back() &= first_word_mask;
	this->fix(); // Remove redundant word

	// Check postcondition
	#ifdef DEBUG
		assert(this->n_bits() <= n_bits);
		assert(this->_words.size() <= n_blocks);

		for (size_t i = 0; i < this->_words.size(); i++)
			assert(this->_words[i] >= 0);
	#endif
}

std::istream & operator>>(std::istream & stream, Integer & n) {

	Integer::Stream_read_state state = Integer::SKIP;
	std::string aux;
	char c;

	while ((c = stream.get())) {

		/*Discard spaces and end-of-line on the stream until digit,
		 *EOF or null character is found*/
		if (c == '\n' || c == EOF)
			break;

		// SKIP state: Discard until a numeric character or the minus character is found.
		if (state == Integer::SKIP)
			if (c == '-' || c >= '0' && c <= '9')
				state = Integer::READ_MINUS;

		// READ_MINUS state: Read initial minus character and remaining digits.
		if (state == Integer::READ_MINUS)
			if (c == '-' || c >= '0' && c <= '9')
				state = Integer::READ_DIGIT;
			else
				break; // End of reading

		// READ_DIGIT state: Read consequent digits until non-numeric character is found
		if (state == Integer::READ_DIGIT)
			if (c < '0' && c > '9')
				break; // End reading from stream

		aux += c;
	}

	n = aux;
	return stream;
}

long long int Integer::getInt() const {

	//Check precondition
	if (this->_words.size() * sizeof(int32_t) >= sizeof(long long int)) {
		throw integer_conversion_error();
	}

	long long int ret(0LL);
	long long int aux;

	const size_t length = this->_words.size();

	// Writes the block into a long long int type until max capacity is reachen
	for (size_t i=0; i < length; i++) {

		aux = this->_words[i];
		aux <<= (31*i);

		ret += aux;
	}

	//Check postcondition
	#ifdef DEBUG
		assert(this->compare(ret) == 0);
	#endif

	return ret;
}
