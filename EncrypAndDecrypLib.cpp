#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <bitset>

using namespace std;

// S-Box for SubBytes step
const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf0, 0x6f, 0x6b, 0x6e, 0x5c, 0x5d, 0x47, 0x4f, 0x5e, 0x51, 0x45, 0x49, 0x6a,
    0x68, 0x1b, 0x6d, 0x73, 0x79, 0x7d, 0x38, 0x41, 0x8f, 0x92, 0x9d, 0x38, 0x81, 0x72, 0x70, 0x8d, 0x99,
    0x65, 0x1c, 0x7b, 0x51, 0x74, 0x3c, 0x5f, 0x9b, 0x5c, 0x5f, 0x68, 0x55, 0x35, 0x34, 0x88, 0x72, 0x94,
    0x62, 0x6f, 0x9e, 0xa2, 0x2d, 0x43, 0x51, 0x38, 0x33, 0x36, 0x7a, 0x5a, 0x67, 0x98, 0x4a, 0x33, 0x41,
    0x5f, 0x9c, 0x45, 0x5d, 0x7c, 0x4c, 0x60, 0x51, 0x98, 0x62, 0x55, 0x8b, 0x47, 0x99, 0x74, 0x41, 0x52
};
// Inverse S-Box for InverseSubBytes step
const uint8_t inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0x7e, 0x3d, 0x64, 0x5d,
    0x19, 0x73, 0x60, 0x81, 0x5a, 0x5d, 0x76, 0x87, 0x42, 0x6c, 0x99, 0x0b, 0x72, 0x4e, 0x60, 0x68, 0x51,
    0x2b, 0x76, 0x91, 0x7e, 0x92, 0x2b, 0x72, 0x98, 0x6b, 0x7c, 0xa0, 0x55, 0x20, 0xe5, 0x75, 0x29, 0x38,
    0x55, 0xd7, 0x66, 0x31, 0x53, 0x7e, 0x69, 0x66, 0x1b, 0x38, 0x57, 0x60, 0x6b, 0x32, 0x57, 0x7b, 0x88
};

// Round constants (for AES key expansion)
const uint8_t rcon[10] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };

// AddRoundKey: XORs the state with the round key used for both encryption and decryption
void AddRoundKey(uint8_t state[4][4], uint8_t roundKey[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] ^= roundKey[i][j];
        }
    }
}

// SubBytes: Applies the S-Box to the state
void SubBytes(uint8_t state[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

// Inverse SubBytes: Applies the inverse S-Box to the state
void InvSubBytes(uint8_t state[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}

// ShiftRows: Performs the row shift in the AES process
void ShiftRows(uint8_t state[4][4]) {
    uint8_t temp;
    // Row 1: Left shift by 1
    temp = state[1][0];
    for (int i = 0; i < 3; ++i) {
        state[1][i] = state[1][i + 1];
    }
    state[1][3] = temp;

    // Row 2: Left shift by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Row 3: Left shift by 3
    temp = state[3][0];
    for (int i = 0; i < 3; ++i) {
        state[3][i] = state[3][i + 1];
    }
    state[3][3] = temp;
}

// Inverse ShiftRows: Performs the inverse row shift for decryption
void InvShiftRows(uint8_t state[4][4]) {
    uint8_t temp;
    // Row 1: Right shift by 1
    temp = state[1][3];
    for (int i = 3; i > 0; --i) {
        state[1][i] = state[1][i - 1];
    }
    state[1][0] = temp;

    // Row 2: Right shift by 2
    temp = state[2][3];
    state[2][3] = state[2][1];
    state[2][1] = temp;
    temp = state[2][2];
    state[2][2] = state[2][0];
    state[2][0] = temp;

    // Row 3: Right shift by 3
    temp = state[3][3];
    for (int i = 3; i > 0; --i) {
        state[3][i] = state[3][i - 1];
    }
    state[3][0] = temp;
}

// MixColumns: Mixes the columns using a fixed matrix multiplication
void MixColumns(uint8_t state[4][4]) {
    uint8_t temp[4][4];
    for (int c = 0; c < 4; ++c) {
        temp[0][c] = (state[0][c] * 2) ^ (state[1][c] * 3) ^ state[2][c] ^ state[3][c];
        temp[1][c] = state[0][c] ^ (state[1][c] * 2) ^ (state[2][c] * 3) ^ state[3][c];
        temp[2][c] = state[0][c] ^ state[1][c] ^ (state[2][c] * 2) ^ (state[3][c] * 3);
        temp[3][c] = (state[0][c] * 3) ^ state[1][c] ^ state[2][c] ^ (state[3][c] * 2);
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = temp[i][j];
        }
    }
}

// Inverse MixColumns: Performs the inverse mix columns for decryption
void InvMixColumns(uint8_t state[4][4]) {
    uint8_t temp[4][4];
    for (int c = 0; c < 4; ++c) {
        temp[0][c] = (state[0][c] * 14) ^ (state[1][c] * 11) ^ (state[2][c] * 13) ^ (state[3][c] * 9);
        temp[1][c] = (state[0][c] * 9) ^ (state[1][c] * 14) ^ (state[2][c] * 11) ^ (state[3][c] * 13);
        temp[2][c] = (state[0][c] * 13) ^ (state[1][c] * 9) ^ (state[2][c] * 14) ^ (state[3][c] * 11);
        temp[3][c] = (state[0][c] * 11) ^ (state[1][c] * 13) ^ (state[2][c] * 9) ^ (state[3][c] * 14);
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = temp[i][j];
        }
    }
}
// KeyExpansion to generate round keys
void KeyExpansion(uint8_t key[16], uint8_t roundKeys[11][4][4]) {
    uint8_t temp[4]; // Temporary storage for the column being processed
    int i = 0;

    // Copy the original key as the first round key
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            roundKeys[0][c][r] = key[i++];
        }
    }

    // Generate the rest of the round keys
    for (int round = 1; round <= 10; ++round) {
        // Step 1: Get the last column from the previous round key
        for (int row = 0; row < 4; ++row) {
            temp[row] = roundKeys[round - 1][3][row];
        }

        // Step 2: Rotate the column (Word rotation)
        uint8_t t = temp[0];
        temp[0] = temp[1];
        temp[1] = temp[2];
        temp[2] = temp[3];
        temp[3] = t;

        // Step 3: Apply SubBytes to the column
        for (int row = 0; row < 4; ++row) {
            temp[row] = sbox[temp[row]];
        }

        // Step 4: XOR the first element with the round constant
        temp[0] ^= rcon[round - 1];

        // Step 5: Compute the first column of the current round key
        for (int row = 0; row < 4; ++row) {
            roundKeys[round][0][row] = roundKeys[round - 1][0][row] ^ temp[row];
        }

        // Step 6: Compute the remaining columns of the current round key
        for (int col = 1; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                roundKeys[round][col][row] = roundKeys[round - 1][col][row] ^ roundKeys[round][col - 1][row];
            }
        }
    }
}

