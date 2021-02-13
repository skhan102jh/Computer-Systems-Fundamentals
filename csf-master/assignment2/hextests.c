// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
/*
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);
*/
void sarahTestHexToPrintable();

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();
  /*
  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);
  */
  // SARAH'S ADDITIONAL TESTS
  TEST(sarahTestHexToPrintable);

  TEST_FINI();

  return 0;
}
/*
void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
}
*/
void sarahTestHexToPrintable() {
  printf("\n*** sarahTestHexToPrintable ***\n");
  /* NOTE: The long parameter is in decimal form.
  */

  // TESTING INVALID PARAMETERS
  ASSERT(-1 == hex_to_printable(-1));
  ASSERT(-1 == hex_to_printable(256));

  // TESTING LEGAL VALUES THAT REMAIN UNCHANGED
  // Testing the lowest unchanged value, which corresponds to "space" char
  ASSERT(' ' == hex_to_printable(32));
  // Testing the highest unchanged value, which corresponds to "~" char
  ASSERT('~' == hex_to_printable(126));
  //Test random characters between 32 and 126
  ASSERT('S' == hex_to_printable(83));
  ASSERT('e' == hex_to_printable(101));
  ASSERT('x' == hex_to_printable(120));

  // TESTING LEGAL VALUES THAT CHANGE TO PERIODS
  // Testing edge cases from 0 - 31 and 127 - 255
  ASSERT('.' == hex_to_printable(0));
  ASSERT('.' == hex_to_printable(31));
  ASSERT('.' == hex_to_printable(127));  
  ASSERT('.' == hex_to_printable(255));
}

