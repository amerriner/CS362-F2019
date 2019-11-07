#include "container.o"
#include "library.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
	struct container originalContainer, testContainer1, &testContainer2;
	int sizeDecreasedBy, sizeIncreasedBy;
	originalContainer = newContainer();
	add(5, &originalContainer);
	add(2, &originalContainer);
	add(7, &originalContainer);
	add(31, &originalContainer);
	add(22, &originalContainer);
	add(2, &originalContainer);
	add(5, &originalContainer);
	add(5, &originalContainer);
	int originalSize = size(&originalContainer);


	printf("***********TESTING ADD AND REMOVE************\n");
	sizeDecreasedBy = 0;
	sizeIncreasedBy = 0;
	int myValue = MAX_VALUE + 1;
	memcpy(&testContainer1, &originalContainer, sizeof(originalContainer));
	memcpy(&testContainer2, &originalContainer, sizeof(originalContainer));
	
	add(myValue, &testContainer1);
	int j = remove(myValue, &testContainer2);

	int testSize1 = size(&testContainer1);
	int expectedSize1 = originalSize + sizeIncreasedBy;
	int testSize2 = size(&testContainer2);
	int expectedSize2 = originalSize - sizeDecreasedBy;
	
	assert(testSize1 == expectedSize1); //assert nothing added to testContainer1
	assert(testSize2 == expectedSize2);  //assert nothing removed from testContainer2
	assert(j != 1); //remove should not have found this number in container

	printf("************TESTING COMPLETE**************\n");
	
}
