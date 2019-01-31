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

// So long "and thanks's for the fishes"
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
        // otherwise add the char to the current token
        else
        {
            // if the char is marked to keep add it to the current token
            if (!remove_current)
            {
                current += c;
            }
        }
        // cout << "char: " << c << " , escaped: " << escaped << ", single quoted: " << single_quoted << " , double quoted: " << double_quoted << ", current: " << current << '\n';
    }
    return tokens;
}

void tokenize()
{

    vector<string> tokens;
    tokens = strtok_custom(readline("> "));

    if (tokens.size() > 0)
    {
        cout << '[';
        for (vector<string>::const_iterator i = tokens.begin(); i != tokens.end(); ++i)
        {
            cout << '{' << *i << '}';
            if (i < tokens.end() - 1)
            {
                cout << ',';
            }
        }

        cout << "]\n";
    }
    else
    {
        cout << '\n';
    }
}

// /**
//  * takes a string (read in by tokenize()) and returns vector of tokens
//  * @param - str -> the string we want to split into tokens
// */
// vector<string> strtok_custom(const string &input)
// {
//     // temp storage for manipulating the input string
//     string str = input;
//     // current token string
//     string current;
//     // total number of chars in input string
//     int total = str.length();
//     // markers for tracking the start char index of tokens
//     int tail = 0;
//     // storage for the tokens in the string
//     vector<string> tokens;
//     // state vector
//     vector<int> state;
//     state.push_back(0); // is escaped
//     state.push_back(0); // is in single quotes
//     state.push_back(0); // is in double quotes

//     /**
//      * iterate through each character in the input string
//      * for each character calculate the new state, then pass
//      * the current char into the state machine which adds new
//      * tokens to vector on valid states
//     **/
//     int i = 0;
//     for (i; i < total; i++)
//     {
//         // calculate the state for is escaped
//         str[i - 1] == '\\' ? state[0] = 1 : state[0] = 0;

//         // calculate the new state for is in single quotes
//         if (str[i] == '\'' && state[0] != 1)
//         {
//             // if were not in single quotes and not in double quotes
//             // then then a single quote has started
//             if (state[1] == 0 && state[2] == 0)
//             {
//                 // were in single quotes now
//                 state[1] = 1;
//                 // parse the quote out of the string so it doesnt end up in the token
//                 string tmp = str.substr(0, i);
//                 tmp.append(str.substr(i + 1, total));
//                 str = tmp;
//                 total--;
//             }
//             else if (state[1] == 1 && state[2] == 0)
//             {
//                 // were not in single quotes anymore
//                 state[1] = 0;
//                 // parse the quote out of the string so it doesnt end up in the token
//                 string tmp = str.substr(0, i);
//                 tmp.append(str.substr(i + 1, total));
//                 str = tmp;
//                 total--;
//             }
//         }
//         // calculate new state for is in double quotes (mirrors is in sinle quotes)
//         if (str[i] == '\"' && state[0] != 1)
//         {
//             if (state[2] == 0 && state[1] == 0)
//             {
//                 state[2] = 1;
//                 string tmp = str.substr(0, i);
//                 tmp.append(str.substr(i + 1, total));
//                 str = tmp;
//                 total--;
//             }
//             else if (state[2] == 1)
//             {
//                 state[2] = 0;
//                 string tmp = str.substr(0, i);
//                 tmp.append(str.substr(i + 1, total));
//                 str = tmp;
//                 total--;
//             }
//         }
//         // if we hit a space, end the token, unless were in single or double quotes
//         else if (isspace(str[i]) && state[1] == 0 && state[2] == 0)
//         {
//             // make sure were not adding an empty token
//             if (i - tail != 0)
//             {
//                 // end current token and add to tokens
//                 current = str.substr(tail, i - tail);
//                 tokens.push_back(current);
//             }
//             // progress token marker
//             tail = i + 1;
//         }
//         // if we hit a space, end the token, unless were in single or double quotes or the character has been escaped
//         else if (isspecialchar(str[i]) == 1 && state[0] != 1 && state[1] != 1 && state[2] != 1)
//         {
//             // make sure were not adding an empty token
//             if (i - tail != 0)
//             {
//                 // end current token and add to tokens
//                 current = str.substr(tail, i - tail);
//                 tokens.push_back(current);
//             }

//             // add special char to tokens
//             current = str.substr(i, 1);
//             tokens.push_back(current);
//             // progress token marker
//             tail = i + 1;
//         }

//         // handle escape characters
//         else if (str[i] == '\\' && state[0] != 1 && state[1] != 1)
//         {
//             // parse the escape character out of the string so it doesnt end up in the token
//             string tmp = str.substr(0, i);
//             tmp.append(str.substr(i + 1, total));
//             str = tmp;
//             total--;
//         }

//         // handle end of string
//         else if (str[i] == '\0')
//         {
//             // end current token and add to tokens
//             current = str.substr(tail, i - tail);
//             tokens.push_back(current);
//             tail = i;
//             cout << "edndnd";
//         }
//         cout << "char: " << str[i] << " , state[0]: " << state[0] << " , state[1]: " << state[1] << " , state[2]: " << state[2] << '\n';
//     }

//     // push anyhing left on as a token
//     // make sure were not adding an empty token
//     if (i - tail != 0)
//     {
//         // end current token and add to tokens
//         current = str.substr(tail, i - tail);
//         tokens.push_back(current);
//     }

//     return tokens;
// }
