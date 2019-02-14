#include "tokenizer.h"
#include "command.h"

int main(int argc, char **argv)
{
    while (true)
    {
        vector<string> tokens = tokenize();
        execute_commands(tokens);
    }
    return 0;
}