#include <gtest/gtest.h>
#include <integer/integer.hpp>

///////// COMPARISON OPERATORS /////////

TEST(Greater_operator, random_values) {
  /*
   * Test greater and greater/equal operator by using random Integer values
   */
  std::default_random_engine int_gen(6187);
  Integer::set_random_engine(int_gen);

  for (size_t test = 0; test < 10000; test++) {
    Integer a, b;

    a.set_random_value();
    b = a + 1 + Integer::rand(); // In this way, a < b

    EXPECT_EQ(b > a, true) << "b=" << b << ", a=" << a << std::endl;
    EXPECT_EQ(b >= a, true) << "b=" << b << ", a=" << a << std::endl;

    EXPECT_EQ(-b > a, false) << "b=" << b << ", a=" << a << std::endl;
    EXPECT_EQ(-b >= a, false) << "b=" << b << ", a=" << a << std::endl;

    EXPECT_EQ(b > -a, true) << "b=" << b << ", a=" << a << std::endl;
    EXPECT_EQ(b >= -a, true) << "b=" << b << ", a=" << a << std::endl;

    EXPECT_EQ(-b > -a, false) << "b=" << b << ", a=" << a << std::endl;
    EXPECT_EQ(-b >= -a, false) << "b=" << b << ", a=" << a << std::endl;
  }

}

TEST(Greater_operator, long_long_int_right_operand) {
  /*
   * Test greater than and greater than or equal operators having long long int
   * type at right operand
   */
   EXPECT_EQ(4294967295_I > 2294967295, true);
   EXPECT_EQ(4294967295_I >= 2294967295, true);
   EXPECT_EQ(-4294967295_I > 2294967295, false);
   EXPECT_EQ(-4294967295_I >= 2294967295, false);
   EXPECT_EQ(4294967295_I > -2294967295, true);
   EXPECT_EQ(4294967295_I >= -2294967295, true);
   EXPECT_EQ(-4294967295_I > -2294967295, false);
   EXPECT_EQ(-4294967295_I >= -2294967295, false);

   EXPECT_EQ(2294967295_I > 4294967295, false);
   EXPECT_EQ(2294967295_I >= 4294967295, false);
   EXPECT_EQ(2294967295_I > -4294967295, true);
   EXPECT_EQ(2294967295_I >= -4294967295, true);
   EXPECT_EQ(-2294967295_I > 4294967295, false);
   EXPECT_EQ(-2294967295_I >= 4294967295, false);
   EXPECT_EQ(-2294967295_I > -4294967295, true);
   EXPECT_EQ(-2294967295_I >= -4294967295, true);

   EXPECT_EQ(716978_I > 716978, false);
   EXPECT_EQ(716978_I >= 716978, true);
   EXPECT_EQ(-716978_I > 716978, false);
   EXPECT_EQ(-716978_I >= 716978, false);
   EXPECT_EQ(716978_I > -716978, true);
   EXPECT_EQ(716978_I >= -716978, true);
   EXPECT_EQ(-716978_I > -716978, false);
   EXPECT_EQ(-716978_I >= -716978, true);
}

TEST(Greater_operator, long_long_int_left_operand) {
  /*
   * Test greater than and greater than or equal operators having long long int
   * type at left operand
   */
   EXPECT_EQ(4294967295 > 2294967295_I, true);
   EXPECT_EQ(4294967295 >= 2294967295_I, true);
   EXPECT_EQ(-4294967295 > 2294967295_I, false);
   EXPECT_EQ(-4294967295 >= 2294967295_I, false);
   EXPECT_EQ(4294967295 > -2294967295_I, true);
   EXPECT_EQ(4294967295 >= -2294967295_I, true);
   EXPECT_EQ(-4294967295 > -2294967295_I, false);
   EXPECT_EQ(-4294967295 >= -2294967295_I, false);

   EXPECT_EQ(2294967295 > 4294967295_I, false);
   EXPECT_EQ(2294967295 >= 4294967295_I, false);
   EXPECT_EQ(2294967295 > -4294967295_I, true);
   EXPECT_EQ(2294967295 >= -4294967295_I, true);
   EXPECT_EQ(-2294967295 > 4294967295_I, false);
   EXPECT_EQ(-2294967295 >= 4294967295_I, false);
   EXPECT_EQ(-2294967295 > -4294967295_I, true);
   EXPECT_EQ(-2294967295 >= -4294967295_I, true);

   EXPECT_EQ(716978 > 716978_I, false);
   EXPECT_EQ(716978 >= 716978_I, true);
   EXPECT_EQ(-716978 > 716978_I, false);
   EXPECT_EQ(-716978 >= 716978_I, false);
   EXPECT_EQ(716978 > -716978_I, true);
   EXPECT_EQ(716978 >= -716978_I, true);
   EXPECT_EQ(-716978 > -716978_I, false);
   EXPECT_EQ(-716978 >= -716978_I, true);
}

