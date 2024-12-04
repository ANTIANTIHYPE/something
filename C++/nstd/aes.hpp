#pragma once

#include <cstdint>
#include <stdexcept>

namespace nstd
{
/**
 * @todo rework
 * 
 * @brief AES (Advanced Encryption Standard) encryption and decryption.
 */
namespace aes
{

const std::uint8_t SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
}; // S-Box

const std::uint8_t INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
    0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
    0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
    0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
    0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
    0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
    0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
    0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
    0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
    0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
    0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
    0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
    0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
    0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
    0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
    0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
}; // Inverted S-Box

const std::uint8_t RCON[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
}; // Round constants

/**
 * @brief Adds the round key to the state matrix.
 * 
 * @param state The state matrix.
 * @param roundKey The round key to be added.
 */
void addRoundKey(std::uint8_t state[4][4], std::uint8_t roundKey[4][4]) noexcept
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            state[i][j] ^= roundKey[i][j];
        }
    }
}

/**
 * @brief Substitutes bytes in the state matrix using the S-Box.
 * 
 * @param state The state matrix.
 */
void subBytes(std::uint8_t state[4][4]) noexcept
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            state[i][j] = SBOX[state[i][j]];
        }
    }
}

/**
 * @brief Inverses the substitution of bytes in the state matrix using the inverse S-Box.
 * 
 * @param state The state matrix.
 */
void inverseSubBytes(std::uint8_t state[4][4]) noexcept
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            state[i][j] = INV_SBOX[state[i][j]];
        }
    }
}

/**
 * @brief Shifts rows in the state matrix.
 * 
 * @param state The state matrix.
 */
