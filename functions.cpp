#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <iterator>
#include <readline/readline.h>
using namespace std;

/**
 * takes in a char and determines if the char is a special char ->  | ; < > &
 * @param - char *c -> pointer to the char to see if is special
*/
bool isspecialchar(char c)
{
    return c == '|' || c == ';' || c == '<' || c == '>' || c == '&' ? true : false;
}

/**
 * takes in a raw string and parses out the tokens from the string
 * @param - input - the raw string to parse
 * returns - list of strings that contain all tokens in the string  
 * 
 *
*/
list<string> strtok_custom(string input)
{
    int length = input.length() + 1; // length of the string input
    int index = 0;                   // current index in parseing of string

    string current;      // holds the current token
    list<string> tokens; // holds the parsed tokens

    bool escaped = false;        // indicates if current char is escaped
    bool single_quoted = false;  // indicates if current char is in single quotes
    bool double_quoted = false;  // indicates if current char is in double quotes
    bool remove_current = false; // indicates if the current char will not be included in token

    for (index; index < length; index++)
    {
        // ------------------
        // UPDATE CURRENT STATE
        // ------------------
        // current character
        char c = input[index];

        // indicates that the current char will not be included in the token
        remove_current = false;
        escaped = false;

        // update escaped state (escape chars don't work in single quotes)
        if (input[index - 1] == '\\' && !single_quoted)
            escaped = true;

        // if escaped were removing the current char
        if (c == '\\' && !single_quoted && !escaped)
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
        // update double quote state (mirror section for updating single quote state)
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

        // ------------------
        // PARSE CURRENT CHAR
        // ------------------
        // handle whitespace (ends token if not in quotes and not escaped)
        if (isspace(c) && !single_quoted && !double_quoted && !escaped)
        {
            // end the current token if the token isn't empty
            if (current.length() > 0)
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        // handle special characters
        else if (isspecialchar(c) && !escaped)
        {
            // end the current token if the token isn't empty
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
            // end the current token if the token isn't empty
            if (current.length() > 0)
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        // if we made it this far add the char to the current token
        // (unless we're removing the current char)
        else if (!remove_current)
            current += c;
    }
    // once complete return the tokens we've found
    return tokens;
}

/**
 * takes in a list of strings and prints each
 * format -> [{token 1},{token 2}]
 * @param - tokens - the token list to print
*/
void printtokens(list<string> tokens)
{
    int number_tokens = tokens.size();
    int i = 0;

    if (number_tokens > 0)
    {
        list<string>::iterator it;
        cout << '[';
        for (it = tokens.begin(); it != tokens.end(); ++it)
        {
            cout << '{' << *it << '}';
            if (i < number_tokens - 1)
                cout << ',';

            i++;
        }
        cout << ']';
    }
    cout << "\n";
}

void tokenize()
{
    // alocate tokens list
    list<string> tokens;
    // get user input and parse
    tokens = strtok_custom(readline("> "));
    // print out each token
    printtokens(tokens);
}