TEST(Greater_operator, string_right_operand) {
  /*
   * Test greater than and greater than or equal operators having long long int
   * type at right operand
   */
   EXPECT_EQ(4399401354897945467876151264697_I > "5397481354897945467899151264", true);
   EXPECT_EQ(4399401354897945467876151264697_I >= "5397481354897945467899151264", true);
   EXPECT_EQ(-4399401354897945467876151264697_I > "5397481354897945467899151264", false);
   EXPECT_EQ(-4399401354897945467876151264697_I >= "5397481354897945467899151264", false);
   EXPECT_EQ(4399401354897945467876151264697_I > "-5397481354897945467899151264", true);
   EXPECT_EQ(4399401354897945467876151264697_I >= "-5397481354897945467899151264", true);
   EXPECT_EQ(-4399401354897945467876151264697_I > "-5397481354897945467899151264", false);
   EXPECT_EQ(-4399401354897945467876151264697_I >= "-5397481354897945467899151264", false);

   EXPECT_EQ(5397481354897945467899151264_I > "4399401354897945467876151264697", false);
   EXPECT_EQ(5397481354897945467899151264_I >= "4399401354897945467876151264697", false);
   EXPECT_EQ(5397481354897945467899151264_I > "-4399401354897945467876151264697", true);
   EXPECT_EQ(5397481354897945467899151264_I >= "-4399401354897945467876151264697", true);
   EXPECT_EQ(-5397481354897945467899151264_I > "4399401354897945467876151264697", false);
   EXPECT_EQ(-5397481354897945467899151264_I >= "4399401354897945467876151264697", false);
   EXPECT_EQ(-5397481354897945467899151264_I > "-4399401354897945467876151264697", true);
   EXPECT_EQ(-5397481354897945467899151264_I >= "-4399401354897945467876151264697", true);

   EXPECT_EQ(100976600013970343640064646346479797_I > "100976600013970343640064646346479797", false);
   EXPECT_EQ(100976600013970343640064646346479797_I >= "100976600013970343640064646346479797", true);
   EXPECT_EQ(100976600013970343640064646346479797_I > "-100976600013970343640064646346479797", true);
   EXPECT_EQ(100976600013970343640064646346479797_I >= "-100976600013970343640064646346479797", true);
   EXPECT_EQ(-100976600013970343640064646346479797_I > "100976600013970343640064646346479797", false);
   EXPECT_EQ(-100976600013970343640064646346479797_I >= "100976600013970343640064646346479797", false);
   EXPECT_EQ(-100976600013970343640064646346479797_I > "-100976600013970343640064646346479797", false);
   EXPECT_EQ(-100976600013970343640064646346479797_I >= "-100976600013970343640064646346479797", true);
}

