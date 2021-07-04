# Arbitrary-sized two's complement binary Big Integer

Implementation of an Arbitrary-sized Big Integer class for C++. Integer class stores a variable-sized two's complement binary integer.

⚠️ Experimental implementation still under construction.

---

### Contents

* [Requisites](#Requisites)
* [Installation](#Installation)
* [Quick overview](#quick-overview)
* [Unit Tests](#unit-test)

---

## Requisites

* C++ compiler with C++20 support.
* CMake v2.8.12 or higher (optional but required to run the unit tests).
* [GoogleTest](https://github.com/google/googletest) (Only required to run the unit tests).


## Installation

1. Clone this repository inside your project.
2. Include `integer/integer.hpp` in your source codes where you'll use this class.
```
#include "integer/integer.hpp"
```
3. Compile and link `integer/integer.cpp` join to the rest of your sources.


### Quick overview

#### Initialization

Integer value can be created through **constructor** or **assignment operator** in different ways:

###### Constructors

```
Integer a;                                                   //Construct to default value 0
Integer b(132745);                                           //Construct from int
Integer c(8579834591_I);                                     //Construct from long
Integer d("16953955465787689754106889232325959114070");      //Construct from string
Integer e(1427247692705959881058285969449495136382746496_I); //Construct from custom literal
```

###### Assignment operator

```
Integer a = 132745;                                             //Assign value from int
Integer b = 8579834591_I;                                       //Assign value from long
Integer c = "16953955465787689754106889232325959114070";        //Assign value from string
Integer d = 1427247692705959881058285969449495136382746496_I;   //Assign value from another Integer object
```

#### Arithmetic operators

* ###### `+`, `-`, `*`, `/`, `%` arithmetic operators

```
// a, b, c, d, e and f Integer objects
// Integer left operand
a + 483486;
b - 4631134671L;
c * "78913516461356648746";
d / 13654879431548612365867891321145642583678935_I;
e % f;

// Integer right operand
483486 + a;
4631134671L - b;
"78913516461356648746" * c;
13654879431548612365867891321145642583678935_I / d;
f % e;
```

* ###### `+=`, `-=`, `*=`, `/=`, `%=` arithmetic-assignment operators

```
a += 483486;
b -= 4631134671L;
c *= "78913516461356648746";
d /= 13654879431548612365867891321145642583678935_I;
e %= f;
```

* ###### `++`, `--` prefix and postfix operators

```
// a and b Integer objects

a++;  //++ Postfix operator
++a;  //++ Prefix operator

b--;  //-- Postfix operator
--b;  //-- Prefix operator
```

* ###### `+`, `-` unnary operators

```
+a;   //Returns Integer a value
-b;   //Returns Integer b negative value
```

* ###### `pow` operation

Computes power of Integer to an exponent.

```
Integer pow_result1 = Integer::pow(42, 100);       //Computes 42^100
Integer pow_result2 = Integer::pow("78", "584");   //Computes 78^584
Integer pow_result3 = Integer::pow(1206_I, 970_I); //Computes 1206^970
```

* ###### `abs` operation

Computes absolute value of an Integer.

```
Integer abs_result1 = Integer::abs(19807040628566084398385987583_I);    //abs_result1 value is 19807040628566084398385987583
Integer abs_result2 = Integer::abs(-53646441131313478710003697788211_I) //abs_result2 value is 53646441131313478710003697788211
```

#### Comparison and Relational operators

* ###### `>`, `>=`, `<`, `<=`, `==`, `!=` relational operators

All the operators can be used to compare Integer
with other Integers, strings or single integral types integer such `int`, `long`, `long long`, `uint`, etc.

```
// Integers bigint1 and bigint2
// Integer at left operand
bigint1 > 789;
bigint1 >= 146401002164L;
bigint1 < 7871154644645LL;
bigint1 <= "78913516461356648746";
bigint1 == 16953955465787689754106889232325959114070_I;
bigint1 != bigint2;

// Integer at right operand
789 > bigint1;
146401002164L >= bigint1;
7871154644645LL < bigint1;
"78913516461356648746" <= bigint1;
16953955465787689754106889232325959114070_I == bigint1;
```

#### Logical operators

* ###### `~` unnary bitwise not operator

```
bigint1 = ~bigint1; //Computes bitwise not to bigint1 Integer
```

* ######  `&`, `&=`, `|`, `|=` bitwise AND / bitwise OR operators

```
  bigint1 = bigint1 & -5874;
  bigint2 &= 7644664L;

  bigint3 = bigint3 | "42360206598766163657616";
  bigint4 |= 137879131303364876841303_I;
```

* ######  `<<`, `<<=`, `>>`, `>>=` bitwise shift left / bitwise shift right operators

```
bigint1 = bigint1 << 2456;
bigint2 <<= 478;

bigint3 = bigint3 >> 16;
bigint4 >>= 47;
```

⚠️ Only single int types currently supported.

#### I/O operators

* ######  `<<` Extractor operator

Write Integer value to a output data stream.

```
std::cout << bigint1 << std::endl; //Write to the standard output
```

* ######  `>>` Insertor operator

Read an Integer value from input data stream.

```
std::cin >> bigint1; //Read from keyboard
```

#### Cast operators

* ######  `std::string` cast operator

Converts Integer value to a string.

```
std::string str;

str = (std::string) bigint1;
```

* ######  `long long` cast operator

Converts Integer value to a single integral type integer.

```
int a = (int) bigint1;             //Convert to int value
long b = (long) bigint2;           //Convert to long value
long long c = (long long) bigint3; //Convert to long long value
```

#### Helper functions

* ######  `is_negative()`.

Checks whether Integer holds a negative value

```
Integer bigint1 (-1550346240562184622430_I);
Integer bigint2 (4153484468713354886746874684864874864_I);

bigint1.is_negative(); //Returns true
bigint2.is_negative(); //Returns false
```

* ######  `is_zero()`.

Checks whether Integer holds zero value

```
Integer bigint1(0), bigint2(2);

bigint1.is_zero(); //Returns true
bigint2.is_zero(); //Returns false
```

* ######  `is_even()`.

Checks whether Integer holds a even value

```
Integer bigint1 (7871154644645LL);
Integer bigint2 ("84641364662020014094");

bigint1.is_even(); // Returns false
bigint2.is_even(); // Returns true
```

* ######  `n_bits()`.

Actual number of bits holding the Integer value

```
Integer bigint1 (4679467203899414867613_I), bigint2 (-1343516705_I);

bigint1.n_bits(); //Returns 72
bigint2.n_bits(); //Returns 32
```

* ######  `[]` Access operator.

Get decimal digit located at a given index:

```
Integer bigint1(135492_I);

bingint1[0]; //Returns 2
bingint1[1]; //Returns 9
bingint1[2]; //Returns 4
bingint1[3]; //Returns 5
bingint1[4]; //Returns 3
bingint1[5]; //Returns 1
```

#### Random

* ###### `rand()`.

Generates random Integer with desired max number of bits. Default 1000.

```
Integer bigint1 = Integer::rand();    //Generates random Integer having 1000 bits as max
Integer bigint2 = Integer::rand(500); //Generates random Integer having 500 bits as max
```

* ###### `set_random_value()`.

Fills Integer with random value with a given max number of bits.

```
bigint1.set_random_value(); //Fills bigint1 with random value having 1000 bits as max
bigint2.set_random_value(500); //Fills bigint1 with random value having 500 bits as max
```

* ###### `get_random_engine()`, `set_random_engine()` do respectively get and set the random engine used for random Integer generation.


## Unit test

Unit Tests sources can be located at `unit_test` folder. More documentation about compilation and running can be found inside.
