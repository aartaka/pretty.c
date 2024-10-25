#include <assert.h>
#include <stdio.h>
#include "pretty.h"

void
test_comparison_macros()
{
	assert((5 eq 5) == true);
	assert((5 eq 3) == false);
	assert((5 is 5) == true);
	assert((5 is 4) == false);
	assert((5 isnt 4) == true);
}

void
test_bitwise_macros()
{
	assert((unsigned char)(bitnot 0x0F) == (unsigned char)~0x0F);
	assert((0x0F bitxor 0xF0) == (0x0F ^ 0xF0));
}

void
test_min_max_macros()
{
	assert(min(2, 3) == 2);
	assert(max(2, 3) == 3);
}

void
test_divisible_macro()
{
	assert(divisible(10, 2) == true);
	assert(divisible(10, 3) == false);
}

void
test_limit_clamp_macros()
{
	assert(limit(1, 5, 10) == 5);
	assert(limit(1, 0, 10) == 1);
	assert(limit(1, 15, 10) == 10);
	assert(clamp(1, 5, 10) == 5);
	assert(clamp(1, 0, 10) == 1);
}

void
test_predicate_macros()
{
	int even_number = 4;
	int odd_number = 3;
	assert(not(even odd_number));
	assert(odd odd_number);
	assert(not(odd even_number));
	assert(positive 5);
	assert(not(positive - 1));
	assert(negative - 5);
	assert(not(negative 3));
	assert(zero 0);
	assert(not(zero 1));
	int *ptr = NULL;
	assert(empty ptr);
}

void
test_len_macro()
{
	int arr[] = { 1, 2, 3, 4 };
	char *str = "hello";
	assert(len(arr) == 4);
	assert(len(str) == 5);
}

void
test_foreach_forthese_macros()
{
	int arr[] = { 1, 2, 3 };
	int sum = 0;
	foreach(item, int, 3, arr) {
		sum += *item;
	}
	assert(sum == 6);

	sum = 0;
	forthese(num, int, 1, 2, 3, 4) {
		sum += num;
	}
	assert(sum == 10);
}

void
test_memory_macros()
{
	int *num = new(int, 42);
	assert(*num == 42);
	delete(num);

	int *arr = vector(3, int, 1, 2, 3);
	assert(arr[0] == 1);
	assert(arr[1] == 2);
	assert(arr[2] == 3);
	delete(arr);
}

void
test_conditional_macros()
{
	int x = 5;
	int y = when x > 3 then 10 other 0;
	assert(y == 10);

	y = when x < 3 then 10 other 0;
	assert(y == 0);

	int z = unless x > 3 then 10 other 0;
	assert(z == 0);

	ifnt(x > 3) {
		y = 0;
	} else {
		y = 1;
	}
	assert(y == 1);
}

void
test_try_catch_macros()
{
	errno = 0;
	try {
		sqrt(-1);
	}
	catch(EDOM) {
		assert(errno == EDOM);
	}
}

void
test_equal_macro()
{
	assert(equal(5, 5));
	assert(!equal(5, 3));

	char *str1 = "hello";
	char *str2 = "hello";
	char *str3 = "world";
	assert(equal(str1, str2));
	assert(!equal(str1, str3));

	float a = 0.1f + 0.2f;
	float b = 0.3f;
	assert(equal(a, b));
}

void
test_print_macros()
{
	print("Testing print macro: ");
	print(123);
	print("\n");
	print("Testing println macro with number: ");
	println(456);
}

int
main()
{
	test_comparison_macros();
	test_bitwise_macros();
	test_min_max_macros();
	test_divisible_macro();
	test_limit_clamp_macros();
	test_predicate_macros();
	test_len_macro();
	test_foreach_forthese_macros();
	test_memory_macros();
	test_conditional_macros();
	test_try_catch_macros();
	test_equal_macro();
	test_print_macros();

	printf("All tests passed successfully.\n");
	return 0;
}
