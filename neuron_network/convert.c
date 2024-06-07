/*** Đổi từ binary sang decimal có dấu ***/
int binaryToDecimalSigned(int binaryArray[], int size) {
    int decimalNumber = 0;
    int power = size - 1;

    if (binaryArray[0] == 1) {
        for (int core = 0; core < size; core++) {
            if (binaryArray[core] == 1) {
                binaryArray[core] = 0;
            } else {
                binaryArray[core] = 1;
            }
        }
        int carry = 1;
        for (int core = size - 1; core >= 0; core--) {
            binaryArray[core] += carry;
            if (binaryArray[core] > 1) {
                binaryArray[core] = 0;
                carry = 1;
            } else {
                carry = 0;
            }
        }
        decimalNumber =  binaryToDecimalSigned(binaryArray, size);
        decimalNumber = - decimalNumber;
    } else {
        for (int core = 0; core < size; core++) {
            decimalNumber += binaryArray[core] * pow(2, power);
            power --;
        }
    }

    return decimalNumber;
}

/*** Đổi từ binary sang decimal không dấu ***/
int binaryToDecimalUnsigned(int binaryArray[], int size) {
    int decimalNumber = 0;
    int power = size - 1;
    for (int core = 0; core < size; core++) {
        decimalNumber += binaryArray[core] * pow(2, power);
        power--;
    }

    return decimalNumber;
}