#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <bitset>

#define STATE_ROW_SIZE              5U
#define STATE_COLUMN_SIZE           5U
#define STATE_LANE_SIZE             64U

void SHA_MultiRatePadding(const std::string& inputMessage, std::vector<uint8_t>& paddedMessage) 
{
    /* Define the rate for width of 1600 and capacity 512; r = b - c = 1088*/
    int rate = 1088;

    /* Compute length of input message in bits */
    int InputMessageLength = inputMessage.size() * 8;

    /* Compute the number of needed zeros (j) in the padding through j = (-InputMessageLength - 2) mod rate */
    int j = (-InputMessageLength - 2) % rate;

    /* Ensure that j is non-negative number */
    if (j < 0) 
    {
        j += rate;
    }

    /* Compute the total length of the padding which is 2 (The sentinal 1's) plus j (The number of intermediate zeros) */
    int paddingLength = 2 + j;

    /* Clear the output vector in preparation to hold the padded message */
    paddedMessage.clear();

    /* Add input message as bytes in the padded message */
    for (char c : inputMessage) 
    {
        paddedMessage.push_back(static_cast<uint8_t>(c));
    }

    /* Add "1" bit which is 0x80 = 0b10000000 (MSB = 1) */
    paddedMessage.push_back(static_cast<uint8_t>(0x80));

    /* Compute the number of needed zero bytes */
    int fullZeroBytes = (paddingLength - 1) / 8;

    /* Compute the number of remaining zero bits in the last byte if any */
    int remainingBits = (paddingLength - 1) % 8;

    /* Append the full zero bytes if any */
    for (int i = 0; i < fullZeroBytes; ++i) 
    {
        paddedMessage.push_back(0x00);
    }

    /* Append the remaining zero bits if any */
    if (remainingBits > 0) 
    {
        uint8_t lastPartialByte = (1 << (7 - remainingBits)) - 1;
        paddedMessage.push_back(lastPartialByte);
    }

    /* Add final "1" bit by ORing the last bit as 1 */
    paddedMessage.back() |= 0x01;
}

int main() 
{
    std::string input = "hello"; // Example input message
    std::vector<uint8_t> paddedMessage;

    // Pad the message
    SHA_MultiRatePadding(input, paddedMessage);

    // Print the padded message in binary
    std::cout << "Padded Message (in binary):" << std::endl;
    for (uint8_t byte : paddedMessage) {
        std::cout << std::bitset<8>(byte) << " ";
    }
    std::cout << std::endl;

    return 0;
}