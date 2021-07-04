#include <gtest/gtest.h>
#include <list>
#include <random>
#include <string>
#include <stdexcept>
#include <integer/integer.hpp>

///////// ASSESMENT OPERATOR /////////

TEST(Equal_operator, equality_int_literal) {
  std::default_random_engine int_gen(1);
  Integer::set_random_engine(int_gen);
  std::list<int> integers;

  integers.push_back(0);
  integers.push_back(1);
  integers.push_back(-1);

  // Fill list with random int values
  for (size_t i = 0; i < 10000; i++) {
    integers.push_back(int_gen() % std::numeric_limits<int>::max());
    integers.push_back(-integers.back());
  }

  // Check whether the Integer values is equal to the int
  for (std::list<int>::iterator it = integers.begin(); it != integers.end(); it++) {
    Integer i;
    i.set_random_value();

    i = *it;
    EXPECT_EQ(i, *it);
  }

}

TEST(Equal_operator, equality_long_literal) {
  std::default_random_engine int_gen(1);
  Integer::set_random_engine(int_gen);
  std::list<long> longs;

  longs.push_back(0L);
  longs.push_back(1L);
  longs.push_back(-1L);

  // Fill list with random int values
  for (size_t i = 0; i < 10000; i++) {
    longs.push_back(int_gen() % std::numeric_limits<long>::max());
    longs.push_back(-longs.back());
  }

  // Check whether the Integer values is equal to the long
  for (std::list<long>::iterator it = longs.begin(); it != longs.end(); it++) {
    Integer i;
    i.set_random_value();

    i = *it;
    EXPECT_EQ(i, *it);
  }

}

TEST(Equal_operator, equality_string_literal) {
  std::default_random_engine int_gen(1);
  Integer::set_random_engine(int_gen);
  std::list<std::string> str;

  str.push_back("0");
  str.push_back("1");
  str.push_back("-1");

  // Fill list with random string values
  for (size_t i = 0; i < 500; i++) {

    //Generate string by random digits
    str.push_back(std::string());
    size_t n_char = int_gen() % 100 + 1;

    // First digit must be [1-9]
    str.back() += int_gen() % 9 + '1';

    // Fill rest of digits by random 0-9 characters
    for (size_t j = 0; j < n_char - 1; j++)
      str.back() += int_gen() % 10 + '0';

    // Include negative value
    str.push_back('-' + str.back());
  }

  // Check whether the Integer is equal to the value hold by str
  for (std::list<std::string>::iterator it = str.begin(); it != str.end(); it++) {
    Integer i;
    i.set_random_value();

    i = *it;
    EXPECT_EQ(i, *it);
  }

}

TEST(Equal_operator, equality_string_literal_extra_cases) {

  std::default_random_engine int_gen(1);
  Integer::set_random_engine(int_gen);

  // On not valid string, Integer value must be 0
  Integer i;

  // null string
  i.set_random_value();
  i = "";
  EXPECT_EQ(i, 0);

  // Not valid string
  i.set_random_value();
  EXPECT_THROW( {i = "Hola";}, std::invalid_argument);
}

TEST(Equal_operator, equality_integer_positive_values) {
  std::default_random_engine int_gen(6987);
  Integer orig, cop;

  // Generate random Integer values for orig and test equality to cop
  for (size_t i = 0; i < 10000; i++) {
    orig.set_random_value();
    cop = orig;
    EXPECT_EQ(orig, cop);
  }
}

TEST(Equal_operator, equality_integer_negative_values) {

  std::default_random_engine int_gen(8697);
  Integer::set_random_engine(int_gen);
  Integer orig, cop;

  // Generate random Integer values for orig and test equality to cop
  for (size_t i = 0; i < 10000; i++) {
    orig.set_random_value();
    cop = -orig;
    EXPECT_EQ(orig, -cop);
  }
}

TEST(Equal_operator, equality_integer_zero_value) {
  /* Testing assignment operator when second operand is 0
  */
  Integer a, e;
  std::default_random_engine int_gen(53);
  Integer::set_random_engine(int_gen);

  e.set_random_value();

  e = a;
  EXPECT_EQ(e, a);
  EXPECT_EQ(e, 0);
}
