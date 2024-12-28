/*********************************************************************
 * @file SHA3_256.cpp
 * @author Abdelrahman Gamiel, Eslam Khaled, Mohamed Alaa Eldin Eldehimy, Mohamed Hesham Kamaly
 * @date 2024-12-30
 * @brief 
 *        This file provides a C++ implementation of the Secure Hash Algorithm 3 (SHA3), 
 *        specifically SHA3-256.  
 ********************************************************************/
/********************************************************************
 ************************** Included ********************************
 ********************************************************************/
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

/********************************************************************
 *********************** Configurations *****************************
 ********************************************************************/

/********************************************************************
 *********************** SYMBOLIC NAMES *****************************
 ********************************************************************/
#define STATE_ROW_SIZE              5U
#define STATE_COLUMN_SIZE           5U

/********************************************************************
 **************************** GLOBALS *******************************
 ********************************************************************/

/********************************************************************
 ************************* Prototypes *******************************
 ********************************************************************/
void SHA_ComputePi(uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE]);
void SHA_ComputeChi(uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE]);
void SHA_MultiRatePadding(const std::string& inputMessage, std::vector<uint8_t>& paddedMessage);

/********************************************************************
 ************************* Main Function ****************************
 ********************************************************************/
int main() 
{
    uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE] = {0};

    // Example: Perform pi and chi steps on the state
    SHA_ComputePi(state);
    SHA_ComputeChi(state);

    return 0;
}


/********************************************************************
 **************************** Functions *****************************
 ********************************************************************/
/*************************************************************
 * Function Name: SHA_ComputePi
 * Descriotion:
 *  This function performs the Pi step mapping which performs
 *  diffusion on slice level through the following formula:
 *    A'[x,y,z] = A[(x + 3y) mod 5, x, z]
 *  This Step provides more diffusion and helps in breaking
 *  positional patterns
 * Arguments:
 *  state (uint8_t *): Input-Output argument containing the state
 * Return:
 *  void
 ************************************************************/
void SHA_ComputePi(uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE]) 
{
    /* Define a temprary state to hold the original states before computation */
    uint64_t tempState[STATE_ROW_SIZE][STATE_COLUMN_SIZE];

    /* Copy the current state into the temporary State */
    for (int x = 0; x < STATE_ROW_SIZE; ++x) 
    {
        for (int y = 0; y < STATE_COLUMN_SIZE; ++y) 
        {
            tempState[x][y] = state[x][y];
        }
    }

    /* Apply the Pi step mapping */
    for (int x = 0; x < STATE_ROW_SIZE; ++x) 
    {
        for (int y = 0; y < STATE_COLUMN_SIZE; ++y) 
        {
            state[x][y] = tempState[(x + 3 * y) % 5][x];
        }
    }
}

/*************************************************************
 * Function Name: SHA_ComputeChi
 * Descriotion:
 *  This function performs the Chi step mapping which performs
 *  diffusion on row level. Basically, each bit in a row is
 *  XORed with the result of NOT of the following bit ANDed
 *  with the one after it. This is achieved through the
 *  following formula:
 *    A'[x,y,z] = A[x,y,z] ^ (~A[(x + 1) mod 5,y,z] & A[(x + 2) mod 5, y, z])
 *  This Step introduces non-linearity in the permutation
 *  through the use of AND operation making the output of this
 *  step irreversable.
 * Arguments:
 *  state (uint8_t *): Input-Output argument containing the state
 * Return:
 *  void
 ************************************************************/
void SHA_ComputeChi(uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE]) 
{
    /* Define a temporary state to hold the original states before computation */
    uint64_t tempState[STATE_ROW_SIZE][STATE_COLUMN_SIZE];

    /* Copy the current state into the temporary state */
    for (int x = 0; x < STATE_ROW_SIZE; ++x) 
    {
        for (int y = 0; y < STATE_COLUMN_SIZE; ++y) 
        {
            tempState[x][y] = state[x][y];
        }
    }

    /* Apply the Chi step mapping */
    for (int x = 0; x < STATE_ROW_SIZE; ++x) 
    {
        for (int y = 0; y < STATE_COLUMN_SIZE; ++y) 
        {
            state[x][y] = tempState[x][y] ^ 
                          ((~tempState[(x + 1) % STATE_ROW_SIZE][y]) & 
                           tempState[(x + 2) % STATE_ROW_SIZE][y]);
        }
    }
}

/*************************************************************
 * Function Name: SHA_MultiRatePadding
 * Descriotion:
 *  This function performs padding rule pad10*1 which adds
 *  padding to the input message to make it multiples of
 *  the rate so that it can be partioned during the SPONGE
 *  operation.
 * Arguments:
 *  inputMessage (string): Input string to be hashed
 *  paddedMessage (vector): the input string after being padded
 * Return:
 *  void
 ************************************************************/
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
