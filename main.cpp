/**
 * COLIN CASAZZA
 * CS570 - OPERATING SYSTEMS
 * CSSC1070
 * ASSIGNMENT TWO
 * 2/21/19
 */
#include <string>
#include <vector>
// tokenizer functions
#include "tokenizer.h"
// command functions
#include "command.h"
using namespace std;

int main(int argc, char **argv)
{
    while (true)
    {
        // take in a line and parse it as a set of tokens
        vector<string> tokens = tokenize();
        // take the set of tokens and parse them as a command
        execute_commands(tokens);
    }
    return 0;
}