TEST(Greater_operator, string_left_operand) {
  /*
   * Test greater than and greater than or equal operators having string
   * type at right operand
   */
   EXPECT_EQ("4399401354897945467876151264697" > 5397481354897945467899151264_I, true);
   EXPECT_EQ("4399401354897945467876151264697" >= 5397481354897945467899151264_I, true);
   EXPECT_EQ("-4399401354897945467876151264697" > 5397481354897945467899151264_I, false);
   EXPECT_EQ("-4399401354897945467876151264697" >= 5397481354897945467899151264_I, false);
   EXPECT_EQ("4399401354897945467876151264697" > -5397481354897945467899151264_I, true);
   EXPECT_EQ("4399401354897945467876151264697" >= -5397481354897945467899151264_I, true);
   EXPECT_EQ("-4399401354897945467876151264697" > -5397481354897945467899151264_I, false);
   EXPECT_EQ("-4399401354897945467876151264697" >= -5397481354897945467899151264_I, false);

   EXPECT_EQ("5397481354897945467899151264" > 4399401354897945467876151264697_I, false);
   EXPECT_EQ("5397481354897945467899151264" >= 4399401354897945467876151264697_I, false);
   EXPECT_EQ("5397481354897945467899151264" > -4399401354897945467876151264697_I, true);
   EXPECT_EQ("5397481354897945467899151264" >= -4399401354897945467876151264697_I, true);
   EXPECT_EQ("-5397481354897945467899151264" > 4399401354897945467876151264697_I, false);
   EXPECT_EQ("-5397481354897945467899151264" >= 4399401354897945467876151264697_I, false);
   EXPECT_EQ("-5397481354897945467899151264" > -4399401354897945467876151264697_I, true);
   EXPECT_EQ("-5397481354897945467899151264" >= -4399401354897945467876151264697_I, true);

   EXPECT_EQ("100976600013970343640064646346479797" > 100976600013970343640064646346479797_I, false);
   EXPECT_EQ("100976600013970343640064646346479797" >= 100976600013970343640064646346479797_I, true);
   EXPECT_EQ("100976600013970343640064646346479797" > -100976600013970343640064646346479797_I, true);
   EXPECT_EQ("100976600013970343640064646346479797" >= -100976600013970343640064646346479797_I, true);
   EXPECT_EQ("-100976600013970343640064646346479797" > 100976600013970343640064646346479797_I, false);
   EXPECT_EQ("-100976600013970343640064646346479797" >= 100976600013970343640064646346479797_I, false);
   EXPECT_EQ("-100976600013970343640064646346479797" > -100976600013970343640064646346479797_I, false);
   EXPECT_EQ("-100976600013970343640064646346479797" >= -100976600013970343640064646346479797_I, true);
}

TEST(Less_operator, random_values) {
  /*
   * Test less and less/equal operator by using random Integer values
   */
  std::default_random_engine int_gen(8617);
  Integer::set_random_engine(int_gen);

  for (size_t test = 0; test < 10000; test++) {
    Integer a, b;

    a.set_random_value();
    b = a + 1 + Integer::rand(); // In this way, a < b

    EXPECT_EQ(a < b, true) << "b=" << b << ", a=" << a << std::endl;
    EXPECT_EQ(a <= b, true) << "b=" << b << ", a=" << a << std::endl;

    EXPECT_EQ(a < -b, false) << "b=" << b << ", a=" << a << std::endl;
    EXPECT_EQ(a <= -b, false) << "b=" << b << ", a=" << a << std::endl;

    EXPECT_EQ(-b < -a, true) << "b=" << b << ", a=" << a << std::endl;
    EXPECT_EQ(-b <= -a, true) << "b=" << b << ", a=" << a << std::endl;
  }
}

TEST(Less_operator, long_long_int_right_operand) {
  /*
   * Test less than and less than or equal operators having long long int at
   *  right operand
   */
   EXPECT_EQ(2294967295_I < 4294967295, true);
   EXPECT_EQ(2294967295_I <= 4294967295, true);
   EXPECT_EQ(2294967295_I < -4294967295, false);
   EXPECT_EQ(2294967295_I <= -4294967295, false);
   EXPECT_EQ(-2294967295_I < 4294967295, true);
   EXPECT_EQ(-2294967295_I <= 4294967295, true);
   EXPECT_EQ(-2294967295_I < -4294967295, false);
   EXPECT_EQ(-2294967295_I <= -4294967295, false);

   EXPECT_EQ(4294967295_I < 2294967295, false);
   EXPECT_EQ(4294967295_I <= 2294967295, false);
   EXPECT_EQ(-4294967295_I < 2294967295, true);
   EXPECT_EQ(-4294967295_I <= 2294967295, true);
   EXPECT_EQ(4294967295_I < -2294967295, false);
   EXPECT_EQ(4294967295_I <= -2294967295, false);
   EXPECT_EQ(-4294967295_I < -2294967295, true);
   EXPECT_EQ(-4294967295_I <= -2294967295, true);

   EXPECT_EQ(716978_I < 716978, false);
   EXPECT_EQ(716978_I <= 716978, true);
   EXPECT_EQ(716978_I < -716978, false);
   EXPECT_EQ(716978_I <= -716978, false);
   EXPECT_EQ(-716978_I < 716978, true);
   EXPECT_EQ(-716978_I <= 716978, true);
   EXPECT_EQ(-716978_I < -716978, false);
   EXPECT_EQ(-716978_I <= -716978, true);
}

