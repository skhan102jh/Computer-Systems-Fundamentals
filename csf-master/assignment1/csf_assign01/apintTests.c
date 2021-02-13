/*
 * Unit tests for arbitrary-precision integer data type
 *
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apint.h"
#include "tctest.h"

typedef struct {
	ApInt *ap0;
	ApInt *ap1;
	ApInt *ap110660361;
	ApInt *max1;
	/* TODO: add additional fields of test fixture */
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testCreateFromU64(TestObjs *objs);
void testHighestBitSet(TestObjs *objs);
void testLshiftN(TestObjs *objs);
void testCompare(TestObjs *objs);
void testFormatAsHex(TestObjs *objs);
void testAdd(TestObjs *objs);
void testSub(TestObjs *objs);
/* TODO: add more test function prototypes */
void testCreateFromHex(TestObjs *objs);

int main(int argc, char **argv) {
	TEST_INIT();
	
	if (argc > 1) {
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}

	TEST(testCreateFromU64);
	TEST(testHighestBitSet);
	TEST(testLshiftN);
	TEST(testCompare); // failed thus far
	TEST(testFormatAsHex); 
	TEST(testAdd);
	TEST(testSub);
	
	/* TODO: use TEST macro to execute more test functions */
	//TEST(testCreateFromHex);
	//TEST(testGetHighestBits);
	TEST(myTestFormatAsHex);
	TEST(myLeftShiftTest);
	TEST(testSubLargeNumbers);
	TEST(testLeftShift132);
	TEST(testLeftShiftOnLargeNumber);
	
	TEST(shiftOneToLeft);
	TEST(funShit);
	TEST(shiftingEightsTheirs);
	TEST(shiftingEightsMine);

	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->ap0 = apint_create_from_u64(0UL);
	objs->ap1 = apint_create_from_u64(1UL);
	objs->ap110660361 = apint_create_from_u64(110660361UL);
	objs->max1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFFUL);
	/* TODO: initialize additional members of test fixture */

	return objs;
}

void cleanup(TestObjs *objs) {
	apint_destroy(objs->ap0);
	apint_destroy(objs->ap1);
	apint_destroy(objs->ap110660361);
	apint_destroy(objs->max1);
	/* TODO: destroy additional members of test fixture */

	free(objs);
}

void testCreateFromU64(TestObjs *objs) {
	ASSERT(0UL == apint_get_bits(objs->ap0, 0));
	ASSERT(1UL == apint_get_bits(objs->ap1, 0));
	ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
}

void testHighestBitSet(TestObjs *objs) {
	ASSERT(-1 == apint_highest_bit_set(objs->ap0));
	ASSERT(0 == apint_highest_bit_set(objs->ap1));
	ASSERT(26 == apint_highest_bit_set(objs->ap110660361));
	ASSERT(63 == apint_highest_bit_set(objs->max1));
}

void testLshiftN(TestObjs *objs) {
	ApInt *result;

	result = apint_lshift_n(objs->ap0, 17);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);
	printf("Passed the first left shift test\n");

	
	result = apint_lshift_n(objs->ap1, 17);
	ASSERT(0x20000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);
	

	result = apint_lshift_n(objs->ap110660361, 17);
	ASSERT(0xD3116120000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	
}

void testCompare(TestObjs *objs) {
	/* 1 > 0 */
	ASSERT(apint_compare(objs->ap1, objs->ap0) > 0);
	/* 0 < 1 */
	ASSERT(apint_compare(objs->ap0, objs->ap1) < 0);
	/* 110660361 > 0 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap0) > 0);
	/* 110660361 > 1 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap1) > 0);
	/* 0 < 110660361 */
	ASSERT(apint_compare(objs->ap0, objs->ap110660361) < 0);
	/* 1 < 110660361 */
	ASSERT(apint_compare(objs->ap1, objs->ap110660361) < 0);

	// MY ADDED TEST
	ASSERT(apint_compare(objs->ap1, objs->ap1) == 0);
}

void testFormatAsHex(TestObjs *objs) {
	char *s;
	//s = apint_format_as_hex(objs->ap0);
	//printf("in testFormatAsHex, s got: %s\n", s[0]);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->ap0))));
	free(s);
	//printf("Passed ZERO TEST\n");

	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(objs->ap1))));
	free(s);

	ASSERT(0 == strcmp("6988b09", (s = apint_format_as_hex(objs->ap110660361))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1))));
	free(s);
}

