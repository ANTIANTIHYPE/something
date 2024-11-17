#pragma once

#include <cstdint>
#include <stdexcept>

namespace nstd
{
    /**
     * @brief AES (Advanced Encryption Standard) encryption class.
     * 
     * This class provides methods for encrypting data using the AES algorithm.
     * It supports 128-bit key size and operates on 128-bit blocks of data.
     */
    class aes
    {
    public:
        /**
         * @brief Encrypts a 128-bit block of data using the provided key.
         * 
         * @param input The input block (16 bytes) to be encrypted.
         * @param key The encryption key (16 bytes).
         * @param output The output block (16 bytes) that will hold the encrypted data.
         * 
         * @throws std::runtime_error if the input or key size is invalid.
         */
        void encrypt(std::uint8_t input[16], std::uint8_t key[16], std::uint8_t output[16])
        {
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

            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    output[i * 4 + j] = state[j][i];
                }
            }
        }

    private:
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
            0x8c, 0xa1, 0x89, 0x0d, 0x9c, 0x3f, 0x81, 0xf0,
            0x7c, 0x9a, 0x7d, 0x7b, 0x6b, 0x6f, 0x30, 0x01
        };

        const std::uint8_t RCON[10] = {
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
        };

        /**
         * @brief Adds the round key to the state matrix.
         * 
         * @param state The state matrix.
         * @param roundKey The round key to be added.
         */
        void addRoundKey(std::uint8_t state[4][4], std::uint8_t roundKey[4][4])
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
         * @param state The state matrix.
         */
        void subBytes(std::uint8_t state[4][4])
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
         * @brief Shifts rows in the state matrix.
         * @param state The state matrix.
         */
        void shiftRows(std::uint8_t state[4][4])
        {
            std::uint8_t temp;

            // Shift row 1
            temp = state[1][0];
            state[1][0] = state[1][1];
            state[1][1] = state[1][2];
            state[1][2] = state[1][3];
            state[1][3] = temp;

            // Shift row 2
            temp = state [2][0];
            state[2][0] = state[2][2];
            state[2][2] = temp;
            temp = state[2][1];
            state[2][1] = state[2][3];
            state[2][3] = temp;

            // Shift row 3
            temp = state[3][3];
            state[3][3] = state[3][2];
            state[3][2] = state[3][1];
            state[3][1] = state[3][0];
            state[3][0] = temp;
        }

        /**
         * @brief Mixes the columns of the state matrix.
         * @param state The state matrix.
         */
        void mixColumns(std::uint8_t state[4][4])
        {
            for (int j = 0; j < 4; j++)
            {
                std::uint8_t a[4];
                std::uint8_t b[4];
                for (int i = 0; i < 4; i++)
                {
                    a[i] = state[i][j];
                    b[i] = (state[i][j] << 1) ^ ((state[i][j] & 0x80) ? 0x1b : 0x00);
                }
                state[0][j] = b[0] ^ a[1] ^ a[2] ^ a[3];
                state[1][j] = a[0] ^ b[1] ^ a[2] ^ a[3];
                state[2][j] = a[0] ^ a[1] ^ b[2] ^ a[3];
                state[3][j] = a[0] ^ a[1] ^ a[2] ^ b[3];
            }
        }

        /**
         * @brief Expands the key for use in the AES algorithm.
         * 
         * @param key The original encryption key.
         * @param roundKeys The array to hold the expanded keys.
         */
        void keyExpansion(const std::uint8_t* key, std::uint8_t* roundKeys)
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

                // Rotate and substitute
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

                for (int j = 0; j < 4; ++j)
                {
                    roundKeys[i * 16 + j] = roundKeys[(i - 1) * 16 + j] ^ temp[j];
                }

                for (int j = 1; j < 4; ++j)
                {
                    for (int k = 0; k < 4; ++k)
                    {
                        roundKeys[i * 16 + j * 4 + k] = roundKeys[i * 16 + (j - 1) * 4 + k] ^ roundKeys[i * 16 + (j - 1) * 4 + k + 4];
                    }
                }
            }
        }
    }; // class aes
} // namespace nstd
