#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <readline/readline.h>

using namespace std;

/**
	takes in a char and determines if the char is a special char ->  | ; < > &
	@param - char *c -> pointer to the char to see if is special
*/
int isspecialchar(char c)
{
    if (c == '|' || c == ';' || c == '<' || c == '>' || c == '&')
    {
        return 1;
    }
    return 0;
}

/**
	takes a string (read in by tokenize()) and returns vector of tokens
	@param - str -> the string we want to split into tokens
*/
vector<string> strtok_custom(const string &str)
{
    string current;

    // storage for the tokens in the string
    vector<string> tokens;

    // markers for tracking the start char index of tokens
    int tail = 0;

    // state vector -> is escaped, is in ', is in "
    vector<int> state;
    state.push_back(0);
    state.push_back(0);
    state.push_back(0);

    // index through the string character
    // current state machine is applied against the next state
    // to yield new token or no new token and a longer possible token
    for (int i = 0; i < str.length() + 1; i++)
    {

//        cout << "char: " << str[i] << " , state[0]: " << state[0] << " , state[1]: " << state[1] << " , state[2]: " << state[2] << '\n';

        // update state for state[0]
        str[i - 1] == '\\' ? state[0] = 1 : state[0] = 0;

        // update for state[1]
        if (str[i] == '\'') {
            if (state[1] == 0 ) {
		if(state[2] != 1) {
			state[1] = 1;
			tail = i+1;
		}
            } else if (state[1] == 1) {
                if(state[2] != 1) {
  		  state[1] = 0;
   	          // end of the token
	          current = str.substr(tail, i - tail);
        	  tokens.push_back(current);
  	          tail = i+1;
                }
	    }
        }

	// update for state[2]
        if (str[i] == '\"') {
            if (state[2] == 0 ) {
		if(state[1] != 1) {
			state[2] = 1;
			tail = i+1;
		}
            } else if (state[2] == 1) {
                if(state[1] != 1) {
  		  state[2] = 0;
   	          // end of the token
	          current = str.substr(tail, i - tail);
        	  tokens.push_back(current);
  	          tail = i+1;
                }
	    }
        }

        // handle spaces
        else if (isspace(str[i]) && state[1] == 0 && state[2] == 0)
        {
            if (state[1] != 1 && state[2] != 1)
            {
                // end of the token
		if(i-tail != 0){

                  current = str.substr(tail, i - tail);
                  tokens.push_back(current);
	       }
               tail = i+1;

            }
        }

        // handle special chars
        else if (isspecialchar(str[i]) == 1 && state[0] != 1 && state[1] != 1 && state[2] != 1)
        {

            if (i-tail != 0) {
              // end token and add this char as token
              // add prev token to tokens
              current = str.substr(tail, i - tail);
              tokens.push_back(current);
            }

            // add special char to tokens
            current = str.substr(i, 1);
            tokens.push_back(current);
            tail = i+1;
        }

        // handle end
        else if (str[i] == '\0') {
       	    // end of the token
            current = str.substr(tail, i - tail);
       	    tokens.push_back(current);
       	    tail = i;		
        }

    }

    tokens.pop_back();
    // return accumulated tokens
    return tokens;
}

void tokenize()
{

    vector<string> tokens;
    string line = readline("> ");
    line.append(" ");
    tokens = strtok_custom(line);

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

