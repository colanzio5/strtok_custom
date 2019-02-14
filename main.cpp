#include "tokenizer.h"
#include "command.h"

#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
    while (true)
    {
        vector<string> tokens = tokenize();
        execute_commands(tokens);
    }
    return 0;
}