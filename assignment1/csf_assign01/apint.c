/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "apint.h"
#include <math.h>

ApInt *apint_create_from_u64(uint64_t val) {
	/* TODO: implement */
  ApInt * ai = malloc(sizeof(ApInt));
  ai->arr = malloc(sizeof(uint64_t));
  ai->length = 1;
  ai->arr[0] = val;
  //assert(0);
  return ai;
}

ApInt *apint_create_from_hex(const char *hex) {
	//printf("Beginning of create_from_hex method call****\n");
	/* TODO: implement */
	// Get rid of all of the zeros in the beginning 
	//while(*hex == '0') {
	//	hex++; // Move the pointer to the next character
	//}

	// In case there are only zeros (NEED TO IMPLEMENT)

	// Figure out the string length 
	size_t hexLength = strlen(hex);
	//printf("The string length is %d\n", hexLength);
	// Create a new ApInt object with needed length
	ApInt * ai = malloc(sizeof(ApInt));
	ai->length = ceil(hexLength / 16.0);
	ai->arr = malloc((ai->length) * sizeof(uint64_t)); 
	
	// Calculate number of elements needed in ApInt arr
	int elements = ceil(hexLength / 16.0);
	//printf("The ceiling is %d\n", elements);

	// ITERATE THROUGH STRING TO GET GROUPS OF 16 CHARACTERS
	int count = 15; 
	int arrIndex = 0; 
	// create a 16 element string
	char tempWord[17];
	for (int i = 0; i < 16; i++) {
		tempWord[i] = '0';
	}
	tempWord[16] = '\0';


	//printf("The very first element is %c\n", hex[0]);
	for (int d = (hexLength - 1); d >= 0; d--) {

		// if count is less than 16
		char tempChar = hex[d];

		//printf("%c\n", hex[d]);
		//printf("Tempchar = \n" + hex[d]);
		if (count < 16) {
			tempWord[count] = tempChar;
			count--;
		}
		if ((count == -1) || (d == 0)) {
			//printf("TempWord: %s\n", tempWord);
			uint64_t tempElement = strtoul(tempWord, NULL, 16);
			//printf("Temp element: %lu\n", tempElement);
			ai->arr[arrIndex] = tempElement;
			count = 15;
			// clear the word 
			for (int b = 0; b < 16; b++) {
				tempWord[b] = '0';
			}
			arrIndex++;	
		}
		
	}
	return ai;
	
}
/*
uint64_t hexStringToUInt64(char *hex) {
	printf("Using my helper method\n");
	int hexLength = strlen(hex);
	uint64_t sum = 0; 
	char tempWord[2];
	tempWord[1] = '\0';
	for (int i = 0; i < hexLength; i++) {
		tempWord[0] = hex[hexLength - 1 - i];
		unsigned int tempInt = strtol(tempWord, NULL, 16); 
		uint64_t secondHalf = 1; 
		printf("i is:  %d     ;",i);
		for (int j = 0; j < i; j++) {
	
			secondHalf *= 16;
			
		}
		printf("second half %d\n", secondHalf);
		sum += tempInt * secondHalf;
		printf("sum: %d\n", sum);
	}
	return sum;
}
*/

void apint_destroy(ApInt *ap) {
	/* TODO: implement */
	free(ap->arr);
	free(ap);
	//assert(0);
}

uint64_t apint_get_bits(ApInt *ap, unsigned n) {
	/* TODO: implement */
	if (ap->length <= n) {
		return 0;
	}
	else {
		return ap->arr[n];
	}
	//assert(0);
	return 0UL;
}

