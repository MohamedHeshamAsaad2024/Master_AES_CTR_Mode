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
void generateKeys(mpz_class &Public_Modulus, mpz_class &Public_Exponent, mpz_class &Private_Exponent);
void computePhi(mpz_class result, const mpz_class PrimeNumber_1, const mpz_class PrimeNumber_2);
void computeGCD(mpz_class result, const mpz_class num1, const mpz_class num2);
void generateTwoPrimeNumbers(mpz_class PrimeNumber_1,mpz_class PrimeNumber_2);
void ConvertOrdinaryMessageToPaddedMessage(mpz_class PaddedMessage, const mpz_class OrdinaryMessage);
void ConvertPaddedMessageToOrdinaryMessage(mpz_class OrdinaryMessage, const mpz_class PaddedMessage);
void generateRandomNumber(mpz_class randomNumber);
bool isPrime(const mpz_class randomNumber);
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

    /* Declare placeholders for the keys */
    mpz_class Public_Modulus;
    mpz_class Public_Exponent;
    mpz_class Private_Exponent;

    // Display a message indicating that RSA key generation is in progress
    cout << "Generating RSA keys (2048-bit)..." << endl;

    // Generate RSA keys (public modulus, public exponent, and private exponent)
    generateKeys(Public_Modulus, Public_Exponent, Private_Exponent);

    // Display the generated RSA keys
    gmp_printf("Public Key (Public_Modulus, Public_Exponent): (%Zd, %Zd)\n", Public_Modulus, Public_Exponent);
    gmp_printf("Private Key (Private_Exponent): (%Zd)\n", Private_Exponent);

    // Set the message to be encrypted
    message.set_str("31", 10); 

    // Display the original message
    gmp_printf("Original message: %Zd\n", message);

    // Convert the ordinary message to a padded message
    // ConvertOrdinaryMessageToPaddedMessage(Padded_message, message);
    // gmp_printf("Padded message: %Zd\n", Padded_message);

    // Encrypt the padded message using the public key
    ciphertext = RSA_PKCS_1_encrypt(message, Public_Exponent, Public_Modulus);
    gmp_printf("Encrypted message: %Zd\n", ciphertext);

    // Decrypt the encrypted message using the private key
    decryptedMessage = RSA_PKCS_1_decrypt(ciphertext, Private_Exponent, Public_Modulus);
    gmp_printf("Decrypted padded message: %Zd\n", decryptedPaddedMessage);

    // Convert the decrypted padded message back to the original message
    // ConvertPaddedMessageToOrdinaryMessage(decryptedMessage, decryptedPaddedMessage);
    // gmp_printf("Decrypted message: %Zd\n", decryptedMessage);

    return 0;
}

/*************************************************************
 *                       F U N C T I O N S                   *
 *************************************************************/
// Generates RSA keys: public and private (Stub function for demonstration purposes)
// Kamaly's responsibility
void generateKeys(mpz_class &Public_Modulus, mpz_class &Public_Exponent, mpz_class &Private_Exponent) 
{
    /* mpz_class Generted_PrimeNumber_1, Generted_PrimeNumber_2, phi,temp;
    mpz_inits(Public_Exponent, Private_Exponent, Public_Modulus,Generted_PrimeNumber_1,Generted_PrimeNumber_2,phi,temp, nullptr);
    bool Private_Exponent_Is_Found="No"
    //Generate two prime numbers
    generateTwoPrimeNumbers(Generted_PrimeNumber_1,Generted_PrimeNumber_2);
    //Calculate Phi value 
    computePhi(phi,Generted_PrimeNumber_1,Generted_PrimeNumber_2);

    //Calculate Public_Modulus
    pz_mul(Public_Modulus, Generted_PrimeNumber_1, Generted_PrimeNumber_2);
    //Set  Public_Modulus to make calculation faster and optmize implementation
    mpz_set_str(Public_Exponent,"65537",16);

    //Calculate Private_Exponent
    while("Yes"==Private_Exponent_Is_Found)
    {
        mpz_mul(temp, PrimeNumber_1_minus_1, PrimeNumber_2_minus_1);
        mpz_powm(,
    } */

   Public_Exponent = "17";
   Private_Exponent = "157";
   Public_Modulus = "2773";

   return;
}

// Function to calculate Euler's Totient function for n = p * q
/* void computePhi(mpz_t result, const mpz_t PrimeNumber_1, const mpz_t PrimeNumber_2) 
{
    mpz_t PrimeNumber_1_minus_1, PrimeNumber_2_minus_1;
    mpz_inits(p_minus_1, q_minus_1, nullptr);

    // Compute PrimeNumber_1 - 1 and PrimeNumber_2 - 1
    mpz_sub_ui(p_minus_1, PrimeNumber_1, 1);
    mpz_sub_ui(q_minus_1, PrimeNumber_2, 1);

    // Compute phi(n) = (PrimeNumber_1 - 1) * (PrimeNumber_2 - 1)
    mpz_mul(result, PrimeNumber_1_minus_1, PrimeNumber_2_minus_1);

    // Clear temporary variables
    mpz_clears(PrimeNumber_1_minus_1, PrimeNumber_2_minus_1, nullptr);
} */

