#include <iostream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

// Initial Permutation (IP)
const int IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// Final Permutation (FP)
const int FP[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// Expansion Permutation (E)
const int E[] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

// Permutation Function (P)
const int P[] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

// S-Boxes (S1 to S8)
const int S[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

// Key Permutation (PC1)
const int PC1[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

// Key Permutation (PC2)
const int PC2[] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Number of left shifts per round
const int SHIFTS[] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// Function prototypes
string string_to_binary(string text);
string binary_to_string(string binary);
string permute(string key, const int* table, int size);
string xor_strings(string a, string b);
vector<string> generate_keys(string key);
string feistel(string block, string key);
string des_encrypt(string plaintext, string key);
string des_decrypt(string ciphertext, string key);

int main() {
    string plaintext, key;

    cout << "Enter plaintext (8 characters): ";
    getline(cin, plaintext);

    cout << "Enter key (8 characters): ";
    getline(cin, key);

    if (plaintext.length() > 8) {
        plaintext = plaintext.substr(0, 8);
    }
    else {
        while (plaintext.length() < 8) {
            plaintext += ' ';
        }
    }

    if (key.length() > 8) {
        key = key.substr(0, 8);
    }
    else {
        while (key.length() < 8) {
            key += ' ';
        }
    }

    string ciphertext = des_encrypt(plaintext, key);
    cout << "Ciphertext: " << ciphertext << endl;

    string decrypted_text = des_decrypt(ciphertext, key);
    cout << "Decrypted text: " << decrypted_text << endl;

    return 0;
}

string string_to_binary(string text) {
    string binary_string = "";
    for (char& c : text) {
        binary_string += bitset<8>(c).to_string();
    }
    return binary_string;
}

string binary_to_string(string binary) {
    string text = "";
    for (size_t i = 0; i < binary.length(); i += 8) {
        string byte = binary.substr(i, 8);
        text += (char)bitset<8>(byte).to_ulong();
    }
    return text;
}

string permute(string key, const int* table, int size) {
    string permuted_key = "";
    for (int i = 0; i < size; i++) {
        permuted_key += key[table[i] - 1];
    }
    return permuted_key;
}

string xor_strings(string a, string b) {
    string result = "";
    for (size_t i = 0; i < a.length(); i++) {
        result += (a[i] == b[i] ? '0' : '1');
    }
    return result;
}

vector<string> generate_keys(string key) {
    vector<string> round_keys;
    string binary_key = string_to_binary(key);
    string permuted_key = permute(binary_key, PC1, 56);

    string left = permuted_key.substr(0, 28);
    string right = permuted_key.substr(28, 28);

    for (int i = 0; i < 16; i++) {
        left = left.substr(SHIFTS[i]) + left.substr(0, SHIFTS[i]);
        right = right.substr(SHIFTS[i]) + right.substr(0, SHIFTS[i]);

        string combined_key = left + right;
        round_keys.push_back(permute(combined_key, PC2, 48));
    }

    return round_keys;
}

string feistel(string block, string key) {
    string expanded_block = permute(block, E, 48);
    string xored_block = xor_strings(expanded_block, key);

    string substituted_block = "";
    for (int i = 0; i < 8; i++) {
        string sub_block = xored_block.substr(i * 6, 6);
        int row = bitset<2>(sub_block.substr(0, 1) + sub_block.substr(5, 1)).to_ulong();
        int col = bitset<4>(sub_block.substr(1, 4)).to_ulong();
        substituted_block += bitset<4>(S[i][row][col]).to_string();
    }

    return permute(substituted_block, P, 32);
}

string des_encrypt(string plaintext, string key) {
    string binary_plaintext = string_to_binary(plaintext);
    string permuted_plaintext = permute(binary_plaintext, IP, 64);

    string left = permuted_plaintext.substr(0, 32);
    string right = permuted_plaintext.substr(32, 32);

    vector<string> round_keys = generate_keys(key);

    for (int i = 0; i < 16; i++) {
        string new_right = xor_strings(left, feistel(right, round_keys[i]));
        left = right;
        right = new_right;
    }

    string combined_text = right + left;
    string ciphertext = permute(combined_text, FP, 64);

    return binary_to_string(ciphertext);
}

string des_decrypt(string ciphertext, string key) {
    string binary_ciphertext = string_to_binary(ciphertext);
    string permuted_ciphertext = permute(binary_ciphertext, IP, 64);

    string left = permuted_ciphertext.substr(0, 32);
    string right = permuted_ciphertext.substr(32, 32);

    vector<string> round_keys = generate_keys(key);

    for (int i = 15; i >= 0; i--) {
        string new_right = xor_strings(left, feistel(right, round_keys[i]));
        left = right;
        right = new_right;
    }

    string combined_text = right + left;
    string plaintext = permute(combined_text, FP, 64);

    return binary_to_string(plaintext);
}