int apint_highest_bit_set(ApInt *ap) {
	
	int currIndex = ap->length - 1;
	if ((currIndex == 0) && (ap->arr[currIndex] == 0)) {
		return -1;
	} 
	
	
	while((ap->arr[currIndex] == 0) && (currIndex >= 0)) {
		currIndex--;
	}
	//printf("currIndex that will be used is: %d\n", currIndex);
	if ((currIndex == 0) && (ap->arr[currIndex] == 0)) {
		return -1;
	}
	uint64_t element = ap->arr[currIndex];
	int numShifts = 0; 
	while (element != 0) {
		element = element >> 1;
		numShifts++;
	}
	// have to account for if there are still element below that. it will be that number
	// plus the number of elements below * 64
	//return (numShifts - 1); ORIGINAL RETURN STATEMENT
	numShifts--; // gives us the placement in the given element of array
	numShifts += currIndex * 64; // gives us the placement in the entire array
	return numShifts;
	//assert(0);
	//return -1;
}

ApInt *apint_lshift(ApInt *ap) {
	/* TODO: implement */
	return apint_lshift_n(ap, 1);
	//assert(0);
	//return NULL;
}

ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
	//printf("entered the desired method lol.\n");
	// The part i just commented out to see if i can shift 1:
	/*
	if (apint_highest_bit_set(ap) == 0) {
		// There will be no effect when shifting. 
		return apint_create_from_u64(0UL);
	}*/
	// Create a deep copy of the passed in ApInt
	int extraSpaces = n / 64; 
	printf("\n We need %d extra spaces.\n", extraSpaces);
	int realShifts = n % 64;
	printf("We are doing %d real shifts\n", realShifts);
	/*
	if ((extraSpaces == 0) && (n == 1) && (ai->length == 1)) {
		ApInt * ai = malloc(sizeof(ApInt));
		ai->length = ap->length;
		ai->arr = malloc(ai->length * sizeof(uint64_t));
		ai->arr[0] = 
	}*/
	ApInt * ai = malloc(sizeof(ApInt));
	ai->length = ap->length + 1;
	// Make the new array one element longer
  	ai->arr = malloc((ai->length) * sizeof(uint64_t));
	// Make the first element of the new array zero 
	ai->arr[0] = 0; 
	printf("\nThe length of the new array is %d\n", ai->length);
	// Copy over elements with each element one index greater
	for (int i = 0; i < ap->length; i++) {
		//printf("entered first for loop\n");
		printf("trying to store: %lu \n", ap->arr[i]);
		//apint_format_as_hex(ap->arr[i]);
		ai->arr[i + 1] = ap->arr[i]; //>> (64 - n);
		
	}
	printf("The contents of the new array:\n");
	// shift all the contents in the new array over by 64 - n 
	for (int i = 0; i < ai->length; i++) {
		//apint_format_as_hex(ai->arr[i]);
		printf("num: %lu \n", ai->arr[i]);
	}
	for (int i = 0; i < ai->length; i++) {
		ai->arr[i] = ai->arr[i] >> (64 - realShifts);
	}
	printf("\nRight BEFORE we shift the AP operation, AP contains: \n");
		for (int i = 0; i < ap->length; i++) {
		printf("number: %lu\n", ap->arr[i]);
	}
	// shift all the contents of the old array by n 
	for (int i = 0; i < ap->length; i++) {
		ap->arr[i] = ap->arr[i] << realShifts;
		//ai->arr[i] = ap->arr[i] & ai->arr[i];
	}
	// add the elements of the array together with the OR operation
	// only go the length of the old array
	printf("\nRight BEFORE we do the OR operation, AI contains: \n");
		for (int i = 0; i < ai->length; i++) {
		printf("number: %lu\n", ai->arr[i]);
	}
	
	for (int i = 0; i < ap->length; i++) {
		ai->arr[i] = ap->arr[i] | ai->arr[i];
		
	}
	printf("\nRight after we do the OR operation, AP contains: \n");
		for (int i = 0; i < ap->length; i++) {
		printf("number: %lu\n", ap->arr[i]);
	}
	//printf("\nRight after we do the OR operation, ai contains: \n");
	for (int i = 0; i < ai->length; i++) {
		//printf("number: %lu\n", ai->arr[i]);
	}
	// if the last element of the new array is zero, get rid of the last element
	if (ai->arr[ai->length - 1] == 0) {
		ai->arr[ai->length - 1] = NULL;
		ai->length = ai->length - 1; 
	}
	// before we add the extra zeros, let's see what the shifted values are
	printf("\nBefore we add the extra zeros, the shifted stuff is: \n");
	for (int i = 0; i < ai->length; i++) {
		printf("number: %lu\n", ai->arr[i]);
	}
	//printf("%s\n", apint_format_as_hex(ai));
	//for (int i = 0; i < ai->length; i++) {
	//	printf("Entered the for loop\n");
	//	printf("%s\n", apint_format_as_hex(ai->arr[i]));
	//}
	if (extraSpaces != 0) {
		printf("\nIn the process of adding on extra spaces to the end.\n");
		ApInt * finalAi = malloc(sizeof(ApInt));
		finalAi->length = ai->length + extraSpaces;
		finalAi->arr = malloc(finalAi->length * sizeof(uint64_t));
		// fill the extra spaces with zeros
		for (int i = 0; i < extraSpaces; i++) {
			finalAi->arr[i] = 0;
			//printf("filling in extra zeros at end\n");
		}
		for (int i = 0; i < ai->length; i++) {
			finalAi->arr[i + extraSpaces] = ai->arr[i];
			//printf("filling in real numbers from original");
		}
		return finalAi; 
	}
	/*
	if (ai->arr[ai->length - 1] == 1) {
		ai->arr[ai->length - 1] = NULL;
		ai->length = ai->length - 1;
	}
	*/
	return ai;
	
	//else {
	//	printf("The shift was larger than 64.\n");
	//	return NULL;
	//}
	//assert(0);
	//return NULL;
}

