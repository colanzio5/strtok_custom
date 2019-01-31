#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <readline/readline.h>

using namespace std;

/**
 * takes in string and removes all instances of escape characters
 * @param - str -> the string to clense
**/
string removechar(string str, char c)
{
    string result;
    for (size_t i = 0; i < str.size(); i++)
    {
        char currentChar = str[i];
        if (currentChar != c)
            result += currentChar;
    }
    return result;
}
/**
 * takes in a char and determines if the char is a special char ->  | ; < > &
 * @param - char *c -> pointer to the char to see if is special
*/
bool isspecialchar(char c)
{
    if (c == '|' || c == ';' || c == '<' || c == '>' || c == '&')
    {
        return true;
    }
    return false;
}

/**
 * takes in a raw string and parses out the tokens from the string
 * @param - input - the raw string to parse
 * returns - vector of strings that contain all tokens in the string  
 *
*/
vector<string> strtok_custom(string input)
{
    int length = input.length() + 1;
    int index = 0;

    string current;
    vector<string> tokens;

    // state variables
    bool escaped = false;
    bool single_quoted = false;
    bool double_quoted = false;
    bool remove_current = false;

    for (index; index < length; index++)
    {
        // current character
        char c = input[index];

        // indicates that the current char will not be included in the token
        remove_current = false;
        escaped = false;

        // update escaped state
        if (input[index - 1] == '\\' && !single_quoted)
            escaped = true;

        // update escaped state
        if (input[index] == '\\' && !single_quoted && !escaped)
            remove_current = true;

        // update single quoted state
        if (c == '\'' && !escaped && !double_quoted)
        {
            if (!single_quoted)
            {
                single_quoted = true;
                if (!double_quoted)
                    remove_current = true;
            }
            else if (single_quoted)
            {
                single_quoted = false;
                if (!double_quoted)
                    remove_current = true;
            }
        }
        // update double quote state
        if (c == '\"' && !escaped && !single_quoted)
        {
            if (!double_quoted)
            {
                double_quoted = true;
                if (!single_quoted)
                    remove_current = true;
            }
            else if (double_quoted)
            {
                double_quoted = false;
                if (!single_quoted)
                    remove_current = true;
            }
        }
        // parse section
        // handle whitespace
        if (isspace(c) && !single_quoted && !double_quoted)
        {
            if (current.length() > 0)
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        // handle special characters
        else if (isspecialchar(c) && !escaped)
        {
            // end the current token
            if (current.length() > 0)
            {
                tokens.push_back(current);
                current.clear();
            }
            // add the special char to tokens
            tokens.push_back(string(1, c));
            current.clear();
        }
        // handle end of file
        else if (c == '\0')
        {
            if (current.length() > 0)
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        // if we made it this far add the char to the current token
        else
        {
            // if the char is marked to keep add it to the current token
            if (!remove_current)
            {
                current += c;
            }
        }
    }
    return tokens;
}

/**
 * takes in a vector of strings and prints each
 * format -> [{token 1},{token 2}]
 * @param - tokens - the token vector to print
*/
void printtokens(vector<string> tokens)
{
    int number_tokens = tokens.size();
    if (number_tokens > 0)
    {
        cout << '[';
        for (int i = 0; i < number_tokens; i++)
        {
            cout << '{' << tokens[i] << '}';
        }
        cout << ']';
    }
    cout << "]\n";
}

void tokenize()
{
    // get user input and parse
    vector<string> tokens;
    tokens = strtok_custom(readline("> "));

    // print out each token
    printtokens(tokens);
}