/*************************************************************
 * File Name: RSA_2048
 * Description:
 *  PKCS VERSION 1.5 Padding scheme implementation
 *  
 * Implemented by 
 *  Abdelrahman Gamil
 *  Eslam abdelsamea
 *  Mohamed eldehamy 
 *  Mohamed Hesham Kamaly
 ************************************************************/
/*************************************************************
 *                      I N C L U D E S                      *
 *************************************************************/
#include <iostream>
#include <vector>
#include <random>
#include <sstream>
#include <cassert>
#include <tuple>
#include <gmpxx.h>

/*************************************************************
 *                     N A M E S P A C E S                   *
 *************************************************************/
using namespace std;

/*************************************************************
 *                     P R O T O T Y P E S                   *
 *************************************************************/
mpz_class RSA_PKCS_1_encrypt(const mpz_class message, const mpz_class publicExponent, const mpz_class modulus);
mpz_class RSA_PKCS_1_decrypt(const mpz_class cipher, const mpz_class privateExponent, const mpz_class modulus);
mpz_class modularExponentiation(mpz_class base, mpz_class exponent, mpz_class modulus);

/*************************************************************
 *                           M A I N                         *
 *************************************************************/
int main() 
{
    /* Declare placeholder for the input plain text message */
    mpz_class message;

    /* Declare placeholder for the padded plain text message */
    mpz_class Padded_message;

    /* Declare placeholder for the cipher text message after encryption of padded plain text */
    mpz_class ciphertext;
    
    /* Declare placeholder for the decrypted padded plain text message */
    mpz_class decryptedPaddedMessage;

    /* Declare placeholder for the decrypted plain text message */
    mpz_class decryptedMessage;

    // Display a message indicating that RSA key generation is in progress
    cout << "Generating RSA keys (2048-bit)..." << endl;

    // Generate RSA keys (public modulus, public exponent, and private exponent)
    //auto [Public_Modulus, Public_Exponent, Private_Exponent] = generateKeys();
    mpz_class Public_Modulus("2773");
    mpz_class Public_Exponent("17");
    mpz_class Private_Exponent("157");

    // Display the generated RSA keys
    gmp_printf("Public Key (Public_Modulus, Public_Exponent): (%Zd, %Zd)\n", Public_Modulus, Public_Exponent);
    gmp_printf("Private Key (Private_Exponent): (%Zd)\n", Private_Exponent);

    message = "31";
    //mpz_init_set_str(message, "This is the message that needed to be encrypted", 10);

    // Display the original message
    gmp_printf("Original message: %Zd\n", message);

    // Convert the ordinary message to a padded message
    // ConvertOrdinaryMessageToPaddedMessage(Padded_message, message);
    gmp_printf("Padded message: %Zd\n", Padded_message);

    // Encrypt the padded message using the public key
    ciphertext = RSA_PKCS_1_encrypt(message, Public_Exponent, Public_Modulus);
    gmp_printf("Encrypted message: %Zd\n", ciphertext);

    // Decrypt the encrypted message using the private key
    decryptedMessage = RSA_PKCS_1_decrypt(ciphertext, Private_Exponent, Public_Modulus);
    gmp_printf("Decrypted padded message: %Zd\n", decryptedPaddedMessage);

    // Convert the decrypted padded message back to the original message
    // ConvertPaddedMessageToOrdinaryMessage(decryptedMessage, decryptedPaddedMessage);
    gmp_printf("Decrypted message: %Zd\n", decryptedMessage);

    return 0;
}

/*************************************************************
 *                       F U N C T I O N S                   *
 *************************************************************/
// Encrypts a message using RSA with PKCS #1 padding (Stub function for demonstration purposes)
mpz_class RSA_PKCS_1_encrypt(const mpz_class message, const mpz_class publicExponent, const mpz_class modulus) 
{
    mpz_class cipherText = modularExponentiation(message, publicExponent, modulus);
    
    return cipherText;
}

// Decrypts a message using RSA with PKCS #1 padding (Stub function for demonstration purposes)
mpz_class RSA_PKCS_1_decrypt(const mpz_class cipher, const mpz_class privateExponent, const mpz_class modulus) 
{
    mpz_class plainText = modularExponentiation(cipher, privateExponent, modulus);
    
    return plainText;
}

mpz_class modularExponentiation(mpz_class base, mpz_class exponent, mpz_class modulus)
{
    /* Initialization Step: Start with result of 1 */
    mpz_class result = 1;

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