TEST(Less_operator, long_long_int_left_operand) {
  /*
   * Test less than and less than or equal operators having long long int at
   *  left operand
   */
   EXPECT_EQ(2294967295 < 4294967295_I, true);
   EXPECT_EQ(2294967295 <= 4294967295_I, true);
   EXPECT_EQ(2294967295 < -4294967295_I, false);
   EXPECT_EQ(2294967295 <= -4294967295_I, false);
   EXPECT_EQ(-2294967295 < 4294967295_I, true);
   EXPECT_EQ(-2294967295 <= 4294967295_I, true);
   EXPECT_EQ(-2294967295 < -4294967295_I, false);
   EXPECT_EQ(-2294967295 <= -4294967295_I, false);

   EXPECT_EQ(4294967295 < 2294967295_I, false);
   EXPECT_EQ(4294967295 <= 2294967295_I, false);
   EXPECT_EQ(-4294967295 < 2294967295_I, true);
   EXPECT_EQ(-4294967295 <= 2294967295_I, true);
   EXPECT_EQ(4294967295 < -2294967295_I, false);
   EXPECT_EQ(4294967295 <= -2294967295_I, false);
   EXPECT_EQ(-4294967295 < -2294967295_I, true);
   EXPECT_EQ(-4294967295 <= -2294967295_I, true);

   EXPECT_EQ(716978 < 716978_I, false);
   EXPECT_EQ(716978 <= 716978_I, true);
   EXPECT_EQ(716978 < -716978_I, false);
   EXPECT_EQ(716978 <= -716978_I, false);
   EXPECT_EQ(-716978 < 716978_I, true);
   EXPECT_EQ(-716978 <= 716978_I, true);
   EXPECT_EQ(-716978 < -716978_I, false);
   EXPECT_EQ(-716978 <= -716978_I, true);
}

TEST(Less_operator, string_right_operand) {
  /*
   * Test less than and less than or equal operators having string
   * type at right operand
   */
   EXPECT_EQ(5397481354897945467899151264_I < "4399401354897945467876151264697", true);
   EXPECT_EQ(5397481354897945467899151264_I <= "4399401354897945467876151264697", true);
   EXPECT_EQ(5397481354897945467899151264_I < "-4399401354897945467876151264697", false);
   EXPECT_EQ(5397481354897945467899151264_I <= "-4399401354897945467876151264697", false);
   EXPECT_EQ(-5397481354897945467899151264_I < "4399401354897945467876151264697", true);
   EXPECT_EQ(-5397481354897945467899151264_I <= "4399401354897945467876151264697", true);
   EXPECT_EQ(-5397481354897945467899151264_I < "-4399401354897945467876151264697", false);
   EXPECT_EQ(-5397481354897945467899151264_I <= "-4399401354897945467876151264697", false);

   EXPECT_EQ(4399401354897945467876151264697_I < "5397481354897945467899151264", false);
   EXPECT_EQ(4399401354897945467876151264697_I <= "5397481354897945467899151264", false);
   EXPECT_EQ(-4399401354897945467876151264697_I < "5397481354897945467899151264", true);
   EXPECT_EQ(-4399401354897945467876151264697_I <= "5397481354897945467899151264", true);
   EXPECT_EQ(4399401354897945467876151264697_I < "-5397481354897945467899151264", false);
   EXPECT_EQ(4399401354897945467876151264697_I <= "-5397481354897945467899151264", false);
   EXPECT_EQ(-4399401354897945467876151264697_I < "-5397481354897945467899151264", true);
   EXPECT_EQ(-4399401354897945467876151264697_I <= "-5397481354897945467899151264", true);

   EXPECT_EQ(100976600013970343640064646346479797_I < "100976600013970343640064646346479797", false);
   EXPECT_EQ(100976600013970343640064646346479797_I <= "100976600013970343640064646346479797", true);
   EXPECT_EQ(-100976600013970343640064646346479797_I < "100976600013970343640064646346479797", true);
   EXPECT_EQ(-100976600013970343640064646346479797_I <= "100976600013970343640064646346479797", true);
   EXPECT_EQ(100976600013970343640064646346479797_I < "-100976600013970343640064646346479797", false);
   EXPECT_EQ(100976600013970343640064646346479797_I <= "-100976600013970343640064646346479797", false);
   EXPECT_EQ(-100976600013970343640064646346479797_I < "-100976600013970343640064646346479797", false);
   EXPECT_EQ(-100976600013970343640064646346479797_I <= "-100976600013970343640064646346479797", true);
}

