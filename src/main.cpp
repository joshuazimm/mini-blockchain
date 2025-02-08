#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <cstdint>
#include <random>
#include <thread>
#include <chrono>
#include <unistd.h>

#include "net/client_controller.h"
#include "net/master_controller.h"

/* ---------- DIFFICULTY ----------
This is a Fixed Target Difficulty Algorithm (or Static 
Difficulty PoW). The target value is computed as: 
target=1<<(32−TARGET_BITS)

This gives a threshhold that the first 32 bit integer must
be under in order to meet the target.

Example: 1 << (32 - 25) = 1 << 7 = 128
In this case the first 32 bit integer must be less than 128
The first 24 bits of a 32 bit integer must be 0 with this
example, which comes out to 6 leading 0s in hex.

As you increase this difficulty, more leading 0s are required
to meet the target, making it more improbable, and thus 
increasing the amount of work it will take to find a solution.
*/

const int TARGET_BITS = 25;
const uint32_t target = 1 << (32 - TARGET_BITS);

/* ---------- END DIFFICULTY ---------- */



/*
Returns a time_t high resolution timestamp. It is important to
use high resolution to avoid similar solutions by introducing
more entropy.
*/
time_t getTime() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}



/*
Uses a mersenne twist rng to generate a random nonce, giving
each thread/node a different starting point in the PoW,
preventing multiple nodes from doing the same work.
*/
uint64_t getRandomNonce() {
    static std::random_device rd;
    static std::mt19937_64 eng(rd());
    static std::uniform_int_distribution<uint64_t> distr;
    return distr(eng);
}



/*
Converts a hash to a hex string for more concise printing.
*/
std::string hashToHex(const unsigned char* hash) {
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}



/*
Extract first 4 bytes and convert them to a uint 32. If bytes
are less than target then return true for meeting PoW.
*/
bool meetsTarget(const unsigned char* hash) {
    uint32_t firstBytes = (hash[0] << 24) | (hash[1] << 16) | (hash[2] << 8) | hash[3];
    return firstBytes < target;
}



/* ---------- POW MINER LOOP ----------
This loop uses SHA256 hashing implemented by open ssl. It is
important that a PoW system utilizes a one-way (irreversible) 
and deterministic hashing algorithm such that the solution can
be verified and one cannot reverse engineer the input from the
solution.

Using this hashing method, a random input must be created to
get a random output. Each cycle, this random input is hashed and
compared to the target (see difficulty section). Each iteration's
hash is different than the last because of the Nonce.

The Nonce is a value that is incremented every loop iteration
which slightly changes the input into the SHA256 algorithm.

Altogether the input is the system time that mineBlock was called
combined with the Nonce (a single string in implementation)
*/
void mineBlock() {
    uint64_t nonce = getRandomNonce();
    time_t timestamp = getTime();
    unsigned char hash[SHA256_DIGEST_LENGTH];

    std::cout << "Mining new block...\n";

    while (true) {
        /*
        Combine timestamp and Nonce into a single string
        */
        std::stringstream ss;
        ss << timestamp << nonce;
        std::string input = ss.str();

        /*
        Use open ssl sha hashing functions to create and evaluate
        a SHA256 hash from the current time and nonce.

        Documentation:
        https://docs.openssl.org/3.2/man3/SHA256_Init/
        */
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, input.c_str(), input.size());
        SHA256_Final(hash, &sha256);

        if (meetsTarget(hash)) {
            std::cout << "Solved! Nonce: " << nonce << "\nHash: " << hashToHex(hash) << "\n\n";
            /*
            Sleep miner for 2 seconds to even distribute solution 
            times between nodes.
            */
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        }

        ++nonce;
    }
}

/* ---------- END POW MINER LOOP ---------- */



int main() {
    // Check environment for NETWORK env variable
    const char* network = std::getenv("NETWORK");
    if (network && std::string(network) == "master") {
        std::cout << "Master node\n";

        MasterController master_controller;
        master_controller.start_broadcast_thread();

        while (true) {
            // Generate random string for message TODO:TESTING
            std::string message = "Hello from master!";
            master_controller.send_message_to_clients(message.c_str(), message.size());
            sleep(5);
        }

    } else {
        std::cout << "Client node\n";

        ClientController client_controller;
        client_controller.start_broadcast_thread();

        while (true) {
            /*
            Utilize a thread based structure for scalability (miner
            pools) and also allows the toy example to sleep and 
            more granularly track the status of the loop from the 
            program side.
            */
            std::thread miningThread(mineBlock);
            miningThread.join();
        }
    }
}