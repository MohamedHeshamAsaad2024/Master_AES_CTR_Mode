#include <iostream>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For seeding rand()
#include <cstring> // For memcpy()
using namespace std;

/**
 * Function to pad plaintext in PKCS#1 v1.5 format.
 * PKCS#1 padding ensures that the plaintext is padded to the size of the RSA modulus.
 * 
 * Format:
 *   [0x00 | 0x02 | Random Non-Zero Padding | 0x00 | Plaintext]
 * 
 * @param plaintext: The input plaintext as a vector of bytes.
 * @param keySize: The size of the RSA key in bytes (e.g., 128 bytes for 1024-bit key).
 * @return A vector of bytes containing the padded message.
 */
vector<uint8_t> pkcs1Pad(const vector<uint8_t>& plaintext, size_t keySize) {
    // Ensure the plaintext fits within the padded structure
    if (plaintext.size() > keySize - 11) {
        throw invalid_argument("Plaintext is too long for the given key size.");
    }

    // Initialize the padded message with the required size
    vector<uint8_t> paddedMessage(keySize, 0x00);

    // Step 1: Add the padding format bytes
    paddedMessage[0] = 0x00; // First byte must be 0x00
    paddedMessage[1] = 0x02; // Second byte must be 0x02 for encryption padding

    // Step 2: Fill with random non-zero bytes
    size_t paddingLength = keySize - plaintext.size() - 3; // Length of the padding
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    for (size_t i = 0; i < paddingLength; ++i) {
        uint8_t randomByte;
        do {
            randomByte = rand() % 256; // Generate a random byte
        } while (randomByte == 0x00); // Ensure it's non-zero
        paddedMessage[2 + i] = randomByte;
    }

    // Step 3: Add the 0x00 separator byte
    paddedMessage[2 + paddingLength] = 0x00;

    // Step 4: Copy the plaintext at the end
    memcpy(&paddedMessage[3 + paddingLength], plaintext.data(), plaintext.size());

    return paddedMessage;
}

int main() {
    // Example usage
    vector<uint8_t> plaintext = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII
    size_t keySize = 128; // Example for a 1024-bit RSA key (128 bytes)

    try {
        vector<uint8_t> paddedMessage = pkcs1Pad(plaintext, keySize);

        // Print the padded message in hexadecimal format
        cout << "Padded Message: ";
        for (uint8_t byte : paddedMessage) {
            printf("%02X ", byte);
        }
        cout << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
