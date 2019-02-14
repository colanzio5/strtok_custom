#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/procfs.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <vector>
#include <iterator>
#include <readline/readline.h>
using namespace std;

/**
 * takes in a char and determines if the char is a special char ->  | ; < > &
 * @param - char *c -> pointer to the char to see if is special
*/
bool is_special_char(char c)
{
    return c == '|' || c == ';' || c == '<' || c == '>' || c == '&' ? true : false;
}

/**
 * takes in a raw string and parses out the tokens from the string
 * @param - input - the raw string to parse
 * returns - vector of strings that contain all tokens in the string  
 * 
 *
*/
vector<string> strtok_custom(string input)
{
    int length = input.length() + 1; // length of the string input
    int index = 0;                   // current index in parseing of string

    string current;        // holds the current token
    vector<string> tokens; // holds the parsed tokens

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
        else if (is_special_char(c) && !escaped)
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

string get_pwd()
{
    try
    {
        char temp[10000];
        return (getcwd(temp, sizeof(temp)) ? std::string(temp) : std::string(""));
    }
    catch (const exception &e)
    {
        cout << "Unable to obtain current directory " << '\n';
    }
}

void run_shell_cmd(vector<string> tokens, int &index)
{

    string token = tokens[index];

    if (token == "cd")
    {
        if (++index < tokens.size())
        {
            string cmd = tokens[index];
            if (chdir(cmd.c_str()) == -1)
            {
                cout << "Directory does not exist or is not accessible." << '\n';
            }
        }
        else
        {
            cout << token << " - accepts exactly one argument" << '\n';
        }
    }
    else if (token == "pwd")
    {
        cout << get_pwd() << '\n';
    }
}

void run_system_cmd(vector<string> tokens, int &index)
{
    vector<char *> argv;
    int start = index;
    for (index; index < tokens.size() && tokens[index] != ";" && tokens[index] != "|"; index++)
    {
        argv.push_back(const_cast<char *>(tokens[index].c_str()));
    }

    string last_token = tokens.back();

    if (last_token == "|")
    {
        tokens.pop_back();
        tokens.push_back(";");
        cout << "Pipe not implemented\n";
    }

    argv.push_back(NULL);

    // run the command
    pid_t child_pid = fork();
    int child_status;
    int loc;

    // check if the fork failed
    if (child_pid < 0)
    {
        cout << "Unable to spawn program";
        return;
    }

    // if were in the child pid
    if (child_pid == 0)
    {
        child_status = execvp(argv[0], &argv[0]);
        cout << "Unable to execute" << argv[1] << '\n';
    }
    // or the parent
    else
    {
        waitpid(child_pid, &loc, WUNTRACED);
    }

    // once parent is done
    if (child_status < 0)
    {
        cout << "Process exited with error \n";
        return;
    }
    else
    {
        cout << "Process exited successfully \n";
        return;
    }
}

void execute_commands(vector<string> tokens)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        string token = tokens[i];

        // handle io redirect tokens
        if (token == "<" || token == ">" || token == "&")
        {
            i++;
        }
        // handle shell command tokens
        else if (token == "cd" || token == "pwd")
        {
            run_shell_cmd(tokens, i);
        }
        // handle system commands
        else if (token != ";" && token != "|")
        {
            run_system_cmd(tokens, i);
        }
    }
}

void tokenize()
{
    // alocate tokens vector
    vector<string> tokens;
    // get user input and parse
    tokens = strtok_custom(readline("> "));
    // execute the set of tokens as commands
    execute_commands(tokens);
}
