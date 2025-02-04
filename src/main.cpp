#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <ctime>
#include <cstdint>
#include <random>
#include <thread>
#include <chrono>

const int TARGET_BITS = 25; // Difficulty

time_t getHighResTimestamp() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

uint64_t getRandomNonce() {
    static std::random_device rd;
    static std::mt19937_64 eng(rd());
    static std::uniform_int_distribution<uint64_t> distr;
    return distr(eng);
}

// Converts a hash to a hex string
std::string hashToHex(const unsigned char* hash) {
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

// Checks if hash is below the target
bool meetsTarget(const unsigned char* hash) {
    uint32_t target = 1 << (32 - TARGET_BITS);
    uint32_t firstBytes = (hash[0] << 24) | (hash[1] << 16) | (hash[2] << 8) | hash[3];
    return firstBytes < target;
}

// Proof-of-work loop
void mineBlock() {
    uint64_t nonce = getRandomNonce();
    unsigned char hash[SHA256_DIGEST_LENGTH];

    std::cout << "Mining new block...\n";
    time_t timestamp = getHighResTimestamp();

    while (true) {
        // Prepare input
        std::stringstream ss;
        ss << timestamp << nonce;
        std::string input = ss.str();

        // Compute SHA-256 hash
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, input.c_str(), input.size());
        SHA256_Final(hash, &sha256);

        // Check if it meets difficulty
        if (meetsTarget(hash)) {
            std::cout << "Solved! Nonce: " << nonce << "\nHash: " << hashToHex(hash) << "\n\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        }
        nonce++;
    }
}

int main() {
    while (true) {
        std::thread miningThread(mineBlock);
        miningThread.join();
    }
}