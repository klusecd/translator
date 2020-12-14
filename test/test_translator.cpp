#include "translator.h"

#include <gtest.h>
TEST(Translator, cant_divide_by_0) {
	std::string str = "1/0=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.solve());
}

TEST(Translator, detects_illegal_characters) {
	std::string exprStr = "1+1~";
	Arithmetic_expressions expr(exprStr);
	ASSERT_ANY_THROW(expr.solve());
}

TEST(Translator, too_many_opening_brackets) {
	std::string exprStr = "1+(1+2+(4-3)";
	Arithmetic_expressions expr(exprStr);
	ASSERT_ANY_THROW(expr.solve());
}

TEST(Translator, too_many_closing_brackets) {
	std::string exprStr = "1+(1+2)+4-3)";
	Arithmetic_expressions expr(exprStr);
	ASSERT_ANY_THROW(expr.solve());
}

TEST(Translator, invalid_term_sequence) {
	std::string exprStr = "1++3";
	Arithmetic_expressions expr(exprStr);
	ASSERT_ANY_THROW(expr.solve());
}

TEST(Translator, two_points_in_the_number) {
	std::string str = "1.2.34=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.solve());
}

TEST(Translator, uncorrect_symbols) {
	std::string str = "a+2=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.solve());
}

TEST(Translator, two_operations) {
	std::string str = "*+2=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.solve());
}

TEST(Translator, operation_and_bracket) {
	std::string str = "1+)2=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.solve());
}

TEST(Translator, two_different_brackets) {
	std::string str = "(1+2)(1+3)=";
	Arithmetic_expressions a(str);
	ASSERT_ANY_THROW(a.solve());
}

TEST(Translator, can_add) {
	Arithmetic_expressions expr("2+3");
	double res = 2 + 3;
	EXPECT_EQ(res, expr.solve());
}

TEST(Translator, can_subtract) {
	Arithmetic_expressions expr("4-1");
	double res = 4-1;
	EXPECT_EQ(res, expr.solve());
}

TEST(Translator, can_multiply) {
	Arithmetic_expressions expr("7*3");
	double res = 7*3;
	EXPECT_EQ(res, expr.solve());
}

TEST(Translator, can_divide) {
	Arithmetic_expressions expr("6/3");
	double res = 6 / 3;
	EXPECT_EQ(res, expr.solve());
}

TEST(Translator, correct_order_of_operations) {
	Arithmetic_expressions expr("1-7+2*2");
	double res = 1 - 7 + 2 * 2;
	EXPECT_EQ(res, expr.solve());
}

TEST(Translator, brackets_work) {
	Arithmetic_expressions expr("(2+1)*3");
	double res = (2 + 1) * 3;
	EXPECT_EQ(res, expr.solve());
}


TEST(TExpression, combination_of_operations_and_priority_with_correct_answer) {
	Arithmetic_expressions expr("(6+4.3)*((4.5/5.03+12)+1.0)");
	double res = (6 + 4.3)*((4.5 / 5.03 + 12) + 1.0);
	EXPECT_EQ(res, expr.solve());
}