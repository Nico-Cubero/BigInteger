#include <gtest/gtest.h>
#include <list>
#include <string>
#include <fstream>
#include <integer/integer.hpp>

///////// EXTRACTOR AND INSERTOR OPERATORS /////////
TEST(Extractor_Insertor_operator, known_cases) {
  /*
   * Test saving and reading numbers from file
  */
  std::list<Integer> sample = {0_I, 1_I, -1_I, 29873614_I, -29873614_I,
                               4294967296_I, -4294967296_I, 2147483548_I,
                              -2147483548_I, 18446744073709551616_I,
                              -18446744073709551616_I, 9223363323275862454_I,
                              -9223363323275862454_I, 79228162514264337593543950336_I,
                              -79228162514264337593543950336_I, 39614081127153423112123300301_I,
                              -39614081127153423112123300301_I, 2135987035920910082395021706169552114602704522356652769947041607822219725780640550022962086936576_I,
                              -2135987035920910082395021706169552114602704522356652769947041607822219725780640550022962086936576_I,
                              1067993517960455041197510853084776057301352261178326384973520803901325349374833476231681043468289_I,
                              -1067993517960455041197510853084776057301352261178326384973520803901325349374833476231681043468289_I};

  std::fstream f;
  Integer aux;
  char aux_str[1024];

  for(std::list<Integer>::iterator it = sample.begin(); it != sample.end(); it++) {
    // Write Integer value on file
    f.open("/tmp/Integer_extractor_insertor_operator", std::ios::out);

    if (!f)
     GTEST_SKIP();

    f << *it << std::endl;

    f.close();

    // Read Integer value from file
    f.open("/tmp/Integer_extractor_insertor_operator", std::ios::in);

    if (!f)
     GTEST_SKIP();

    f >> aux;

    EXPECT_EQ(*it, aux);

    // Read Integer value as string and compare string representation
    f.seekg(0, std::ios::beg);
    f.getline(aux_str, 1024);
    EXPECT_EQ(std::string(aux_str), (std::string)aux);

    f.close();
  }
}

TEST(Extractor_Insertor_operator, random_values) {
  /*
   * Test reading and writing random Integer values on file
  */
  std::default_random_engine int_gen(37824);
  Integer::set_random_engine(int_gen);

  std::fstream f;
  Integer aux;
  std::list<Integer> random_values;

  // Generate random Integer values
  for (size_t test = 0; test < 500; test++) {
    random_values.push_back(Integer::rand());
    random_values.push_back(-random_values.back());
  }

  // Write random Integer values on file
  f.open("/tmp/Integer_extractor_insertor_operator_random_values", std::ios::out);

  if (!f)
    GTEST_SKIP();

  for (std::list<Integer>::iterator it = random_values.begin(); it != random_values.end(); it++) {
      f << *it << std::endl;
  }

  f.close();

  // Test read the written Integer
  f.open("/tmp/Integer_extractor_insertor_operator_random_values", std::ios::in);

  if (!f)
    GTEST_SKIP();

  for (std::list<Integer>::iterator it = random_values.begin(); it != random_values.end(); it++) {
      f >> aux;
      EXPECT_EQ(*it, aux);
  }

  f.close();
}
