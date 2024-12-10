#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <cassert>
#include <tuple>

using namespace std;



// Generate RSA keys
tuple<string, string, string> generateKeys() {

    string Public_Exponent,Private_Exponent,Public_Modulus;
    return make_tuple(Public_Modulus, Public_Exponent, Private_Exponent);
}

// RSA Encryption function
string RSA_PKCS_1_encrypt(const string& message, const string& Public_Exponent, const string& Public_Modulus) {
  return"";
}

// RSA Decryption function
string RSA_PKCS_1_decrypt(const string& ciphertext, const string& Private_Exponent, const string& Public_Modulus) {
  return"";
}

int main() {


    cout << "Generating RSA keys (2048-bit)..." << endl;
    auto [Public_Modulus, Public_Exponent, Private_Exponent] = generateKeys();

    cout << "Public Key (Public_Modulus, Public_Exponent): (" << Public_Modulus << ", " << Public_Exponent << ")" << endl;
    cout << "Private Key (Public_Modulus,Private_Exponent):  (" << Public_Modulus << ", " << Private_Exponent << ")" << endl;

    // Sample message to encrypt
    string message = "This is the text to be encrypted."; // Example message
    cout << "Original message: " << message << endl;

    // Encrypt the message
    string ciphertext = RSA_PKCS_1_encrypt(message, Public_Exponent, Public_Modulus);
    cout << "Encrypted message: " << ciphertext << endl;

    // Decrypt the message
    string decryptedMessage = RSA_PKCS_1_decrypt(ciphertext, Private_Exponent, Public_Modulus);
    cout << "Decrypted message: " << decryptedMessage << endl;

    return 0;
}
