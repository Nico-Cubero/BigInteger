#include <gtest/gtest.h>
#include <list>
#include <string>
#include <integer/integer.hpp>

TEST(Is_negative, zero) {

  /*Test of is_negative observer method. Zero value int tested
  */
  Integer zero;
  EXPECT_EQ(zero.is_negative(), false);
}

TEST(Is_negative, random_values) {

  /*Test of is_negative observer method. Random Integers generated through string
   * by using Integer ran generated through string filled by random digits
  */

  std::default_random_engine int_gen(10);
  Integer::set_random_engine(int_gen);
  std::list<std::string> str;

  str.push_back("1");
  str.push_back("-1");

  // Fill list with random int values
  for (size_t i = 0; i < 500; i++) {
    //Generate random string containing digits
    str.push_back(std::string());
    size_t n_char = int_gen() % 100 + 1;

    // First digit must be [1-9]
    str.back() += int_gen() % 9 + '1';

    // Fill rest of digits by random 0-9 characters
    for (size_t j = 0; j < n_char - 1; j++)
      str.back() += int_gen() % 10 + '0';

    // Append the negative representation
    str.push_back('-' + str.back());
  }

  for (std::list<std::string>::iterator it = str.begin(); it != str.end(); it++) {
    Integer ti(*it);
    EXPECT_EQ(ti.is_negative(), (*it)[0] == '-'? true: false);
  }
}

TEST(Is_zero, zero_values) {

  /*Test of is_zero observer method.
  */

  // Integer zero value generated through default constructor
  Integer zero_default_constr;
  EXPECT_EQ(zero_default_constr.is_zero(), true);

  // Integer zero value generated through literal
  EXPECT_EQ((0_I).is_zero(), true);

  // Integer zero value generated through int constructor
  Integer zero_int_constr(0);
  EXPECT_EQ(zero_int_constr.is_zero(), true);

  // Integer zero value generated through string constructor
  Integer zero_str_constr("0");
  EXPECT_EQ(zero_str_constr.is_zero(), true);
}

TEST(Is_zero, random_non_zero_values) {

  /*Test of is_zero observer method. Random Integers with non-zero values
   * generated through string by using Integer generated through string
   * filled by random digits
  */

  // Test random generated integers having non-zero value
  std::default_random_engine int_gen(27);
  Integer::set_random_engine(int_gen);
  std::list<std::string> str;

  // Fill list with random int values
  for (size_t i = 0; i < 500; i++) {
    //Generate random string containing digits
    str.push_back(std::string());
    size_t n_char = int_gen() % 100 + 1;

    // First digit must be [1-9]
    str.back() += int_gen() % 9 + '1';

    // Fill rest of digits by random 0-9 characters
    for (size_t j = 0; j < n_char - 1; j++)
      str.back() += int_gen() % 10 + '0';

    // Append the negative representation
    str.push_back('-' + str.back());
  }

  for (std::list<std::string>::iterator it = str.begin(); it != str.end(); it++) {
    Integer ti(*it);
    EXPECT_EQ(ti.is_zero(), false);
  }
}

TEST(Check_even, even_values) {

  /*Test of is_even observer method. Random Integers generated through string
   * by using Integer ran generated through string filled by random digits
  */

  // Test random generated integers having even non-zero value
  std::default_random_engine int_gen(27);
  Integer::set_random_engine(int_gen);
  std::list<std::string> str;
  int aux;

  // Fill list with random int values
  for (size_t i = 0; i < 250; i++) {
    //Generate random string containing digits
    str.push_back(std::string());
    size_t n_char = int_gen() % 100 + 1;

    // First digit must be in [1-9]
    str.back() += int_gen() % 9 + '1';

    // Fill inner digits by random 0-9 characters
    for (size_t j = 0; j < n_char - 2; j++)
      str.back() += int_gen() % 10 + '0';

    // Add last digit with even value
    aux = int_gen() % 9;
    aux = aux % 2 == 1 ? aux + 1: aux;
    str.back() += aux + '0';

    // Append the negative representation
    str.push_back('-' + str.back());
  }

  for (std::list<std::string>::iterator it = str.begin(); it != str.end(); it++) {
    Integer ti(*it);
    EXPECT_EQ(ti.is_even(), true) <<
              "Integer value: " << ti << ". String: " << *it << '\n';
  }

}

TEST(Check_even, odd_values) {

  /*Test of is_even observer method. Random Integers generated through string
   * by using Integer ran generated through string filled by random digits
  */

  // Test random generated integers having even non-zero value
  std::default_random_engine int_gen(27);
  Integer::set_random_engine(int_gen);
  std::list<std::string> str;
  int aux;

  // Test random generated integers having odd non-zero value
  for (size_t i = 0; i < 250; i++) {
    //Generate random string containing digits
    str.push_back(std::string());
    size_t n_char = int_gen() % 100 + 1;

    // First digit must be [1-9]
    str.back() += int_gen() % 9 + '1';

    // Fill inner digits by random 0-9 characters
    for (size_t j = 0; j < n_char - 2; j++)
      str.back() += int_gen() % 10 + '0';

    // Add last digit with odd value
    aux = int_gen() % 9 + 1;
    aux = aux % 2 == 1 ? aux: aux - 1;
    str.back() += aux + '0';

    // Append the negative representation
    str.push_back('-' + str.back());
  }

  for (std::list<std::string>::iterator it = str.begin(); it != str.end(); it++) {
    Integer ti(*it);
    EXPECT_EQ(ti.is_even(), false) <<
              "Integer value: " << ti << ". String: " << *it << '\n';
  }

}

TEST(Access_operator, access_operator) {

  /*Test of access operator. Random Integers generated through string
   * by using Integer generated through
   * string filled by random digits
  */

  // Test 0 is not negative
  std::default_random_engine int_gen(1234);
  Integer::set_random_engine(int_gen);
  std::list<std::string> str;

  str.push_back("0");
  str.push_back("1");
  str.push_back("-1");

  // Fill list with random int values
  for (size_t i = 0; i < 500; i++) {
    //Generate random string containing digits
    str.push_back(std::string());
    size_t n_char = int_gen() % 100 + 1;

    // First digit must be [1-9]
    str.back() += int_gen() % 9 + '1';

    // Fill rest of digits by random 0-9 characters
    for (size_t j = 0; j < n_char - 1; j++)
      str.back() += int_gen() % 10 + '0';

    // Append the negative representation
    str.push_back('-' + str.back());
  }

  for (std::list<std::string>::iterator it = str.begin(); it != str.end(); it++) {
    Integer ti(*it);

    // Test all the digits can be retrieved through [] operator
    for (size_t i = 0; i < it->size(); i++){
      EXPECT_EQ(ti[i], (*it)[it->size() - i - 1] - '0');
    }

    // Out of boundaries, no digit can be retrievable
    EXPECT_THROW(ti[it->size()], std::out_of_range);
  }
}
