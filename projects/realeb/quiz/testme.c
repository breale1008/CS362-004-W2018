#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

/*
*	Function:	inputChar
*	Author:		Brian Reale
*	Purpose:	generate a single random printable character
*	Date:		2/3/18
*/
char inputChar()
{
	// generate random char in ASCII range [32, 126] (printable chars)
	int min = 32;
	int max = 126;
	return min + rand() % (max - min + 1);
}

/*
*	Function:	inputString
*	Author:		Brian Reale
*	Purpose:	generate a random input string for testing. 1/10 chance to be 'reset', 9/10 chance to be random
*	Date:		2/3/18
*/
char *inputString()
{
	char* randStr;

	// 1/10 chance to send 'reset' as string
	int n = rand() % 10;
	if (n == 0) {

		randStr = malloc(5 * sizeof(char) + 1);
		randStr[0] = 'r';
		randStr[1] = 'e';
		randStr[2] = 's';
		randStr[3] = 'e';
		randStr[4] = 't';
		randStr[5] = '\0';
	}

	// otherwise, random string with random length [1,10]
	else {

		int strLen = rand() % 10 + 1;
		randStr = malloc(strLen * sizeof(char) + 1);

		int x;
		for (x = 0; x < strLen; x++) {

			randStr[x] = inputChar();
		}
		randStr[strLen] = '\0';
	}

    return randStr;
}

/*
*	Function:	testme
*	Author:		OSU
*	Purpose:	some kind of arbitrary state-based thing to be tested
*	Date:		2/3/18
*/
void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
	  free(s);
      exit(200);
    }

	free(s);
  }
}

/*
*	Function:	tester
*	Author:		Brian Reale
*	Purpose:	runs testme() a few times
*	Date:		2/3/18
*/
void tester() {

	int testCount;
	for (testCount = 0; testCount < 10; testCount++) {

		testme();
		printf("\n");  // will force error message to print if still in buffer
	}
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    tester();
    return 0;
}
