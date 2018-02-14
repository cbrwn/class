#include <ctime>
#include <iostream>

void printArray(int nums[], int length);

int main()
{
	srand((unsigned int)time(NULL));
	const int arrayLength = 10;
	const int maxValue = 20000;
	int numbers[arrayLength];
	int toSearch = -1; // the number we will search for in binary search

	while (true)
	{
		clock_t tmr;
		double genTime, sortTime, searchTime;
		tmr = clock(); // start timer
		printf("Generating numbers...\n");

		for (int i = 0; i < arrayLength; i++)
		{
			numbers[i] = rand() % maxValue;

			// make sure we set the number we're searching for to a number that exists in the array
			toSearch = numbers[i];
		}
		
		tmr = clock() - tmr; // stop timer
		genTime = (((double)tmr) / CLOCKS_PER_SEC) * 1000.0f;

		printf("Generated: ");
		printArray(numbers, arrayLength);

		printf("Ordering numbers...\n");
		tmr = clock(); // start timer
		// order numbers - bubble sort I think idk
		// unordered keeps track if the array is ordered or not
		bool unordered = true;
		int pass = 0; // count passes
		while (unordered)
		{
			pass++;
			// this will be set to true in the for loop if needed
			unordered = false;
			for (int i = 0; i < arrayLength - 1; i++)
			{
				int thisNum = numbers[i];
				int nextNum = numbers[i + 1];

				// compare these numbers and switch them if they're out of order
				if (nextNum < thisNum)
				{
					numbers[i] = nextNum;
					numbers[i + 1] = thisNum;
					// keep track that we've changed the array so it's still not finished
					unordered = true;
					break;
				}
			}
		}
		tmr = clock() - tmr; // stop timer
		sortTime = (((double)tmr) / CLOCKS_PER_SEC) * 1000.0f;

		printf("Ordered:   ");
		printArray(numbers, arrayLength);

		printf("Ordered in %d passes\n\n", pass);

		printf("Searching for %d...\n", toSearch);
		tmr = clock(); // start timer
		// binary search for toSearch
		int minIndex = 0, maxIndex = arrayLength;
		int midIndex = maxIndex / 2;
		while (numbers[midIndex] != toSearch)
		{
			// compare our search value to the middle index
			if (toSearch > numbers[midIndex])
				minIndex = midIndex;
			else if (toSearch < numbers[midIndex])
				maxIndex = midIndex;

			// update middle index
			midIndex = (minIndex + maxIndex) / 2;
		}
		tmr = clock() - tmr; // stop timer
		searchTime = (((double)tmr) / CLOCKS_PER_SEC) * 1000.0f;
		 
		printf("Index of %d is %d!\nTimes:\n  - Gen:    %.0fms\n  - Sort:   %.0fms\n  - Search: %.0fms\n===========================\n", toSearch, midIndex, genTime, sortTime, searchTime);

		system("pause>nul");
	}
	return 0;
}

//=============================================
// prints an array of integers in this format:
//     [10, 40, 3, 9]
// param:
//		int nums[] - array of numbers
//      int length - length of array
//=============================================
void printArray(int nums[], int length)
{
	printf("[");
	for (int i = 0; i < length; i++)
	{
		printf("%d", nums[i]);
		// don't print out the comma for the last item
		if (i != length - 1)
			printf(", ");
	}
	printf("]\n\n");
}