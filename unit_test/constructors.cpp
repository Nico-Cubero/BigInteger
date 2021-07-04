#include <gtest/gtest.h>
#include <list>
#include <limits>
#include <string>
#include <stdexcept>
#include <integer/integer.hpp>

TEST(Constructor, Empty_constructor) {
  Integer a;
  EXPECT_EQ(a, 0);
  EXPECT_EQ((int)a, 0);
  EXPECT_EQ(a, Integer(a)); // Test copy constructor
}

TEST(Constructor, Int_constructor) {
  std::default_random_engine int_gen(1);
  std::list<int> integers; /*= {0, -1, 1, 153434, -153434, 1000, -1000, 876887,
                              -876887, 343486, -343486, 45, -45, 896, -896, 5,
                              -5, 7364789, -7364789, 41239, -41239}*/
  integers.push_back(0);
  integers.push_back(1);
  integers.push_back(-1);

  // Fill list with random int values
  for (size_t i = 0; i < 10000; i++) {
    integers.push_back(int_gen() % std::numeric_limits<int>::max());
    integers.push_back(-integers.back());
  }

  for (std::list<int>::iterator it = integers.begin(); it != integers.end(); it++) {
    Integer i(*it);
    EXPECT_EQ(i, *it);
    EXPECT_EQ((int)i, *it);
    EXPECT_EQ(i, Integer(*it)); // Test copy constructor
  }

}

TEST(Constructor, Long_constructor) {
  std::default_random_engine int_gen(1);
  std::list<long> longs;

  longs.push_back(0L);
  longs.push_back(1L);
  longs.push_back(-1L);

  // Fill list with random int values
  for (size_t i = 0; i < 10000; i++) {
    longs.push_back(int_gen() % std::numeric_limits<long>::max());
    longs.push_back(-longs.back());
  }

  for (std::list<long>::iterator it = longs.begin(); it != longs.end(); it++) {
    Integer i(*it);
    EXPECT_EQ(i, *it);
    EXPECT_EQ((long)i, *it);
    EXPECT_EQ(i, Integer(*it)); // Test copy constructor
  }

}

TEST(Constructor, String_constructor) {
  std::default_random_engine int_gen(1);
  std::list<std::string> str;

  str.push_back("0");
  str.push_back("1");
  str.push_back("-1");

  // Fill list with random string generated values
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

  for (std::list<std::string>::iterator it = str.begin(); it != str.end(); it++) {
    Integer i(*it);
    EXPECT_EQ(i, *it);
    EXPECT_EQ((std::string)i, *it);
    EXPECT_EQ(i, Integer(*it)); // Test copy constructor
  }

}

TEST(Constructor, String_constructor_extra_cases) {
  // On not valid string, Integer value must be 0
  EXPECT_EQ(Integer(""), 0);
  EXPECT_THROW(Integer("Hola"), std::invalid_argument);
}

TEST(Constructor, literal_constructor) {

  /*
   *  Test copy constructor for Integer literals. 
  */

  std::list<Integer> integers = {0_I, 1_I, -1_I, 450_I, -450_I, 68_I, -68_I, 7831_I,
                                  -7831_I, 7951531348105475684_I,
                                  -7951531348105475684_I,
                                  4153484468713354886746874684864874864_I,
                                  -4153484468713354886746874684864874864_I,
                                5687841153454544684_I, -5687841153454544684_I,
                              134534878169787100351478911654876113458786544645_I,
                            -134534878169787100351478911654876113458786544645_I};

  std::list<std::string> integers_str = {"0", "1", "-1", "450", "-450", "68", "-68", "7831",
                                  "-7831", "7951531348105475684",
                                  "-7951531348105475684",
                                  "4153484468713354886746874684864874864",
                                  "-4153484468713354886746874684864874864",
                                "5687841153454544684", "-5687841153454544684",
                              "134534878169787100351478911654876113458786544645",
                            "-134534878169787100351478911654876113458786544645"};

  std::list<Integer>::iterator it = integers.begin();
  std::list<std::string>::iterator jt = integers_str.begin();

  for (;it != integers.end();
      it++, jt++) {
        EXPECT_EQ(*it, *jt);
        EXPECT_EQ(*it, Integer(*jt));
      }
}
