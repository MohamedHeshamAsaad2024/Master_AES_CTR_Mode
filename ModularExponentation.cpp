#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <cassert>
#include <tuple>
#include <cstdint>

long long RSA_ModularExponentiation(long long base, long long exponent, long long modulus)
{
    /* Initialization Step: Start with result of 1 */
    long long result = 1;

    /* Initialization Step: Start with base  of base % modulus */
    base = base % modulus;

    /* Begin as long as the exponent is not equal to zero */
    while (exponent > 0)
    {
        /* Check if the current bit (LSB) is set to 1*/
        if (exponent % 2 == 1)
            /* Update the result by multipling the previous result by the base and reduce it by the modulus */
            result = (result * base) % modulus;
        
        /* Update the base by multipling the base by itself and reduce it by the modulus */
        base = (base * base) % modulus;

        /* Shift right to consider the next bit. Essentially, halving the square */
        exponent = exponent >> 1;
    }

    /* Return the result */
    return result;
}

int main()
{
    long long base = 31;
    long long exponent = 17;
    long long modulus = 2773;
    long long privateKeyExponent = 157;
    //encrypt
    long long output = RSA_ModularExponentiation(base, exponent, modulus);
    std::cout << output << std::endl;
    //decrypt
    long long decryptedOutput = RSA_ModularExponentiation(output, privateKeyExponent, modulus); 
    std::cout << decryptedOutput << std::endl;
    return 0;
}