/* // Function to compute GCD using the Euclidean algorithm
void computeGCD(mpz_t result, const mpz_t num1, const mpz_t num2) {
    mpz_t a, b, temp;
    mpz_inits(a, b, temp, nullptr);

    // Initialize a and b with num1 and num2
    mpz_set(a, num1);
    mpz_set(b, num2);

    // Apply Euclidean algorithm
    while (mpz_cmp_ui(b, 0) != 0) {
        mpz_mod(temp, a, b); // temp = a % b
        mpz_set(a, b);       // a = b
        mpz_set(b, temp);    // b = temp
    }

    // Set result to a (the GCD)
    mpz_set(result, a);

    // Clear mpz_t variables
    mpz_clears(a, b, temp, nullptr);
} */

/* //Function to generate two different prime numbers
void generateTwoPrimeNumbers(mpz_t PrimeNumber_1,mpz_t PrimeNumber_2) 
{
    // Generate a random 256-byte number 
    mpz_t randomNumber; 
    mpz_init(randomNumber);    

    generateRandomNumber(randomNumber);
    if(isPrime(const mpz_t randomNumber)=="Yes") PrimeNumber_1=randomNumber;
    

    generateRandomNumber(randomNumber);
    if(isPrime(const mpz_t randomNumber)=="Yes") PrimeNumber_2=randomNumber;

    // Clear GMP variables 
    mpz_clear(randomNumber);
} */

/* // Converts an ordinary message to a padded message (Stub function for demonstration purposes)
// Gamel's responsibility
void ConvertOrdinaryMessageToPaddedMessage(mpz_t PaddedMessage, const mpz_t OrdinaryMessage) {
    mpz_set(PaddedMessage, OrdinaryMessage); // Placeholder logic, this should implement proper padding
}

// Converts a padded message back to an ordinary message (Stub function for demonstration purposes)
// Gamel's responsibility
void ConvertPaddedMessageToOrdinaryMessage(mpz_t OrdinaryMessage, const mpz_t PaddedMessage) {
    mpz_set(OrdinaryMessage, PaddedMessage); // Placeholder logic, this should remove padding
}

// Function to generate a random 256-byte number
void generateRandomNumber(mpz_t randomNumber) 
{
    // Initialize GMP random state
    gmp_randstate_t state;
    gmp_randinit_default(state);

    // Seed the random state with current time
    std::random_device rd;
    unsigned long seed = rd();
    gmp_randseed_ui(state, seed);
    mpz_urandomb(randomNumber, state, 256 * 8); // 256 bytes = 256 * 8 bits


    gmp_randclear(state);
}

// Function to check if a number is prime
bool isPrime(const mpz_t randomNumber) 
{
    int result = mpz_probab_prime_p(randomNumber, 25); // 25 rounds of Miller-Rabin tests
    return result > 0; // Result > 0 indicates a probable prime
} */

/*************************************************************
 * Function Name: RSA_PKCS_1_encrypt
 * Descriotion:
 *  This function acts as a wrapper for the modular exponentiation
 *  to be used for encryption
 * Arguments:
 *  message (mpz_class): The message to be encrypted
 *  publicExponent (mpz_class): The public exponent
 *  modulus (mpz_class): The modulus
 * Return:
 *  cipherText (mpz_class): The encrypted message
 ************************************************************/
mpz_class RSA_PKCS_1_encrypt(const mpz_class message, const mpz_class publicExponent, const mpz_class modulus) 
{
    mpz_class cipherText = modularExponentiation(message, publicExponent, modulus);
    
    return cipherText;
}

/*************************************************************
 * Function Name: RSA_PKCS_1_decrypt
 * Descriotion:
 *  This function acts as a wrapper for the modular exponentiation
 *  to be used for decryption
 * Arguments:
 *  cipher (mpz_class): The message to be decrypted
 *  privateExponent (mpz_class): The private exponent
 *  modulus (mpz_class): The modulus
 * Return:
 *  decryptedText (mpz_class): The decrypted message
 ************************************************************/
mpz_class RSA_PKCS_1_decrypt(const mpz_class cipher, const mpz_class privateExponent, const mpz_class modulus) 
{
    mpz_class decryptedText = modularExponentiation(cipher, privateExponent, modulus);
    
    return decryptedText;
}

/*************************************************************
 * Function Name: modularExponentiation
 * Descriotion:
 *  This function performs modular exponentiation using the
 *  Indian algorithm. Given (base ^ exponent) mode (modulus),
 *  the algorithm works as follows:
 *      1- Initialization Step: Start with Result = 1
 *      2- Start from the LSB of the exponent
 *      3- While the exponent is not zero
 *          3.1- If the current bit is 1, update result by
 *               multiplying the result by base
 *          3.2- Reduce the result to the modulus value
 *          3.3- Update the base by multiplying it by itself
 *          3.4- Shift the exponent left (Halving it)
 *      4- The final result is the data of Result
 * Arguments:
 *  base (mpz_class): TThe base of the exponentiation
 *  exponent (mpz_class): The exponent value
 *  modulus (mpz_class): The modulus for reduction
 * Return:
 *  result (mpz_class): The final result of the modular
 *                       exponentiation
 ************************************************************/
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