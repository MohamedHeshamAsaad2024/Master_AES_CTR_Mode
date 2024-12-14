#include <iostream>
#include <vector>
#include <random>
#include <tuple>
#include <cassert>

using namespace std;

// Converts a string to a long long
long long stringToLongLong(const string& str) {
    return stoll(str);
}

// Converts a long long to a string
string longLongToString(long long num) {
    return to_string(num);
}


// Function to generate a random 64-bit number
string generateRandomNumber() {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dis(0, numeric_limits<long long>::max());
    return longLongToString(dis(gen));
}


//Kamaly's responsibility
// Function to check if a number (given as a string) is prime
bool isPrime(const string& str) {
    long long num = stringToLongLong(str);
    if (num <= 1) return false;
    for (long long i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}


// Function to generate two different prime numbers
pair<string, string> generateTwoPrimeNumbers() {
    string PrimeNumber_1, PrimeNumber_2;

    do {
        PrimeNumber_1 = generateRandomNumber();
    } while (!isPrime(PrimeNumber_1));

    do {
        PrimeNumber_2 = generateRandomNumber();
    } while (!isPrime(PrimeNumber_2) || PrimeNumber_2 == PrimeNumber_1);

    return {PrimeNumber_1, PrimeNumber_2};
}

// Function to compute GCD using the Euclidean algorithm
string computeGCD(const string& str1, const string& str2) {
    long long a = stringToLongLong(str1);
    long long b = stringToLongLong(str2);
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return longLongToString(a);
}


//Gamil's responsibility
// Extended Euclidean Algorithm to find the modular inverse


//Gamil's responsibility
// Function to calculate the decryption exponent d
string computeDecryptionExponent(const string& str_e, const string& str_phi) {

}

// Function to calculate Euler's Totient function for n = p * q
string computePhi(const string& str1, const string& str2) {
    long long p = stringToLongLong(str1);
    long long q = stringToLongLong(str2);
    return longLongToString((p - 1) * (q - 1));
}

//Gamil's responsibility
// API function to compute RSA private exponent d given p and q
string computeRSAPrivateExponent(const string& str_p, const string& str_q, const string& str_e) {
    string phi = computePhi(str_p, str_q);
    return computeDecryptionExponent(str_e, phi);
}



//Kamaly's responsibility
tuple<string, string, string> generateKeys() 
{
    // Generate RSA keys (public modulus, public exponent, and private exponent)
    auto [p, q] = generateTwoPrimeNumbers();
    string Public_Modulus = longLongToString(stringToLongLong(p) * stringToLongLong(q));
    string Public_Exponent = "65537";
    string Private_Exponent ;// computeRSAPrivateExponent(p, q, e);


    return make_tuple(Public_Modulus, Public_Exponent, Private_Exponent);
}


//Eslam's responsibility
// Encrypts a message using RSA with PKCS #1 padding (Stub function for demonstration purposes)
string RSA_PKCS_1_encrypt(const string& str_message, const string& str_e, const string& str_n) {
    long long message = stringToLongLong(str_message);
    long long e = stringToLongLong(str_e);
    long long n = stringToLongLong(str_n);
    long long result = 1;
    for (long long i = 0; i < e; ++i) {
        result = (result * message) % n;
    }
    return longLongToString(result);
}

//Eldehimy's responsibility
// Decrypts a message using RSA with PKCS #1 padding (Stub function for demonstration purposes)
string RSA_PKCS_1_decrypt(const string& str_ciphertext, const string& str_d, const string& str_n) 
{
    long long ciphertext = stringToLongLong(str_ciphertext);
    long long d = stringToLongLong(str_d);
    long long n = stringToLongLong(str_n);
    long long result = 1;
    for (long long i = 0; i < d; ++i) {
        result = (result * ciphertext) % n;
    }
    return longLongToString(result);
}

int main() {
    // Display a message indicating that RSA key generation is in progress
    cout << "Generating RSA keys (256-bit)..." << endl;


 
    // Generate RSA keys (public modulus, public exponent, and private exponent)
    auto [Public_Modulus, Public_Exponent, Private_Exponent] = generateKeys();
    // Display the generated RSA keys
    cout << "Public Key (n, e): (" << Public_Modulus << ", " << Public_Exponent << ")" << endl;
    cout << "Private Key (d): (" << Private_Exponent << ")" << endl;

    // Sample message to encrypt
    string message = "123456789"; // Example message

    // Display the original message
    cout << "Original message: " << message << endl;

    // Encrypt the  message using the public key
    string ciphertext = RSA_PKCS_1_encrypt(message, Public_Exponent, Public_Modulus);
    cout << "Encrypted message: " << ciphertext << endl;

    // Decrypt the encrypted message using the private key
    // string decryptedMessage = RSA_PKCS_1_decrypt(ciphertext, Private_Exponent, Public_Modulus);
    // cout << "Decrypted  message: " << decryptedMessage << endl;


    return 0;
}