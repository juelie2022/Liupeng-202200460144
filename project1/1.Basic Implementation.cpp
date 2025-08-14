#include <stdint.h>
#include <string.h>

// SM4 basic implementation
#define SM4_BLOCK_SIZE 16
#define SM4_KEY_SCHEDULE 32

// S-box table
static const uint8_t SM4_SBOX[256] = {
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,
    // ... (full S-box table would be included here)
};

// System parameter FK
static const uint32_t FK[4] = {
    0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc
};

// Fixed parameter CK
static const uint32_t CK[32] = {
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    // ... (full CK array would be included here)
};

// Rotate left operation
static inline uint32_t rotl32(uint32_t x, uint8_t n) {
    return (x << n) | (x >> (32 - n));
}

// S-box lookup and linear transformation L
static uint32_t sm4_t(uint32_t x) {
    uint8_t b[4];
    b[0] = SM4_SBOX[(x >> 24) & 0xff];
    b[1] = SM4_SBOX[(x >> 16) & 0xff];
    b[2] = SM4_SBOX[(x >> 8) & 0xff];
    b[3] = SM4_SBOX[x & 0xff];

    uint32_t y = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
    return y ^ rotl32(y, 2) ^ rotl32(y, 10) ^ rotl32(y, 18) ^ rotl32(y, 24);
}

// Round function F
static uint32_t sm4_f(uint32_t x0, uint32_t x1, uint32_t x2, uint32_t x3, uint32_t rk) {
    return x0 ^ sm4_t(x1 ^ x2 ^ x3 ^ rk);
}

// Key expansion
void sm4_key_schedule(const uint8_t key[16], uint32_t rk[32]) {
    uint32_t k[36];

    // Initialize intermediate key
    k[0] = ((uint32_t)key[0] << 24) | ((uint32_t)key[1] << 16) |
        ((uint32_t)key[2] << 8) | (uint32_t)key[3]) ^ FK[0];
    k[1] = ((uint32_t)key[4] << 24) | ((uint32_t)key[5] << 16) |
        ((uint32_t)key[6] << 8) | (uint32_t)key[7]) ^ FK[1];
    k[2] = ((uint32_t)key[8] << 24) | ((uint32_t)key[9] << 16) |
        ((uint32_t)key[10] << 8) | (uint32_t)key[11]) ^ FK[2];
    k[3] = ((uint32_t)key[12] << 24) | ((uint32_t)key[13] << 16) |
        ((uint32_t)key[14] << 8) | (uint32_t)key[15]) ^ FK[3];

    // Generate round keys
    for (int i = 0; i < 32; i++) {
        k[i + 4] = k[i] ^ sm4_t(k[i + 1] ^ k[i + 2] ^ k[i + 3] ^ CK[i]);
        rk[i] = k[i + 4];
    }
}

// SM4 encryption/decryption (decryption uses reversed round keys)
void sm4_crypt(const uint32_t rk[32], const uint8_t in[16], uint8_t out[16]) {
    uint32_t x[36];

    // Load input
    x[0] = ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16) |
        ((uint32_t)in[2] << 8) | (uint32_t)in[3];
    x[1] = ((uint32_t)in[4] << 24) | ((uint32_t)in[5] << 16) |
        ((uint32_t)in[6] << 8) | (uint32_t)in[7];
    x[2] = ((uint32_t)in[8] << 24) | ((uint32_t)in[9] << 16) |
        ((uint32_t)in[10] << 8) | (uint32_t)in[11];
    x[3] = ((uint32_t)in[12] << 24) | ((uint32_t)in[13] << 16) |
        ((uint32_t)in[14] << 8) | (uint32_t)in[15];

    // 32 rounds
    for (int i = 0; i < 32; i++) {
        x[i + 4] = sm4_f(x[i], x[i + 1], x[i + 2], x[i + 3], rk[i]);
    }

    // Store output (reverse last 4 words)
    uint32_t tmp[4] = { x[35], x[34], x[33], x[32] };
    for (int i = 0; i < 4; i++) {
        out[i * 4] = (tmp[i] >> 24) & 0xff;
        out[i * 4 + 1] = (tmp[i] >> 16) & 0xff;
        out[i * 4 + 2] = (tmp[i] >> 8) & 0xff;
        out[i * 4 + 3] = tmp[i] & 0xff;
    }
}