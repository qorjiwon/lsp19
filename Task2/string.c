#include <stdio.h>

// Function to calculate the length of a string using pointer
int strlen_p(char *str) {
   int	len = 0; // Initialize a counter for the length
   while (*str++)  {
	  len++; // Increment the counter for each character
	   }
		return len; // Return the length of the string
}

// Function to calculate the length of a string using array
int strlen_a(char str[]) {
  int i; // Declare an integer variable to serve as an index
   for (i = 0; str[i]; i++)
   ; // Iterate through the characters in the array until the null character is encountered
   return i; // Return the length of the string
}

// Function to copy one string to another using pointer
void strcpy_p(char *dst, char *src) {
  while (*src)
	 *dst++ = *src++; // Copy each character
  *dst = *src; // Copy the null character
}


// Function to copy one string to another using array
void strcpy_a(char dst[], char src[]) {
  int i; // Declare an integer variable to serve as an index
   for (i = 0; dst[i]; i++) // Iterate through the characters in the source array until the null character is encountered
	  dst[i] = src[i]; // Copy each character
   dst[i] = src[i]; // Copy the null character
}

// Function to concatenate two strings using pointer
void strcat_p(char *dst, char *src) {
 // Move the destination pointer to the end of the destination string
  while (*dst++)
	 ;
  dst--; // Move back one position to the null character of the destination string

  // Copy characters from the source string to the destination string
  while (*src)
	  *dst++ = *src++; // Copy each character from the source to the destination
  *dst = *src; // Copy the null character from the source to mark the end of the concatenated string
}

// Function to concatenate two strings using array
void strcat_a(char dst[], char src[]) {
  int i, j;
  // Find the end of the destination string
   for (i = 0 ; dst[i]; i++)
	  ;
  // Copy characters from the source string to the destination string
   for (j = 0 ; src[j]; j++)
	  dst[i+j] = src[j]; // Copy each character from the source to the destination
   dst[i+j] = src[j]; // Copy the null character from the source to mark the end of the concatenated string
}

// Function to compare two strings using pointer
int strcmp_p(char *dst, char *src){
     while (!(*dst-*src) && *dst++ && *src++)
	          ;
	 return *dst-*src; // Return the difference of the first differing characters
}

// Function to compare two strings using array
int strcmp_a(char dst[], char src[]){
     int i;
	 // Iterate through both strings until a differing character is found or until the end of either string is reached
	 for (i = 0;!(dst[i]-src[i])&&dst[i]&&src[i]; i++)
			   ;
	 return dst[i]-src[i]; // Return the difference of the first differing characters
}

int main() {
  int	len1, len2;
  char	str1[20], str2[20];

  len1 = strlen_p("Hello");
  len2 = strlen_a("Hello");
  printf("strlen: p=%d, a=%d\n", len1, len2);

  strcpy_p(str1, "Hello");
  strcpy_a(str2, "Hello");
  printf("strcpy: p=%s, a=%s\n", str1, str2);

  strcat_p(str1, ", World!");
  strcat_a(str2, ", World!");
  printf("strcat: p=%s, a=%s\n", str1, str2);

// Compare str1 and str2 using strcmp_p and strcmp_a, and print the results
 printf("strcmp_p: The Comparison result between '%s' and '%s' is %d\n", str1, "Hello, World!", strcmp_p(str1, "Hello, World!"));
 printf("strcmp_p: The Comparison result between '%s' and '%s' is %d\n", str1, "good", strcmp_p(str1, "good"));
 printf("strcmp_a: The Comparison result between '%s' and '%s' is %d\n", str1, "Hello, World!", strcmp_a(str1, "Hello, World!"));
 printf("strcmp_a: The Comparison result between '%s' and '%s' is %d\n", str1, "good", strcmp_a(str1, "good"));
}
