#ifndef STRING_H
#define STRING_H
// In work... //

char*   mystrcat(char* destination, char* source, size_t size);
void*   mymemset(void* array, const uint8_t value);
int8_t  mystrcmp(const char* array1, const char* array2, size_t size);
uint8_t mystrlen(const char* array);

char* mystrcat(char* destination, char* source, size_t size)
{
    char* pointer = destination + mystrlen(destination); // Point to the destination string
    while (size--)                                       // Concatenate the char into the string
		*pointer++ = *source++;                          // Update destination pointer with source pointer
    *pointer = '\0';                                     // Null terminate the pointer
    return destination;                                  // Return the destination string
}

void* mymemset(void* array, const uint8_t value)
{
    uint8_t* pointer = array;        // Point to the address of array
    while (*pointer)                 // While not a null
        *pointer++ = (uint8_t)value; // Put value into address
    return array;                    // Return the array
}

int8_t mystrcmp(const char* array1, const char* array2, size_t size)
{
    while(*array1++ == *array2++) // While array elements are equal
    --size;                       // Get n closer to 0
    if (size == 0)
    return 0;                     // Return 0 if both arrays are equal
    return 1;                     // Return 1 if arrays are different
}

uint8_t mystrlen(const char* array)
{
    uint8_t length = 0; // Set length to 0 by default
    while(*array++)     // While not a null
        length++;       // Increase the length
    return length;      // Return the length
}

#endif
