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
#define ROTL64(x, n) (((x) << (n)) | ((x) >> (64 - (n))))
/********************************************************************
 **************************** GLOBALS *******************************
 ********************************************************************/

/********************************************************************
 ************************* Prototypes *******************************
 ********************************************************************/
void SHA_ComputePi(uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE]);
void SHA_ComputeChi(uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE]);
void SHA_ComputeTheta(uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE]);
std::vector<uint8_t> SHA_MultiRatePadding(const std::string& input, int rate);

/********************************************************************
 ************************* Main Function ****************************
 ********************************************************************/
int main() 
{
    /* Exampl usage of padding scheme */
    std::string message = "Hello, SHA-3!";
    size_t rate = 1088;
    std::vector<uint8_t> paddedMessage = SHA_MultiRatePadding(message, rate);

    // Example: Perform pi and chi steps on the state
    uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE] = {0};
    SHA_ComputeTheta(state);
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
 * Function Name: SHA_ComputeTheta
 * Description:
 *  This function performs the Theta step mapping which ensures
 *  diffusion across all bits in the state. Each bit is XORed
 *  with a parity bit derived from the columns of the state.
 *  This is achieved through the following formula:
 *    C[x] = A[x,0] ^ A[x,1] ^ A[x,2] ^ A[x,3] ^ A[x,4]
 *    D[x] = C[x-1] ^ ROT(C[x+1], 1)
 *    A'[x,y] = A[x,y] ^ D[x]
 * Arguments:
 *  state (uint64_t *): Input-Output argument containing the state
 * Return:
 *  void
 ************************************************************/
void SHA_ComputeTheta(uint64_t state[STATE_ROW_SIZE][STATE_COLUMN_SIZE]) 
{
    uint64_t C[STATE_ROW_SIZE];
    uint64_t D[STATE_ROW_SIZE];

    /* Compute the parity of each column */
    for (int x = 0; x < STATE_ROW_SIZE; ++x) 
    {
        C[x] = state[x][0] ^ state[x][1] ^ state[x][2] ^ state[x][3] ^ state[x][4];
    }

    /* Compute the D array */
    for (int x = 0; x < STATE_ROW_SIZE; ++x) 
    {
        D[x] = C[(x + STATE_ROW_SIZE - 1) % STATE_ROW_SIZE] ^ ROTL64(C[(x + 1) % STATE_ROW_SIZE], 1);
    }

    /* Apply the Theta step */
    for (int x = 0; x < STATE_ROW_SIZE; ++x) 
    {
        for (int y = 0; y < STATE_COLUMN_SIZE; ++y) 
        {
            state[x][y] ^= D[x];
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
 *  This function performs does two things:
 *  - Append the hash mode identifiers (01)
 *  - Padding rule pad10*1 which adds padding to the input
 *    message to make it multiples of the rate so that it 
 *    can be partioned during the SPONGE operation.
 * Note:
 *   - The hash identifier (01) and the starting padding bit (1) are byte-aligned as 0x06
 *   - The terminating padding bit (1) is byte-aligned as 0x80
 * Arguments:
 *  inputMessage (string): Input string to be hashed
 *  paddedMessage (vector): the input string after being padded
 * Return:
 *  vector: The padded message with the hash mode prefix
 ************************************************************/
std::vector<uint8_t> SHA_MultiRatePadding(const std::string& input, int rate) 
{
    /* Convert the input string to a vector of bytes */
    std::vector<uint8_t> paddedMessage(input.begin(), input.end());

    /* Determine the rate in bytes */
    size_t rateInBytes = rate / 8;

    /* Compute the length of the input in bytes */
    size_t m = paddedMessage.size();

    /* Calculate the number of padding bytes (q) */
    size_t q = rateInBytes - (m % rateInBytes);

    /* Append the padding bytes depending on the needed number of padding bytes (q) */
    if (q == 1) 
    {
        /* If the input message is multiple of the rate, then we only append one byte */
        paddedMessage.push_back(0x86);
    } 
    else if (q == 2) 
    {
        /* If two bytes are needed, we set the first byte 0x06 as the prefix byte */
        paddedMessage.push_back(0x06);
        /* We add the last byte which terminates the padding */
        paddedMessage.push_back(0x80);
    } else 
    {
        /* If more than 2 bytes are needed, we set the first byte 0x06 as the prefix byte */
        paddedMessage.push_back(0x06);

        /* Add intermediate zero bytes */
        for (size_t i = 0; i < q - 2; ++i) 
        {
            paddedMessage.push_back(0x00);
        }

        /* We add the last byte which terminates the padding */
        paddedMessage.push_back(0x80);
    }

    return paddedMessage;
}