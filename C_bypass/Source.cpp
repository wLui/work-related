#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>

using namespace std;

const int bitsPerByte = 8;

void decompress(unsigned char **data_ptr, int n);
void printData(unsigned char *data, int n) {
	printf("Hex\n");
	for(int i = 0; i < n; i++)
		printf("%x, ", data[i]);
	printf("\n");
	printf("Character rep\n");
	for(int i = 0; i < n; i++)
		printf("%c, ", data[i]);
	printf("\n");
}

int main() {
	// define compressed data
	unsigned char *data = (unsigned char *) malloc(sizeof(unsigned char) * 21);
	unsigned char *actual = (unsigned char *) malloc(sizeof(unsigned char) * 21);

	data[0] = 0x8d;
	data[1] = 0x3e;

	data[2] = 0xc2;
	data[3] = 0x09;

	data[4] = 0x55;
	data[5] = 0x66;

	data[6] = 0xd0;
	data[7] = 0x8b;

	data[8] = 0x11;
	data[9] = 0x04;
	
	data[10] = 0xfa;
	data[11] = 0xd1;

	data[12] = 0x69;
	data[13] = 0x20;

	data[14] = 0xa9;
	data[15] = 0x22;

	data[16] = 0x2a;	
	data[17] = 0x09;

	data[18] = 0x90;
	data[19] = 0x69;

	data[20] = 0x59;

	printf("Compressed\n");	
	printData(data, 21);

	decompress(&data, 21);

	actual[0] = 'F';
	actual[1] = 'O';

	actual[2] = 'X';
	actual[3] = ' ';

	actual[4] = 'J';
	actual[5] = 'U';

	actual[6] = 'M';
	actual[7] = 'P';

	actual[8] = 'E';
	actual[9] = 'D';
	
	actual[10] = ' ';
	actual[11] = 'O';

	actual[12] = 'V';
	actual[13] = 'E';

	actual[14] = 'R';
	actual[15] = ' ';

	actual[16] = 'T';
	actual[17] = 'H';

	actual[18] = 'E';
	actual[19] = ' ';

	actual[20] = 'L';

	printf("Decompressed\n");
	printData(data, 21);

	printf("Actual\n");
	printData(actual, 21);

	free(data);
	data = NULL;

	free(actual);
	actual = NULL;

	return 0;
}

/*
	Try the following, for bit k < n, queue Q of bits stored
	1. Read m = min{k, bpb - 1} bits from Q and store into temp
	2. Adjust temp for or'ing with curr - L.S by bpb - m - 1
	3. Store and R.S curr m + 1 bits from Q, if we've stored less than 
	adjTotal bits
*/
void decompress(unsigned char **data_ptr, int n) {
	queue<int> q;
	/*
		note - decompression on n bytes will append n zeros to the end of the 
		decompressed result
	*/
	unsigned char curr, temp;
	int totalBits = n * bitsPerByte;

	// main variables for algorithm
	int m, adjTotal, totalPushed;
	adjTotal = totalBits - n;
	totalPushed = 0;

	// other variables for iteration
	int currPow, nPopped, tShift, nPushed;

	for(int k = 0; k < n; k++) {	
		curr = (*data_ptr)[k];	
		m = (k <= bitsPerByte - 1) ? k : bitsPerByte - 1;
		tShift = bitsPerByte - m - 1;

		temp = 0;
		currPow = 1;
		// pop m bits from q and store in temp
		for(nPopped = 0; nPopped < m && totalPushed < adjTotal; nPopped++) {
			temp = temp + (q.front() * currPow);
			q.pop();
			currPow *= 2;
		}

		// adjust temp for OR'ing
		temp = temp << tShift;
		// printf("byte: %d\n", k);	
		// printf("temp: %x\n", temp);

		// Adjust curr by storing m + 1 bits into q for now
		for(nPushed = 0; nPushed < m + 1 && totalPushed < adjTotal; nPushed++) {
			q.push(curr & 1);
			curr = curr >> 1;
			totalPushed++;
		}

		// printf("curr - before adjustment: %x\n", curr);
		curr = curr | temp;
		// printf("curr - after adjustment: %x, %c\n", curr, curr);

		(*data_ptr)[k] = curr;	
	}
	if(!q.empty()) {
		printf("queue not empty afterwards- unexpected\n");
	}

	return;
}
