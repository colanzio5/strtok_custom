
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

using namespace std;
using std::string;
using std::vector;

bool is_special_char(char c);

vector<string> strtok_custom(string input);

vector<string> tokenize();

#endif // TOKENIZER_H
