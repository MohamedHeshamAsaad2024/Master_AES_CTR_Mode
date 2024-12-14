#include <iostream>
#include <gmpxx.h> // GMP library for handling large integers
#include <vector>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For seeding rand()
using namespace std;

/**
 * Function to pad an ordinary message in PKCS#1 v1.5 format.
 * Converts the plaintext message into a padded message suitable for RSA encryption.
 * 
 * @param PaddedMessage: Reference to the resulting padded message as an mpz_class.
 * @param OrdinaryMessage: The original plaintext message as an mpz_class.
 */
void ConvertOrdinaryMessageToPaddedMessage(mpz_class &PaddedMessage, const mpz_class OrdinaryMessage) {
    // Convert OrdinaryMessage to a byte array
    string messageStr = OrdinaryMessage.get_str(16); // Get the message as a hexadecimal string
    vector<uint8_t> messageBytes(messageStr.size() / 2);
    for (size_t i = 0; i < messageBytes.size(); ++i) {
        messageBytes[i] = stoi(messageStr.substr(2 * i, 2), nullptr, 16);
    }

    // Determine key size (e.g., 128 bytes for 1024-bit RSA)
    size_t keySize = 128;

    // Ensure the plaintext fits within the padded structure
    if (messageBytes.size() > keySize - 11) {
        throw invalid_argument("Plaintext is too long for the given key size.");
    }

    // Initialize the padded message as a byte array
    vector<uint8_t> paddedMessage(keySize, 0x00);

    // Step 1: Add the padding format bytes
    paddedMessage[0] = 0x00; // First byte must be 0x00
    paddedMessage[1] = 0x02; // Second byte must be 0x02 for encryption padding

    // Step 2: Fill with random non-zero bytes
    size_t paddingLength = keySize - messageBytes.size() - 3; // Length of the padding
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
    for (size_t i = 0; i < messageBytes.size(); ++i) {
        paddedMessage[3 + paddingLength + i] = messageBytes[i];
    }

    // Convert the padded message back to an mpz_class
    string paddedHex;
    for (uint8_t byte : paddedMessage) {
        char buf[3];
        sprintf(buf, "%02X", byte);
        paddedHex += buf;
    }
    PaddedMessage.set_str(paddedHex, 16); // Convert from hex string to mpz_class
}

/**
 * Function to remove PKCS#1 v1.5 padding from a padded message.
 * Converts a padded message back into the original plaintext message.
 * 
 * @param OrdinaryMessage: Reference to the resulting plaintext message as an mpz_class.
 * @param PaddedMessage: The padded message as an mpz_class.
 */
void ConvertPaddedMessageToOrdinaryMessage(mpz_class &OrdinaryMessage, const mpz_class PaddedMessage) {
    // Convert PaddedMessage to a byte array
    string paddedStr = PaddedMessage.get_str(16); // Get the padded message as a hexadecimal string
    vector<uint8_t> paddedBytes(paddedStr.size() / 2);
    for (size_t i = 0; i < paddedBytes.size(); ++i) {
        paddedBytes[i] = stoi(paddedStr.substr(2 * i, 2), nullptr, 16);
    }

    // Verify the padding format
    if (paddedBytes[0] != 0x00 || paddedBytes[1] != 0x02) {
        throw invalid_argument("Invalid padding format.");
    }

    // Find the 0x00 separator byte
    size_t separatorIndex = 2;
    while (separatorIndex < paddedBytes.size() && paddedBytes[separatorIndex] != 0x00) {
        separatorIndex++;
    }

    if (separatorIndex >= paddedBytes.size()) {
        throw invalid_argument("Padding separator not found.");
    }

    // Extract the plaintext message
    vector<uint8_t> messageBytes(paddedBytes.begin() + separatorIndex + 1, paddedBytes.end());

    // Convert the plaintext message back to an mpz_class
    string messageHex;
    for (uint8_t byte : messageBytes) {
        char buf[3];
        sprintf(buf, "%02X", byte);
        messageHex += buf;
    }
    OrdinaryMessage.set_str(messageHex, 16); // Convert from hex string to mpz_class
}

int main() {
    // Example usage
    mpz_class OrdinaryMessage("48656C6C6F", 16); // "Hello" in hexadecimal
    mpz_class PaddedMessage;

    // Convert to padded message
    ConvertOrdinaryMessageToPaddedMessage(PaddedMessage, OrdinaryMessage);
    cout << "Padded Message: " << PaddedMessage.get_str(16) << endl;

    // Convert back to ordinary message
    mpz_class RecoveredMessage;
    ConvertPaddedMessageToOrdinaryMessage(RecoveredMessage, PaddedMessage);
    cout << "Recovered Message: " << RecoveredMessage.get_str(16) << endl;

    return 0;
}