/*
ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
	
	// Create a deep copy of the passed in ApInt
	int extraSpaces = n / 64; 
	int realShifts = n % 64;
	if (n < 64) {
	ApInt * ai = malloc(sizeof(ApInt));
	ai->length = ap->length + 1;
	// Make the new array one element longer
  	ai->arr = malloc((ai->length) * sizeof(uint64_t));
	// Make the first element of the new array zero 
	ai->arr[0] = 0; 
	//printf("\nThe length of the new array is %d\n", ai->length);
	// Copy over elements with each element one index greater
	for (int i = 0; i < ap->length; i++) {
		//printf("entered first for loop\n");
		//printf("trying to store: %lu \n", ap->arr[i]);
		//apint_format_as_hex(ap->arr[i]);
		ai->arr[i + 1] = ap->arr[i]; //>> (64 - n);
		
	}
	//printf("The contents of the new array:\n");
	// shift all the contents in the new array over by 64 - n 
	for (int i = 0; i < ai->length; i++) {
		//apint_format_as_hex(ai->arr[i]);
		//printf("num: %lu \n", ai->arr[i]);
	}
	for (int i = 0; i < ai->length; i++) {
		ai->arr[i] = ai->arr[i] >> (64 - n);
	}
	// shift all the contents of the old array by n 
	for (int i = 0; i < ap->length; i++) {
		ap->arr[i] = ap->arr[i] << n;
		//ai->arr[i] = ap->arr[i] & ai->arr[i];
	}
	// add the elements of the array together with the OR operation
	// only go the length of the old array
	for (int i = 0; i < ap->length; i++) {
		ai->arr[i] = ap->arr[i] | ai->arr[i];
	}
	// if the last element of the new array is zero, get rid of the last element
	if (ai->arr[ai->length - 1] == 0) {
		ai->arr[ai->length - 1] = NULL;
		ai->length = ai->length - 1; 
	}
	return ai;
	}
	else {
		printf("The shift was larger than 64.\n");
		return NULL;
	}
	//assert(0);
	//return NULL;
}*/