void shiftRows(std::uint8_t state[4][4]) noexcept
{
    std::uint8_t temp;

    // Shift row 1 (1st row, index 0) - left shift by 1
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    // Shift row 2 (2nd row, index 1) - left shift by 2
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // Shift row 3 (3rd row, index 2) - left shift by 3
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

/**
 * @brief Shifts rows in the state matrix in the inverse direction.
 * 
 * @param state The state matrix.
 */
void inverseShiftRows(std::uint8_t state[4][4]) noexcept
{
    std::uint8_t temp;

    // Inverse shift row 1 - no change

    // Inverse shift row 2 (2nd row, index 1) - right shift by 2
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;

    // Inverse shift row 3 (3rd row, index 2) - right shift by 3
    temp = state[2][0];
    state[2][0] = state[2][3];
    state[2][3] = state[2][2];
    state[2][2] = state[2][1];
    state[2][1] = temp;

    // Inverse shift row 4 (4th row, index 3) - right shift by 1
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

/**
 * @brief Mixes the columns of the state matrix.
 * 
 * @param state The state matrix.
 */
void mixColumns(std::uint8_t state[4][4]) noexcept
{
    for (int j = 0; j < 4; j++)
    {
        std::uint8_t a[4];
        for (int i = 0; i < 4; i++)
        {
            a[i] = state[i][j];
        }

        state[0][j] = (0x02 * a[0]) ^ (0x03 * a[1]) ^ (0x01 * a[2]) ^ (0x01 * a[3]);
        state[1][j] = (0x01 * a[0]) ^ (0x02 * a[1]) ^ (0x03 * a[2]) ^ (0x01 * a[3]);
        state[2][j] = (0x01 * a[0]) ^ (0x01 * a[1]) ^ (0x02 * a[2]) ^ (0x03 * a[3]);
        state[3][j] = (0x03 * a[0]) ^ (0x01 * a[1]) ^ (0x01 * a[2]) ^ (0x02 * a[3]);
    }
}

/**
 * @brief Mixes the columns of the state matrix in the inverse direction.
 * 
 * @param state The state matrix.
 */
void inverseMixColumns(std::uint8_t state[4][4]) noexcept
{
    for (int j = 0; j < 4; j++)
    {
        std::uint8_t a[4];
        for (int i = 0; i < 4; i++)
        {
            a[i] = state[i][j];
        }

        state[0][j] = (0x03 * a[0]) ^ (0x01 * a[1]) ^ (0x01 * a[2]) ^ (0x02 * a[3]);
        state[1][j] = (0x01 * a[0]) ^ (0x01 * a[1]) ^ (0x02 * a[2]) ^ (0x03 * a[3]);
        state[2][j] = (0x01 * a[0]) ^ (0x02 * a[1]) ^ (0x02 * a[2]) ^ (0x03 * a[3]);
        state[3][j] = (0x02 * a[0]) ^ (0x03 * a[1]) ^ (0x01 * a[2]) ^ (0x01 * a[3]);
    }
}

/**
 * @brief Expands the key for use in the AES algorithm.
 * 
 * @param key The original encryption key.
 * @param roundKeys The array to hold the expanded keys.
 */
void keyExpansion(const std::uint8_t* key, std::uint8_t* roundKeys) noexcept
{
    for (int i = 0; i < 16; ++i)
    {
        roundKeys[i] = key[i];
    }

    for (int i = 1; i <= 10; ++i)
    {
        std::uint8_t temp[4];
        for (int j = 0; j < 4; ++j)
        {
            temp[j] = roundKeys[(i - 1) * 16 + j + 12];
        }

        // Rotate the last 4 bytes
        std::uint8_t t = temp[0];
        for (int j = 0; j < 3; ++j)
        {
            temp[j] = temp[j + 1];
        }

        temp[3] = t;

        for (int j = 0; j < 4; ++j)
        {
            temp[j] = SBOX[temp[j]];
        }

        temp[0] ^= RCON[i - 1];

        // Generate the round key for this round
        roundKeys[i * 16 + 0] = roundKeys[(i - 1) * 16 + 0] ^ temp[0];
        roundKeys[i * 16 + 1] = roundKeys[(i - 1) * 16 + 1] ^ temp[1];
        roundKeys[i * 16 + 2] = roundKeys[(i - 1) * 16 + 2] ^ temp[2];
        roundKeys[i * 16 + 3] = roundKeys[(i - 1) * 16 + 3] ^ temp[3];

        // Fill in the rest of the round keys
        for (int j = 1; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                roundKeys[i * 16 + j * 4 + k] = roundKeys[i * 16 + (j - 1) * 4 + k] ^ roundKeys[i * 16 + j * 4 + k - 4];
            }
        }
    }
}

/**
 * @brief Encrypts a 128-bit block of data using the provided key.
 * 
 * @param input The input block (16 bytes) to be encrypted.
 * @param key The encryption key (16 bytes).
 * @param output The output block (16 bytes) that will hold the encrypted data.
 * 
 * @throws std::runtime_error if the input or key size is invalid.
 */
void encrypt(const std::uint8_t input[16], const std::uint8_t key[16], std::uint8_t output[16])
{
    if (!input || !key || !output)
    {
        throw std::runtime_error("Input, key, and output must not be null.");
    }

    std::uint8_t state[4][4];
    std::uint8_t roundKeys[176];

    // Copy input to state matrix
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = input[i * 4 + j];
        }
    }

    keyExpansion(key, roundKeys);

    // Initial round
    addRoundKey(state, reinterpret_cast<std::uint8_t(*)[4]>(roundKeys));

    // Main rounds
    for (int round = 1; round <= 9; ++round)
    {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, reinterpret_cast<std::uint8_t(*)[4]>(roundKeys + round * 16));
    }

    // Final round
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, reinterpret_cast<std::uint8_t(*)[4]>(roundKeys + 10 * 16));

    // Copy state back to output
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            output[i * 4 + j] = state[j][i];
        }
    }
}

/**
 * @brief Decrypts a 128-bit block of data using the provided key.
 * 
 * @param input The input block (16 bytes) to be decrypted.
 * @param key The decryption key (16 bytes).
 * @param output The output block (16 bytes) that will hold the decrypted data.
 * 
 * @throws std::runtime_error if the input or key size is invalid.
 */
void decrypt(const std::uint8_t input[16], const std::uint8_t key[16], std::uint8_t output[16])
{
    if (!input || !key || !output)
    {
        throw std::runtime_error("Input, key, and output must not be null.");
    }

    std::uint8_t state[4][4];
    std::uint8_t roundKeys[176];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            state[j][i] = input[i * 4 + j];
        }
    }

    keyExpansion(key, roundKeys);
    addRoundKey(state, reinterpret_cast<std::uint8_t(*)[4]>(roundKeys + 10 * 16));

    // Main rounds
    for (int round = 9; round >= 1; --round)
    {
        inverseShiftRows(state);
        inverseSubBytes(state);
        addRoundKey(state, reinterpret_cast<std::uint8_t(*)[4]>(roundKeys + round * 16));
        inverseMixColumns(state);
    }

    // Final round
    inverseShiftRows(state);
    inverseSubBytes(state);
    addRoundKey(state, reinterpret_cast<std::uint8_t(*)[4]>(roundKeys));

    // Copy state back to output (row-major order)
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            output[i * 4 + j] = state[j][i];
        }
    }
}

} // namespace aes

} // namespace nstd
