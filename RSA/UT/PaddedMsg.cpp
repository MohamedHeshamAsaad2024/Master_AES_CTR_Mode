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
    // Define key size in bytes (e.g., 256 bytes for 2048-bit RSA)
    size_t keySize = 256;

    // Convert the plaintext to a byte array
    vector<uint8_t> plaintext = mpzToByteArray(OrdinaryMessage, OrdinaryMessage.get_str(16).size() / 2);

    // Ensure the plaintext fits within the padded structure
    if (plaintext.size() > keySize - 11) {
        throw invalid_argument("Plaintext is too long for the given key size.");
    }

    // Initialize the padded message
    vector<uint8_t> paddedMessage(keySize, 0x00);

    // Add padding format bytes
    paddedMessage[0] = 0x00;
    paddedMessage[1] = 0x02;

    // Add random non-zero padding
    size_t paddingLength = keySize - plaintext.size() - 3;
    srand(static_cast<unsigned>(time(0)));
    for (size_t i = 0; i < paddingLength; ++i) {
        uint8_t randomByte;
        do {
            randomByte = rand() % 256;
        } while (randomByte == 0x00);
        paddedMessage[2 + i] = randomByte;
    }

    // Add the separator and plaintext
    paddedMessage[2 + paddingLength] = 0x00;
    memcpy(&paddedMessage[3 + paddingLength], plaintext.data(), plaintext.size());

    // Convert padded message to mpz_class
    PaddedMessage = byteArrayToMpz(paddedMessage);
}


/**
 * Function to remove PKCS#1 v1.5 padding from a padded message.
 * Converts a padded message back into the original plaintext message.
 * 
 * @param OrdinaryMessage: Reference to the resulting plaintext message as an mpz_class.
 * @param PaddedMessage: The padded message as an mpz_class.
 */
void ConvertPaddedMessageToOrdinaryMessage(mpz_class &OrdinaryMessage, const mpz_class PaddedMessage) {
    // Define key size in bytes
    size_t keySize = 256;

    // Convert padded message to a byte array
    vector<uint8_t> paddedBytes = mpzToByteArray(PaddedMessage, keySize);

    // Verify padding format
    if (paddedBytes[0] != 0x00 || paddedBytes[1] != 0x02) {
        throw invalid_argument("Invalid padding format.");
    }

    // Find the separator
    size_t separatorIndex = 2;
    while (separatorIndex < paddedBytes.size() && paddedBytes[separatorIndex] != 0x00) {
        separatorIndex++;
    }

    if (separatorIndex >= paddedBytes.size()) {
        throw invalid_argument("Padding separator not found.");
    }

    // Extract the plaintext
    vector<uint8_t> plaintext(paddedBytes.begin() + separatorIndex + 1, paddedBytes.end());
    OrdinaryMessage = byteArrayToMpz(plaintext);
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
