#include "lexer.h"

void remove_quotes(char* src, char* dest)
{
	int n = strlen(src);
	if (n <= 1) 
	{
		strcpy(dest, src);
		return ;
	}
		
	char lastquote = 0;
	int i = -1, j = 0;

	// for ( i=0; i < n; i++)
	while (++i < n)
	{
		char c = src[i];
		if ((c == 39 || c == 34) && lastquote == 0)
			lastquote = c;
		else if (c == lastquote)
			lastquote = 0;
		else
			dest[j++] = c;
	}
	dest[j] = 0;
}

int main()
{
    char test1[] = "e'c'h'o";
    /*char test2[] = "'Hello', \"World!\"";
    char test3[] = "''";
    char test4[] = "\"\"";
    char test5[] = "A 'B\"C' D";*/
    
    char result1[100], result2[100], result3[100], result4[100], result5[100];
    
    remove_quotes(test1, result1);
    /*remove_quotes(test2, result2);
    remove_quotes(test3, result3);
    remove_quotes(test4, result4);
    remove_quotes(test5, result5);*/
    
    printf("Test 1: [%s] \t-> [%s]\n", test1, result1);
    /*printf("Test 2: [%s] \t-> [%s]\n", test2, result2);
    printf("Test 3: [%s] \t-> [%s]\n", test3, result3);
    printf("Test 4: [%s] \t-> [%s]\n", test4, result4);
    printf("Test 5: [%s] \t-> [%s]\n", test5, result5);*/
    
    return (0);
}