TEST(Less_operator, string_left_operand) {
  /*
   * Test less than and less than or equal operators having string
   * type at left operand
   */
   EXPECT_EQ("5397481354897945467899151264" < 4399401354897945467876151264697_I, true);
   EXPECT_EQ("5397481354897945467899151264" <= 4399401354897945467876151264697_I, true);
   EXPECT_EQ("5397481354897945467899151264" < -4399401354897945467876151264697_I, false);
   EXPECT_EQ("5397481354897945467899151264" <= -4399401354897945467876151264697_I, false);
   EXPECT_EQ("-5397481354897945467899151264" < 4399401354897945467876151264697_I, true);
   EXPECT_EQ("-5397481354897945467899151264" <= 4399401354897945467876151264697_I, true);
   EXPECT_EQ("-5397481354897945467899151264" < -4399401354897945467876151264697_I, false);
   EXPECT_EQ("-5397481354897945467899151264" <= -4399401354897945467876151264697_I, false);

   EXPECT_EQ("4399401354897945467876151264697" < 5397481354897945467899151264_I, false);
   EXPECT_EQ("4399401354897945467876151264697" <= 5397481354897945467899151264_I, false);
   EXPECT_EQ("-4399401354897945467876151264697" < 5397481354897945467899151264_I, true);
   EXPECT_EQ("-4399401354897945467876151264697" <= 5397481354897945467899151264_I, true);
   EXPECT_EQ("4399401354897945467876151264697" < -5397481354897945467899151264_I, false);
   EXPECT_EQ("4399401354897945467876151264697" <= -5397481354897945467899151264_I, false);
   EXPECT_EQ("-4399401354897945467876151264697" < -5397481354897945467899151264_I, true);
   EXPECT_EQ("-4399401354897945467876151264697" <= -5397481354897945467899151264_I, true);

   EXPECT_EQ("100976600013970343640064646346479797" < 100976600013970343640064646346479797_I, false);
   EXPECT_EQ("100976600013970343640064646346479797" <= 100976600013970343640064646346479797_I, true);
   EXPECT_EQ("-100976600013970343640064646346479797" < 100976600013970343640064646346479797_I, true);
   EXPECT_EQ("-100976600013970343640064646346479797" <= 100976600013970343640064646346479797_I, true);
   EXPECT_EQ("100976600013970343640064646346479797" < -100976600013970343640064646346479797_I, false);
   EXPECT_EQ("100976600013970343640064646346479797" <= -100976600013970343640064646346479797_I, false);
   EXPECT_EQ("-100976600013970343640064646346479797" < -100976600013970343640064646346479797_I, false);
   EXPECT_EQ("-100976600013970343640064646346479797" <= -100976600013970343640064646346479797_I, true);
}