void testAdd(TestObjs *objs) {
	ApInt *sum;
	char *s;

	/* 0 + 0 = 0 */
	sum = apint_add(objs->ap0, objs->ap0);
	s = apint_format_as_hex(sum); // MY LINE
	//printf("The sum that s got was: %s\n", s);
	//ASSERT(0 == strcmp("0", (s = apint_format_as_hex(sum))));
	
	apint_destroy(sum);
	free(s);

	/* 1 + 0 = 1 */
	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 1 = 2 */
	sum = apint_add(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 110660361 + 1 = 110660362 */
	sum = apint_add(objs->ap110660361, objs->ap1);
	ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);
}

void testSub(TestObjs *objs) {

	ApInt *a, *b, *diff;
	char *s;
	
	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);


	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);
	//printf("\n Passed invalid subtraction arguments.\n");

	/* subtracting 0 from 1 is 1 */
	diff = apint_sub(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap1));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 1 is 0 */
	diff = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap0));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 0 can't be represented because it is negative */
	ASSERT(NULL == apint_sub(objs->ap0, objs->ap1));

	
	/* test involving larger values */
	a = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	b = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	diff = apint_sub(a, b);
	ASSERT(0 == strcmp("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
		(s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);
}

/* TODO: add more test functions */
void testCreateFromHex(TestObjs *objs) {
	//printf("\n\nTESTING CREATE FROM HEX\n");
	//printf("TESTING WITH 7e35\n");
	//apint_create_from_hex("7e35");
	//printf("TESTING WITH 7e35207519b6afc4\n");
	//ApInt * tester = apint_create_from_hex("7e35207519b6afc4");
	//printf(" TESTING WITH 7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691");
	//ApInt * tester2 = apint_create_from_hex("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691");
	printf("TESTING WITH 01230123456789abcdef");
	ApInt * tester3 = apint_create_from_hex("01230123456789abcdef");
	apint_format_as_hex(tester3);
	//char * testWord = {'a', 'b', '\0'};
	//printf(strlen(testWord));
}

void testGetHighestBits(TestObjs *objs) {
	ApInt * tester3 = apint_create_from_hex("01230123456789abcdef");
	printf("The highest position is %d", apint_highest_bit_set(tester3));


}
void myTestFormatAsHex(TestObjs *objs) {
	//printf("\nTESTING FORMAT AS HEX\n");
	char * s;
	ApInt * tester3 = apint_create_from_hex("01230123456789abcdef");
	//ApInt * tester3 = apint_create_from_hex("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691");
	//printf("The result from formatting as hex\n");
	s = apint_format_as_hex(tester3);
	//printf("About to print the new hex string: %s \n", s);
	//printf(s);
}

void myLeftShiftTest(TestObjs *objs) {
	//ApInt * tester3 = apint_create_from_hex("01230123456789abcdef");
	ApInt * tester = apint_create_from_hex("abc");

	/*
	for (int i = 0; i < tester3->length; i++) {
		printf("num: %lu\n",tester3->arr[i]);
	}
	*/
	//ApInt * result = apint_lshift_n(tester, 1);
	ApInt * result = apint_lshift(tester);
	//printf("Result is %s \n", apint_format_as_hex(result));
	//ASSERT(0UL == apint_get_bits(result, 0));
	//ASSERT(0UL == apint_get_bits(result, 1));
	//apint_destroy(result);
	//printf("\nPassed the first left shift test\n");
}

void testAddLargeNumbers(TestObjs *objs) {
	// Create two ApInt objects and check the sum 
	ApInt * a = apint_create_from_hex("641afee52358ee358c4676ec8a32025f83ed57b5b8fb3bcf5643f6987d8267e84498a6c25662c7");
	ApInt * b = apint_create_from_hex("cfb8b3d2e990c6dfd7da4d2fe62d69775f932ccd5b28d9295106b4d85e3");
	ApInt * sum = apint_add(a, b);
	char * s;
	ASSERT(0 == strcmp("641afee52358ee358c537277c7609b6bf1ead55a8bf99ea5edb9efcb4a581a75d72db72da3e8aa", (s = apint_format_as_hex(sum))));

	a = apint_create_from_hex("f02e0c0f3e9e9ed3d526c8f3");
	b = apint_create_from_hex("85c581ba75f649b09877c7ba722c58afef322c763bc0e83b72964cedaae0ce2");
	sum = apint_add(a, b);
	ASSERT(0 == strcmp("85c581ba75f649b09877c7ba722c58afef322c853ea1a92f5c803a2afd4d5d5", (s = apint_format_as_hex(sum))));
}

void testSubLargeNumbers(TestObjs *objs) {
	/* Fact: 151ca661d9f1a58207bbe68c5 - 1c31e6137476fd5d9e55d3f5 = 13598800a2aa35ac2dd6894d0
	*/
	ApInt * a = apint_create_from_hex("151ca661d9f1a58207bbe68c5");
	ApInt * b = apint_create_from_hex("1c31e6137476fd5d9e55d3f5");
	ApInt * diff = apint_sub(a, b);
	char * d;
	ASSERT(0 == strcmp("13598800a2aa35ac2dd6894d0", (d = apint_format_as_hex(diff))));

	/* Fact: 6fa3da8bfcf2ce045f41ff97fd5a06c68bc27dc90cd7b3e9bc2a2036008d22248aaad1bdea2bff875bd0724c86cf032e1e9
	- 429324f2adeb7cc2d338c03419c2da2acc9d1e992b41b214684f068be0012e06658233c17ca81e3a02709a65e3a09cd4 
	= 6f9fb159adc7ef4c9314cc0bfa186a98e915b3f72344ffce9ae39b4597cf2211aa44799aae14350578304b42e070c924515
	*/
	a = apint_create_from_hex("6fa3da8bfcf2ce045f41ff97fd5a06c68bc27dc90cd7b3e9bc2a2036008d22248aaad1bdea2bff875bd0724c86cf032e1e9");
	b = apint_create_from_hex("429324f2adeb7cc2d338c03419c2da2acc9d1e992b41b214684f068be0012e06658233c17ca81e3a02709a65e3a09cd4");
	diff = apint_sub(a, b);
	
	ASSERT(0 == strcmp("6f9fb159adc7ef4c9314cc0bfa186a98e915b3f72344ffce9ae39b4597cf2211aa44799aae14350578304b42e070c924515", (d = apint_format_as_hex(diff))));
}

void testLeftShift132(TestObjs *objs) {
	ApInt * tester = apint_create_from_hex("abcdef");
	ApInt *result;
	char * d;

	//result = apint_lshift_n(tester, 68);
	//printf("The actual shifted stuff: %s\n", d = apint_format_as_hex(result));
	//ASSERT(0 == strcmp("abcdef00000000000000000", (d = apint_format_as_hex(result))));
	// shift it 68 + 64 = 132 times
	result = apint_lshift_n(tester, 132);
	printf("The actual shifted stuff: %s\n", d = apint_format_as_hex(result));
		ASSERT(0 == strcmp("abcdef000000000000000000000000000000000", (d = apint_format_as_hex(result))));
	//0000000000000000
	//2af37bc00000000000000000
	//abcdef000000000000000000000000000000000 33 zeros	  actual 
	//abcdef000000000000000000000000000000000   predicted

}

void testLeftShiftOnLargeNumber(TestObjs *objs) {
	/*
	printf("Entered the desired testing method.\n");
	ApInt * tester = apint_create_from_hex("d172517b34ae51d7");
	ApInt * result = apint_lshift(tester);
	printf("%s\n", apint_format_as_hex(result));
	/*
	ApInt * tester2 = apint_create_from_hex("c3bd0af13cf46f61d108c27777d8a790a67de1ffac3eb56a2a36e0fca59fb3d42e7b14336ec9b3e93585652b092");
	ApInt * result2 = apint_lshift_n(tester2, 3);
	printf("%s\n", apint_format_as_hex(result2));*/
	/*
	ApInt * tester2 = apint_create_from_hex("c3bd0af13cf46f61d108c27777d8a790a67de1ffac3eb56a2a36e0fca59fb3d42e7b14336ec9b3e93585652b092");
	ApInt * result2 = apint_lshift_n(tester2, 68);
	printf("Final result;\n");
	printf("%s\n", apint_format_as_hex(result2));
	
	ApInt * tester3 = apint_create_from_hex("c3bd0af13cf46f61d108c27777d8a790a67de1ffac3eb56a2a36e0fca59fb3d42e7b14336ec9b3e93585652b092");
	ApInt * result3 = apint_lshift(tester3); 
	printf("When we left shift by 1, we get %s\n", apint_format_as_hex(result3));
	*/
	ApInt * tester4 = apint_create_from_hex("8888888888888888");
	//11111111111111110;
	ApInt * result4 = apint_lshift(tester4);
	printf("When we shift 8888888888888888 by 1 we get:%s\n", apint_format_as_hex(result4));
	printf("The elements in the index 0 of result is: %lu\n", result4->arr[0]);
	//printf("The result to my get bits is: %lu\n", apint_get_bits(result4, 0));
	//printf("The result of the left shift on 88.888 is %s\n", apint_format_as_hex(result));
	//ASSERT(1229782938247303440 == apint_get_bits(result4, 0));
}


void shiftOneToLeft(TestObjs *objs) {
	ApInt * tester1 = apint_create_from_hex("1");
	ApInt * result1 = apint_lshift(tester1); 
	printf("When we shift 1 by 1 to the left, we get: %s\n", apint_format_as_hex(result1));
}
void funShit() {
printf("entered shifting eights theirs\n");
	/*
	ApInt * tester3 = apint_create_from_u64(0x8888888888888888UL);
	ApInt *res2 = apint_lshift(tester3);
	ASSERT(2 == res2->length);
	printf("The result as res2->arr[0] is: %lu\n", res2->arr[0]);
	ASSERT(0x1111111111111110UL == res2->arr[0]);
	ASSERT(0x1UL == res2->arr[1]);
	*/

	ApInt * tester1 = apint_create_from_u64(0UL);
	ApInt * tester2 = apint_create_from_u64(1UL);
	ApInt * tester3 = apint_create_from_u64(0x8888888888888888UL);


	//ApInt *res0 = apint_lshift(tester1);
	//ApInt *res1 = apint_lshift(tester2);
	ApInt *res2 = apint_lshift(tester3);
	//ApInt *res3 = apint_lshift(res2);
	//printf("res0 length is %d\n", res0->length);
	//ASSERT(1 == res0->length);
	//ASSERT(0UL == res0->arr[0]);
	//ASSERT(1 == res1->length);
	//ASSERT(2UL == res1->arr[0]);
	ASSERT(2 == res2->length);
	printf("The result as res2->arr[0] is: %lu\n", res2->arr[0]);
	ASSERT(0x1111111111111110UL == res2->arr[0]);
	
	       //11111111111111110
	ASSERT(0x1UL == res2->arr[1]);
	//ASSERT(2 == res3->length);
	//ASSERT(0x2222222222222220UL == res3->arr[0]);
	//ASSERT(0x2UL == res3->arr[1]);

	//apint_destroy(res3);
	apint_destroy(res2);
	//apint_destroy(res1);
	//apint_destroy(res0);
}

void shiftingEightsTheirs() {
	printf("entered shifting eights theirs\n");
	ApInt * tester3 = apint_create_from_u64(0x8888888888888888UL);
	ApInt *res2 = apint_lshift(tester3);
	ASSERT(2 == res2->length);
	printf("The result as res2->arr[0] is: %lu\n", res2->arr[0]);
	ASSERT(0x1111111111111110UL == res2->arr[0]);
	ASSERT(0x1UL == res2->arr[1]);
	//ASSERT(0x1111111111111110UL == res2->arr[0]);

}

void shiftingEightsMine() {
	ApInt * tester4 = apint_create_from_hex("8888888888888888");
	//11111111111111110;
	ApInt * result4 = apint_lshift(tester4);
	printf("When we shift 8888888888888888 by 1 we get:%s\n", apint_format_as_hex(result4));
	printf("The elements in the index 0 of result is: %lu\n", result4->arr[0]);
}
//1877a15e279e8dec3a21184eeefb14f214cfbc3ff587d6ad4546dc1f94b3f67a85cf62866dd9367d26b0aca56124
//1877a15e279e8dec3a21184eeefb14f214cfbc3ff587d6ad4546dc1f94b3f67a85cf62866dd9367d26b0aca56124