char *apint_format_as_hex(ApInt *ap) {
	/* TODO: implement */
	if ((ap->length == 1) && (ap->arr[0] == 0)) {
		char * result = malloc(2);
		result[0] = '0';
		result[1] = '\0';
		//printf("ENTERED SPECIAL CASE\n");
		return result;
	}
	char hexString[16 * ap->length + 1];
	//printf("\nAllotted %d elements for new string\n", strlen(hexString));
	char alphabet[] = {'0','1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', '\0'};
	int hexStringIndex = 0; 
	//printf("\nThe length ApInt passed in: %d\n", ap->length);
	/*
	int allZeros = 0; 
	for (int i = 0; i < ap->length; i++) {
		if (ap->arr[i] != 0) {
			allZeros = 1;
		}
	}
	if (allZeros == 0) {
		printf("ENTERD THE ALLZEROS BLOCK \n");
		char * result = malloc(sizeof(char));
		result[0] = '0';
		return result;
	}
	*/
	for (int i = 0; i < ap->length; i++) {
		uint64_t tempNumber = ap->arr[i];
		//printf("The number in the current element: %lu\n", tempNumber);
		// ADDED THIS IF BLOCK FOR ZERO EDGE CASE
		/*
		if(tempNumber == 0) {
			char * result = malloc(sizeof(int)); 
			result[0] = '\0';
			return result;
		}
		*/
		/*
		if (tempNumber == 0) {
			printf("THERE IS A CASE WHERE tempNum == 0\n");
			for (int k = 0; k < 16; k++);
				hexString[hexStringIndex] = '0';
				hexStringIndex++;
		}
		*/
		//while (tempNumber != 0) 
		for (int w = 0; w < 16; w++) {
			//printf("Entered while loop\n");
			
			uint64_t lastFourBits = tempNumber & 15;
			/*
			if (lastFourBits == 0) {
				printf("WE HIT lastFourBits = 0\n");
				printf("UGHHHH\n");
			}*/
			//printf("The value of the last four bits is %u:\n", lastFourBits);
			// shift tempNumber right by 4
			tempNumber = tempNumber >> 4;
			char tempChar = alphabet[lastFourBits];
			hexString[hexStringIndex] = tempChar; 
			//printf("Just added %c to the end of first hexString\n", tempChar);
			hexStringIndex++;
		}
		
	}
	hexString[hexStringIndex] = '\0';
	//printf("First hexString is %s \n", hexString);
	char * finalHexString = malloc(strlen(hexString) + 1);
	finalHexString[strlen(hexString)] = '\0';
	for (int i = 0; i < strlen(hexString); i++) {
		//printf("ENTERED FOR LOOP\n");
		//printf("Added %c to the end of final string\n", hexString[strlen(hexString) - 1 - i]);
		finalHexString[i] = hexString[strlen(hexString) - 1 - i];
		//printf("Just added %c to the final hexString\n", finalHexString[i]);
	}
	
	//printf("%s", finalHexString);
	int leadingZeros = 0;
	while (finalHexString[leadingZeros] == '0') {
		leadingZeros++;
	}
	//printf("The number of leading zeros is: %d\n", leadingZeros);
	//printf("We are trying to allocate: %d\n", strlen(finalHexString) + 1 - leadingZeros);
	char * retHexString = malloc(strlen(finalHexString) + 1 - leadingZeros);
	
	retHexString[strlen(retHexString) - 1] = '\0';
	//printf("The length of retHexString is: %d\n", strlen(retHexString));
	
	for (int i = 0; i < strlen(finalHexString) + 1 - leadingZeros; i++) {
		//printf("Entered the goddamn for loop\n");
		retHexString[i] = finalHexString[leadingZeros + i];
	}
	return retHexString;
	return finalHexString;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
	// figure out maximum length length is larger
	int maxLength = 0;  
	if (a->length > b->length) {
		 maxLength = a->length;
	}
	else if (a->length < b->length) {
		maxLength = b->length;
	}
	else {
		maxLength = a->length;
	}
	//printf("\nmaxLength = %d\n", maxLength);
	
	ApInt * result = malloc(sizeof(ApInt));
	result->length = maxLength + 1;
	// Make the new array one element longer
  	result->arr = malloc((result->length) * sizeof(uint64_t));
	
	//make the result ai array be larger length + 1
	// only need to iterate through the shorter length 
	int addOne = 0; 
	for (int i = 0; i < maxLength; i++) {
		uint64_t elementSum = 0;
		if (i < a->length) {
			elementSum += a->arr[i];
			//printf("Added %lu from a\n", a->arr[i]);
			//printf("elementSum after adding a: %d\n", elementSum);
		}
		if (i < b->length) {
			elementSum += b->arr[i];
			//printf("Added %lu from b\n", b->arr[i]);
		}
		if (addOne == 1) {
			elementSum++;
		}
		result->arr[i] = elementSum;
		//printf("elementSum was: %lu\n", elementSum);
		if (elementSum < a->arr[i]) {
			addOne = 1; 
		}
		else {
			addOne = 0;
		}
	}
	// including the longer elements 
	// need to include the longer 
	if (addOne == 1) {
		result->arr[result->length - 1] = 1;
	}
	else {
		result->arr[result->length - 1] = NULL;
		result->length = result->length - 1;
	}
	//printf("addOne ended as: %d", addOne);
	return result;
	
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {
	/* TODO: implement */
	// make sure that a is indeed at least as big as b 
	if (apint_compare(a, b) < 0) {
		
		//printf("This is not a valid subtraction operation\n");
		return NULL;
	}
	//printf("The result of compare is %d\n", apint_compare(a, b));
	// create the result ApInt object that is one element longer than a
	ApInt * result = malloc(sizeof(ApInt));
	result->length = a->length + 1;
	//printf("\nThe result length is %d\n", result->length);
	// Make the new array one element longer
  	result->arr = malloc((result->length) * sizeof(uint64_t));
	// iterate through the both arrays, including each only when index permits
	int subOne = 0; 
	for (int i = 0; i < a->length; i++) {
		uint64_t elementDiff = 0; 
		if(i < a->length) {
			elementDiff += a->arr[i];
			//printf("contributed %lu from a\n", a->arr[i]);
		}
		if (i < b->length) { 
			elementDiff -= b->arr[i];
			//printf("contributed %lu from b\n", b->arr[i]);
		}
		if (subOne == -1) {
			elementDiff--;
		}
		// store the temporary difference into current element
		result->arr[i] = elementDiff;
		if (elementDiff > a->arr[i]) {
			subOne = -1; 
		}
		else {
			subOne = 0;
		}
	}

	// if we still need to carry over a borrow, fill the last element with -1
	if (subOne == -1) {
		result->arr[result->length - 1] = -1;
	}
	else {
		result->arr[result->length - 1] = NULL;
		result->length = result->length - 1;
	}
	// see 
	return result;
	assert(0);
	return NULL;
}

int apint_compare(const ApInt *left, const ApInt *right) {
	/* TODO: implement */
	int lengthDiff = left->length - right->length;
	if (lengthDiff != 0) {
		return lengthDiff;
	}
	for (int i = left->length - 1; i >=0; i--) {
		uint64_t leftTemp = left->arr[i]; 
		uint64_t rightTemp = right->arr[i];
		uint64_t elemDiff = leftTemp - rightTemp;
		//printf("The elementDiff is %lu - %lu = %lu\n", leftTemp, rightTemp, elemDiff);
		// need to account for underflow
		if (elemDiff > leftTemp) {
			//printf("Entered underflow condition\n");
			return -1;
		}
		if (elemDiff < 0) {
			//printf("The element difference for compare is negative\n");
			//return elemDiff;
			return -1;
		}
		if (elemDiff > 0) {
			//printf("The element difference for compare is positive\n");
			return 1;
		}
	}

	//assert(0);
	// If you get through all elements and observe that they are all equal, return 0 to indicate equality
	return 0;
}