TEST(Equal_operator, random_values) {
  /*
   * Test equal operator by using random Integer values
   */
  std::default_random_engine int_gen(1678);
  Integer::set_random_engine(int_gen);

  for (size_t test = 0; test < 10000; test++) {
    Integer a;
    a.set_random_value();

    EXPECT_EQ(a == a, true) << "a=" << a << std::endl;
    EXPECT_EQ(a <= a, true) << "a=" << a << std::endl;
    EXPECT_EQ(a >= a, true) << "a=" << a << std::endl;

    EXPECT_EQ(-a == -a, true) << "-a=" << -a << std::endl;
    EXPECT_EQ(-a <= -a, true) << "-a=" << -a << std::endl;
    EXPECT_EQ(-a >= -a, true) << "-a=" << -a << std::endl;

    EXPECT_EQ(-a == a, false || a.is_zero()) << "a=" << a << ", -a=" << -a <<std::endl;
    EXPECT_EQ(-a <= a, true) << "a=" << a << ", -a=" << -a <<std::endl;
    EXPECT_EQ(-a >= a, false || a.is_zero()) << "a=" << a << ", -a=" << -a <<std::endl;

    EXPECT_EQ(a == -a, false || a.is_zero()) << "a=" << a << ", -a=" << -a <<std::endl;
    EXPECT_EQ(a <= -a, false || a.is_zero()) << "a=" << a << ", -a=" << -a <<std::endl;
    EXPECT_EQ(a >= -a, true) << "a=" << a << ", -a=" << -a <<std::endl;
  }
}

TEST(Equal_operator, long_long_int_right_operand) {
  /*
   * Test equal operator having long long int type at right operand
   */

   EXPECT_EQ(0_I == 0, true);

   EXPECT_EQ(4294967295_I == 4294967295, true);
   EXPECT_EQ(4294967295_I == -4294967295, false);
   EXPECT_EQ(-4294967295_I == 4294967295, false);
   EXPECT_EQ(-4294967295_I == -4294967295, true);

   EXPECT_EQ(5464787_I == 4294967295, false);
   EXPECT_EQ(5464787_I == -4294967295, false);
   EXPECT_EQ(-5464787_I == 4294967295, false);
   EXPECT_EQ(-5464787_I == -4294967295, false);
}

TEST(Equal_operator, string_right_operand) {
  /*
   * Test equal operator hacing string type at right operand
   */
   EXPECT_EQ(0_I == "0", true);

   EXPECT_EQ(137879131303364876841303_I == "137879131303364876841303", true);
   EXPECT_EQ(137879131303364876841303_I == "-137879131303364876841303", false);
   EXPECT_EQ(-137879131303364876841303_I == "137879131303364876841303", false);
   EXPECT_EQ(-137879131303364876841303_I == "-137879131303364876841303", true);

   EXPECT_EQ(47981361310364676115367487_I == "47981361310364676115367487", true);
   EXPECT_EQ(47981361310364676115367487_I == "-47981361310364676115367487", false);
   EXPECT_EQ(-47981361310364676115367487_I == "47981361310364676115367487", false);
   EXPECT_EQ(-47981361310364676115367487_I == "-47981361310364676115367487", true);
}

TEST(Equal_operator, long_long_int_left_operand) {
  /*
   * Test equal operator hacing long long int type at left operand
   */

   EXPECT_EQ(0 == 0_I, true);

   EXPECT_EQ(4294967295 == 4294967295_I, true);
   EXPECT_EQ(4294967295 == -4294967295_I, false);
   EXPECT_EQ(-4294967295 == 4294967295_I, false);
   EXPECT_EQ(-4294967295 == -4294967295_I, true);

   EXPECT_EQ(5464787 == 4294967295_I, false);
   EXPECT_EQ(5464787 == -4294967295_I, false);
   EXPECT_EQ(-5464787 == 4294967295_I, false);
   EXPECT_EQ(-5464787 == -4294967295_I, false);
}

TEST(Equal_operator, string_left_operand) {
  /*
   * Test equal operator hacing string type at left operand
   */
   EXPECT_EQ("0" == 0_I, true);

   EXPECT_EQ("137879131303364876841303" == 137879131303364876841303_I, true);
   EXPECT_EQ("137879131303364876841303" == -137879131303364876841303_I, false);
   EXPECT_EQ("-137879131303364876841303" == 137879131303364876841303_I, false);
   EXPECT_EQ("-137879131303364876841303" == -137879131303364876841303_I, true);

   EXPECT_EQ("47981361310364676115367487" == 115366464131313543543_I, false);
   EXPECT_EQ("47981361310364676115367487" == -115366464131313543543_I, false);
   EXPECT_EQ("-47981361310364676115367487" == 115366464131313543543_I, false);
   EXPECT_EQ("-47981361310364676115367487" == -115366464131313543543_I, false);
}

