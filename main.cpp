#include "tokenizer.h"
#include "command.h"

using namespace std;
using std::string;
using std::vector;

int main(int argc, char **argv)
{
    while (true)
    {
        vector<string> tokens = tokenize();
        execute_commands(tokens);
    }
    return 0;
}