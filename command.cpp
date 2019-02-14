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
using std::string;
using std::vector;

/**
 * helper function to get the current directory path
 */
void get_pwd()
{
    try
    {
        char temp[10000];
        cout << (getcwd(temp, sizeof(temp)) ? string(temp) : string("")) << '\n';
    }
    catch (const exception &e)
    {
        cout << "Unable to obtain current directory " << '\n';
    }
}

/**
 * function to process known shell commands:
 * * cd
 * * pwd
 */
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
        get_pwd();
    }
}

/**
 * run a system commad by forking the process and calling the 
 * set of tokens through execvp
 */
void run_system_cmd(vector<string> tokens, int &index)
{
    // get the subset of tokens thats the next command to run
    vector<char *> argv;
    int start = index;
    for (index; index < tokens.size() && tokens[index] != ";" && tokens[index] != "|"; index++)
    {
        argv.push_back(const_cast<char *>(tokens[index].c_str()));
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
    // if the for continues and
    // were in the child process
    if (child_pid == 0)
    {
        child_status = execvp(argv[0], &argv[0]);
        cout << "\n Unable to execute" << argv[1] << '\n';
    }
    // otherwise were in the parent process
    // and we wait for child to complete
    else
    {
        waitpid(child_pid, &loc, WUNTRACED);
    }
    // once the child process has completed
    // let the user know what the exit
    // status of the command was
    if (child_status < 0)
    {
        cout << "\n Process exited with error \n";
        return;
    }
    else
    {
        cout << "\n Process exited successfully \n";
        return;
    }
}

int getTokenType(string token)
{
    // IO_REDIRECT - 1
    // SHELL COMMAND - 2
    // SYSTEM COMMAND - 3

    // handle io redirect tokens
    if (token == "<" || token == ">" || token == "&")
    {
        return 1;
    }
    // handle known shell command tokens
    else if (token == "cd" || token == "pwd")
    {
        return 2;
    }
    // handle othersystem commands
    else if (token != ";")
    {
        return 3;
    }
}

void execute_commands(vector<string> tokens)
{

    // look through the tokens and
    // replace any pipe with a semi colon
    bool pipe_found = false;
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "|")
        {
            pipe_found = true;
            tokens[i] = ";";
        }
    }
    // if we replaced any pipes
    // let the user know pipes arent implemented
    if (pipe_found)
        cout << "\n Pipe not implemented \n";

    // for each token process
    // the token as a command
    for (int i = 0; i < tokens.size(); i++)
    {
        string token = tokens[i];
        switch (getTokenType(token))
        {
        case 1:
            // IO_REDIRECT - 1
            i++;
            break;
        case 2:
            // SHELL COMMAND - 2
            run_shell_cmd(tokens, i);
            break;
        case 3:
            // SYSTEM COMMAND - 3
            run_system_cmd(tokens, i);
            break;
        }
    }
}