TEST(Not_Equal_operator, random_values) {
  /*
   * Test distinct operator by using random Integer values
   */
  std::default_random_engine int_gen(1768);
  Integer::set_random_engine(int_gen);

  for (size_t test = 0; test < 10000; test++) {
    Integer a, b;

    do {
      a.set_random_value();
      b.set_random_value();
    } while (a == b);

    EXPECT_EQ(a != b, true) << "a=" << a << ", b=" << b << std::endl;
    EXPECT_EQ(a < b || a > b, true) << "b=" << b << ", a=" << a << std::endl;
  }
}

TEST(Not_Equal_operator, long_long_int_right_operand) {
  /*
   * Test not equal operator having long long int type at right operand
   */

   EXPECT_EQ(0_I != 0, false);

   EXPECT_EQ(4294967295_I != 4294967295, false);
   EXPECT_EQ(4294967295_I != -4294967295, true);
   EXPECT_EQ(-4294967295_I != 4294967295, true);
   EXPECT_EQ(-4294967295_I != -4294967295, false);

   EXPECT_EQ(5464787_I != 4294967295, true);
   EXPECT_EQ(5464787_I != -4294967295, true);
   EXPECT_EQ(-5464787_I != 4294967295, true);
   EXPECT_EQ(-5464787_I != -4294967295, true);
}

TEST(Not_Equal_operator, string_right_operand) {
  /*
   * Test not equal operator having string type at right operand
   */
   EXPECT_EQ(0_I != "0", false);

   EXPECT_EQ(137879131303364876841303_I != "137879131303364876841303", false);
   EXPECT_EQ(137879131303364876841303_I != "-137879131303364876841303", true);
   EXPECT_EQ(-137879131303364876841303_I != "137879131303364876841303", true);
   EXPECT_EQ(-137879131303364876841303_I != "-137879131303364876841303", false);

   EXPECT_EQ(47981361310364676115367487_I != "115366464131313543543", true);
   EXPECT_EQ(47981361310364676115367487_I != "-115366464131313543543", true);
   EXPECT_EQ(-47981361310364676115367487_I != "115366464131313543543", true);
   EXPECT_EQ(-47981361310364676115367487_I != "-115366464131313543543", true);
}

TEST(Not_Equal_operator, long_long_int_left_operand) {
  /*
   * Test not equal operator having long long int type at left operand
   */

   EXPECT_EQ(0 != 0_I, false);

   EXPECT_EQ(4294967295 != 4294967295_I, false);
   EXPECT_EQ(4294967295 != -4294967295_I, true);
   EXPECT_EQ(-4294967295 != 4294967295_I, true);
   EXPECT_EQ(-4294967295 != -4294967295_I, false);

   EXPECT_EQ(5464787 != 4294967295_I, true);
   EXPECT_EQ(5464787 != -4294967295_I, true);
   EXPECT_EQ(-5464787 != 4294967295_I, true);
   EXPECT_EQ(-5464787 != -4294967295_I, true);
}

TEST(Not_Equal_operator, string_left_operand) {
  /*
   * Test not equal operator having string type at left operand
   */
   EXPECT_EQ("0" != 0_I, false);

   EXPECT_EQ("137879131303364876841303" != 137879131303364876841303_I, false);
   EXPECT_EQ("137879131303364876841303" != -137879131303364876841303_I, true);
   EXPECT_EQ("-137879131303364876841303" != 137879131303364876841303_I, true);
   EXPECT_EQ("-137879131303364876841303" != -137879131303364876841303_I, false);

   EXPECT_EQ("47981361310364676115367487" != 115366464131313543543_I, true);
   EXPECT_EQ("47981361310364676115367487" != -115366464131313543543_I, true);
   EXPECT_EQ("-47981361310364676115367487" != 115366464131313543543_I, true);
   EXPECT_EQ("-47981361310364676115367487" != -115366464131313543543_I, true);
}