#include <stdio.h>
#include <math.h>

int binaryToDecimal(int binaryArray[], int size) {
    int decimalNumber = 0;
    int power = size - 1; // The highest power of 2

    // If the most significant bit is 1, then it's a negative number
    if (binaryArray[0] == 1) {
        // Calculate the decimal equivalent for negative numbers
        for (int i = 0; i < size; i++) {
            // Flip the bits (0 to 1 and 1 to 0)
            if (binaryArray[i] == 1) {
                binaryArray[i] = 0;
            } else {
                binaryArray[i] = 1;
            }
        }

        // Add 1 to the binary number
        int carry = 1;
        for (int i = size - 1; i >= 0; i--) {
            binaryArray[i] += carry;
            if (binaryArray[i] > 1) {
                binaryArray[i] = 0;
                carry = 1;
            } else {
                carry = 0;
            }
        }

        // Calculate the two's complement
        decimalNumber = binaryToDecimal(binaryArray, size);
        decimalNumber = -decimalNumber;
    } else {
        // Calculate the decimal equivalent for positive numbers
        for (int i = 0; i < size; i++) {
            // Calculate the decimal equivalent by multiplying the bit value with 2 raised to the corresponding power
            decimalNumber += binaryArray[i] * pow(2, power);
            power--; // Decrement the power for the next bit
        }
    }

    return decimalNumber;
}

int main() {
    int number[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; // Example array representing signed binary number -111111111
    int size = sizeof(number) / sizeof(number[0]); // Calculate the size of the array

    int decimalNumber = binaryToDecimal(number, size);
    printf("Decimal equivalent: %d\n", decimalNumber);

    return 0;
}