// AESEncrypt function
void AESEncrypt(uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t roundKeys[11][4][4]) {
    uint8_t state[4][4];

    // Copy plaintext into state array (row-major to column-major format)
    int idx = 0;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            state[row][col] = plaintext[idx++];
        }
    }

    // Initial round key addition
    AddRoundKey(state, roundKeys[0]);

    // Main rounds
    for (int round = 1; round <= 9; ++round) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKeys[round]);
    }

    // Final round (no MixColumns)
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys[10]);

    // Copy state to ciphertext
    idx = 0;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            ciphertext[idx++] = state[row][col];
        }
    }
}
// AESDecrypt function
void AESDecrypt(uint8_t ciphertext[16], uint8_t plaintext[16], uint8_t roundKeys[11][4][4]) {
    uint8_t state[4][4];

    // Copy ciphertext into state array (row-major to column-major format)
    int idx = 0;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            state[row][col] = ciphertext[idx++];
        }
    }

    // Initial round key addition
    AddRoundKey(state, roundKeys[10]);

    // Main rounds (in reverse order)
    for (int round = 9; round >= 1; --round) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, roundKeys[round]);
        InvMixColumns(state);
    }

    // Final round (no InvMixColumns)
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, roundKeys[0]);

    // Copy state to plaintext
    idx = 0;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            plaintext[idx++] = state[row][col];
        }
    }
}


// Main function to test AES encryption and decryption
int main() {
    uint8_t key[16] = {
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0xcf, 0xa5,
        0x30, 0x8d, 0x31, 0x32
    };
    
    uint8_t plaintext[16] = {
        0x32, 0x88, 0x31, 0xe0,
        0x43, 0x5a, 0x31, 0x37,
        0xf6, 0x30, 0x98, 0x07,
        0xa8, 0x8d, 0xa2, 0x34
    };
    
    uint8_t ciphertext[16];
    uint8_t decryptedText[16];
    
    uint8_t roundKeys[11][4][4];
    
    // Perform key expansion
    KeyExpansion(key, roundKeys);
    
    // Encrypt the plaintext
    AESEncrypt(plaintext, ciphertext, roundKeys);
    
    // Decrypt the ciphertext
    AESDecrypt(ciphertext, decryptedText, roundKeys);
    
    // Output the result
    cout << "Plaintext: ";
    for (int i = 0; i < 16; ++i) {
        cout << hex << setw(2) << setfill('0') << (int)plaintext[i] << " ";
    }
    cout << endl;
    
    cout << "Ciphertext: ";
    for (int i = 0; i < 16; ++i) {
        cout << hex << setw(2) << setfill('0') << (int)ciphertext[i] << " ";
    }
    cout << endl;
    
    cout << "Decrypted text: ";
    for (int i = 0; i < 16; ++i) {
        cout << hex << setw(2) << setfill('0') << (int)decryptedText[i] << " ";
    }
    cout << endl;

    return 0;
}