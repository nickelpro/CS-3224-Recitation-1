#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Prints a region of memory to stdout as hex digits
 *
 * @param src: starting address in memory to print
 * @param len: the number of bytes to print
 */
void hex(const void* src, size_t len) {
  if(len)
    printf("%02X", ((const unsigned char*) src)[0]);
  for(size_t i = 1; i < len; ++i)
    printf(" %02X", ((const unsigned char*) src)[i]);
  printf("\n");
}

/**
 * @brief Prints information about a buffer to stdout
 *
 * @note: Prints the following in order:
 *          * Buffer name
 *          * Size of the buffer
 *          * Contents of the buffer in hex
 *          * Character representation of the buffer
 *
 * @param   name: name of the buffer
 * @param buffer: starting address of the buffer
 * @param    len: size of the buffer
 */
void bufferInfo(const char* name, const void* buffer, size_t len) {
  printf("Buffer '%s':\n", name);
  printf("\tsizeof: %zu\n", len);
  printf("\tcontents: ");
  hex(buffer, len);
  if(((char*) buffer)[len - 1])
    printf("\tstring: %.*s\n", (int) len, (const char*) buffer);
  else
    printf("\tstring: %s\n", (const char*) buffer);
}

char* myRealloc(char* buffer, size_t oldSize, size_t newSize) {
  char* newBuffer = malloc(newSize);

  // These two assignment loops do the same thing
  for(size_t i = 0; i < oldSize; i++)
    newBuffer[i] = buffer[i];

  // char* a = newBuffer;
  // char* b = buffer;
  // for(char* end = b + oldSize; b != end;)
  //   *a++ = *b++;

  free(buffer);
  return newBuffer;
}

// Things types tell us:
//   * The size in memory
//   * The meaning of the associated variable

int main(int argc, char* argv[]) {
  // Integral Types:
  /**
   * // Interpreted using two's-complement
   * signed char c; // At least one byte
   * short s;       // At least two bytes
   * int i;         // At least two bytes, traditionally 4 bytes
   * long l;        // At least four bytes
   * long long ll;  // At least eight bytes
   */

  /**
   * // Come from stdint.h
   * int8_t i8;   // Always one byte
   * int16_t i16; // Always two bytes
   * int32_t i32; // Always four bytes
   *
   * // Interpreted as regular binary numbers
   * unsigned char c;       // At least one byte
   * unsigned short s;      // At least two bytes
   * unsigned int i;        // At least two bytes, traditionally 4 bytes
   * unsigned long l;       // At least four bytes
   * unsigned long long ll; // At least eight bytes
   *
   * // Interpeted as IEEE floating point
   * float f;  // Always four bytes
   * double d; // Always eight bytes
   */

  // lvalues: Expressions that are allowed to appear
  //          on the left-side of an equal sign.

  // integer variables are lvalues
  // "myInt" is an non-const integer variable
  int myInt = 1; // "1" is an integer constant
  myInt = 5;     // "5" is an integer constant

  // rvalues: Expressions that are NOT allowed to appear
  //          on the left-side of an equal sign
  // integer constants are rvalues
  // 5 = myInt; Bad! 5 is an rvalue

  // lvalues are also allowed on the right side of equal sign
  int anotherNum = myInt;

  // Const integers are *non-modifiable* lvalues;
  const int constNum = 10;
  // constNum = 11; Bad! Non-modifiable

  // int[] is also an lvaue!
  int myIntArray[] = {0, 1, 2, 3, 4};

  // myIntArray = {5, 6, 7, 8, 9};
  // This doesn't work! Array names are non-modifiable lvalues

  // anotherNum is a modifiable lvalue
  // constNum is non-modifiable lvalue
  // This is fine, non-modifiable lvalues are allowed on the right side
  anotherNum = constNum;

  /*
  C 2018 6.3.2.1.2

  Except when it is the operand of the sizeof operator, or the unary &
  operator, or is a string literal used to initialize an array, an expression
  that has type “array of type” is converted to an expression with type
  “pointer to type” that points to the initial element of the array object and
  is not an lvalue.
  */

  // int copyArray[5] = myIntArray;
  // Doesn't work! myIntArray is an array expression, and is converted
  // to "pointer to int"

  int* intPtr = myIntArray; // Fine, myIntArray is converted to pointer to int

  /**
   * // Only a pointer was copied, myIntArray and intPtr now point to the same
   * // my memory address
   * printf("myIntArray: %p\n", (void*) myIntArray);
   * printf("intPtr: %p\n", (void*) intPtr);
   */

  int dereference;

  // These three are all equivalent
  dereference = *intPtr;
  dereference = intPtr[0];
  dereference = *(intPtr + 0);

  // And these two are equivalent
  dereference = intPtr[1];
  dereference = *(intPtr + 1);

  /**
   * for(int i = 0; i < 5; i++) {
   *   printf("myIntArray[%d] == %d\n", i, *(myIntArray + i));
   *   printf("intPtr[%d] == %d\n", i, intPtr[i]);
   * }
   */

  // When performing pointer arithmatic, each unit
  // advanced is equal to sizeof the underlying type
  /**
   * printf("sizeof int is: %zu\n", sizeof(int));
   * for(int i = 0; i < 5; i++) {
   *   printf("myIntArray[%d] at address: %p\n", i, (void*) (myIntArray + i));
   * }
   */

  // These are equivalent
  char myChar;
  myChar = 'a';
  myChar = 97;

  // What is the type??
  // "Hello World!"

  /**
   * char hello[] = "Hello World!";
   * // note: char hello[] is being treated as a pointer
   * // in this function call. You cannot pass arrays
   * // to functions, because they get converted to pointers
   * bufferInfo("hello", hello, sizeof(hello));
   *
   * char singleByte[] = "\xFF";
   * bufferInfo("singleByte", singleByte, sizeof(singleByte));
   *
   * char happy[] = "\xF0\x9F\x98\x8A";
   * bufferInfo("happy", happy, sizeof(happy));
   *
   * char sad[] = "🙁";
   * bufferInfo("sad", sad, sizeof(sad));
   *
   * char bye[] = {'B', 'y', 'e', '!'};
   * bufferInfo("bye", bye, sizeof(bye));
   */

  // What if we don't know size ahead of time? Like reading a file.
  FILE* fd = fopen("mytext", "r");

  if(!fd) {
    printf("Can't find mytext\n");
    return 1;
  }

  size_t bufSize = 20; // Make a guess (20 is a very small guess)
  size_t bytesRead = 0;

  // Keep track with bufSize and bytesRead
  char* myBuf = malloc(bufSize);

  int result = fread(myBuf, 1, bufSize, fd);
  while(result > 0) {
    bytesRead += result;
    if(bytesRead == bufSize) { // Buffer is full
      size_t oldSize = bufSize;
      bufSize *= 2; // New guess, double the old guess
      myBuf = myRealloc(myBuf, oldSize, bufSize); // Need a new buffer
    }

    // myBuf + byteRead: Pointer arithmatic, returns a pointer to the next
    // empty byte
    // bufSize - bytesRead: The number of unfilled bytes left in the
    // buffer
    result = fread(myBuf + bytesRead, 1, bufSize - bytesRead, fd);
  }

  // Make room for a null terminator, printf expects a null terminator
  if(bytesRead == bufSize)
    myBuf = myRealloc(myBuf, bufSize, bufSize + 1);

  myBuf[bytesRead] = 0; // Attach a null terminator to a string
  printf("%s", myBuf);
}
