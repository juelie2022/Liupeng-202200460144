// Precomputed T-table for SM4
static const uint32_t SM4_TTABLE[4][256];

void sm4_ttable_crypt(const uint32_t rk[32], const uint8_t in[16], uint8_t out[16]) {
    uint32_t x[36];

    // Load input (same as before)

    // 32 rounds using T-table
    for (int i = 0; i < 32; i++) {
        uint32_t t = x[i + 1] ^ x[i + 2] ^ x[i + 3] ^ rk[i];
        x[i + 4] = x[i] ^
            SM4_TTABLE[0][(t >> 24) & 0xff] ^
            SM4_TTABLE[1][(t >> 16) & 0xff] ^
            SM4_TTABLE[2][(t >> 8) & 0xff] ^
            SM4_TTABLE[3][t & 0xff];
    }

    // Store output (same as before)
}