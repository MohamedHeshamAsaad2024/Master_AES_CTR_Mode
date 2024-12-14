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

// Generates RSA keys: public and private (Stub function for demonstration purposes)
// Kamaly's responsibility
tuple<mpz_t, mpz_t, mpz_t> generateKeys() {
    mpz_t Public_Exponent, Private_Exponent, Public_Modulus;
    mpz_inits(Public_Exponent, Private_Exponent, Public_Modulus, nullptr);

    // Example values, these should be generated properly
    mpz_set_ui(Public_Modulus, 2773);
    mpz_set_ui(Public_Exponent, 17);
    mpz_set_ui(Private_Exponent, 157);

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
