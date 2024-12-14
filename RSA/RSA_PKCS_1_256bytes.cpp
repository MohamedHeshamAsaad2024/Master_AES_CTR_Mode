/**
 * PKCS VERSION 1.5 Padding scheme implementation
 * 
 * 
 * Implemented by 
 * Eslam abdelsamea
 * Abdelrahman Gamil
 * Mohamed eldehamy 
 * Mohamed Hesham Kamaly
 */

#include <iostream>
#include <vector>
#include <random>
#include <gmp.h>
#include <gmpxx.h>
#include <sstream>
#include <cassert>
#include <tuple>

using namespace std;


// Converts an ordinary message to a padded message (Stub function for demonstration purposes)
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
}

//Function to generate two different prime numbers
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
}


// Function to compute GCD using the Euclidean algorithm
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
}


// Function to calculate Euler's Totient function for n = p * q
void computePhi(mpz_t result, const mpz_t PrimeNumber_1, const mpz_t PrimeNumber_2) 
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
}



// Generates RSA keys: public and private (Stub function for demonstration purposes)
// Kamaly's responsibility
tuple<mpz_t, mpz_t, mpz_t> generateKeys() 
{
    mpz_t Public_Exponent, Private_Exponent, Public_Modulus,Generted_PrimeNumber_1,Generted_PrimeNumber_2,phi,temp;
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
   }






   return make_tuple(Public_Modulus, Public_Exponent, Private_Exponent);
}

// Encrypts a message using RSA with PKCS #1 padding (Stub function for demonstration purposes)
// Eslam's responsibility
void RSA_PKCS_1_encrypt(mpz_t EncryptedMessage, const mpz_t message, const mpz_t Public_Exponent, const mpz_t Public_Modulus) {
    mpz_powm(EncryptedMessage, message, Public_Exponent, Public_Modulus); // Simplified encryption logic
}

// Decrypts a message using RSA with PKCS #1 padding (Stub function for demonstration purposes)
// Eldehamy's responsibility
void RSA_PKCS_1_decrypt(mpz_t DecryptedMessage, const mpz_t ciphertext, const mpz_t Private_Exponent, const mpz_t Public_Modulus) {
    mpz_powm(DecryptedMessage, ciphertext, Private_Exponent, Public_Modulus); // Simplified decryption logic
}

int main() {
    // Display a message indicating that RSA key generation is in progress
    cout << "Generating RSA keys (2048-bit)..." << endl;

    // Generate RSA keys (public modulus, public exponent, and private exponent)
    auto [Public_Modulus, Public_Exponent, Private_Exponent] = generateKeys();

    // Display the generated RSA keys
    gmp_printf("Public Key (Public_Modulus, Public_Exponent): (%Zd, %Zd)\n", Public_Modulus, Public_Exponent);
    gmp_printf("Private Key (Private_Exponent): (%Zd)\n", Private_Exponent);

    // Sample message to encrypt
    mpz_t message;
    mpz_init_set_str(message, "This is the message that needed to be encrypted", 10); // Example message

    // Display the original message
    gmp_printf("Original message: %Zd\n", message);

    // Convert the ordinary message to a padded message
    mpz_t Padded_message;
    mpz_init(Padded_message);
    ConvertOrdinaryMessageToPaddedMessage(Padded_message, message);
    gmp_printf("Padded message: %Zd\n", Padded_message);

    // Encrypt the padded message using the public key
    mpz_t ciphertext;
    mpz_init(ciphertext);
    RSA_PKCS_1_encrypt(ciphertext, Padded_message, Public_Exponent, Public_Modulus);
    gmp_printf("Encrypted message: %Zd\n", ciphertext);

    // Decrypt the encrypted message using the private key
    mpz_t decryptedPaddedMessage;
    mpz_init(decryptedPaddedMessage);
    RSA_PKCS_1_decrypt(decryptedPaddedMessage, ciphertext, Private_Exponent, Public_Modulus);
    gmp_printf("Decrypted padded message: %Zd\n", decryptedPaddedMessage);

    // Convert the decrypted padded message back to the original message
    mpz_t decryptedMessage;
    mpz_init(decryptedMessage);
    ConvertPaddedMessageToOrdinaryMessage(decryptedMessage, decryptedPaddedMessage);
    gmp_printf("Decrypted message: %Zd\n", decryptedMessage);

    // Clear mpz_t variables
    mpz_clears(message, Padded_message, ciphertext, decryptedPaddedMessage, decryptedMessage, Public_Modulus, Public_Exponent, Private_Exponent, nullptr);

    return 0;
}
