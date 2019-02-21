#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/procfs.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iterator>
#include <readline/readline.h>

using namespace std;
using std::string;
using std::vector;

/**
 * this is a helper function that returns the current directory path
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
 * takes in a the set of tokens and an index of the token to run
 * shell commands are commands that we support directly with system api calls
 * pwd, cd
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
 * takes in a the set of tokens and an index of the token to run
 * runs the command using execvp in a child process and waits for the process to complete
 * 
 * todo: need to write test cases for specific error conditions like fork failing and child process never halting
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

    // fork to create child process
    pid_t child;
    int child_status;
    child = fork();

    //if the child process is active
    if (child == 0)
    {
        // run the commands
        execvp(argv[0], &argv[0]);
        // if execvp fails, the function will get to this point
        cout << "\nProcess exited with error\n";
        exit(1);
    }
    // otherwise it's the parent process
    else
    {
        // if the fork failed
        if (child == (pid_t)(-1))
        {
            cout << "Unable to spawn program";
            exit(1);
        }
        else
        {
            // wait for the child process to complete
            wait(&child_status);
            if (child_status < 0)
            {
                cout << "\nProcess exited with error\n";
                return;
            }
            else
            {
                cout << "\nProcess exited successfully\n";
                return;
            }
        }
    }
}

/**
 * this function takes in a set of tokens from the strk_custom function
 * it parses the set of tokens as commands and runs the commands respectively.
 
 * todo: support pipe operators
 * todo: support io redirection
 */
void execute_commands(vector<string> tokens)
{

    cout << '\n';

    // check if the set of commands contains any pipes
    // if found replace w/ ';' and warn user pipes aren't implemented
    bool pipe_found = false;
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "|")
        {
            pipe_found = true;
            tokens[i] = ";";
        }
    }
    if (pipe_found)
        cout << "Pipe not implemented\n";

    // iterate through the tokens and parse each token as a commend from left to right
    // as of now we don't need to preparse and determine dependency tree for command set
    // todo: preparse the set of commands as a DAG so we can start handeling IO redirect
    for (int i = 0; i < tokens.size(); i++)
    {
        string token = tokens[i];

        // handle io redirect tokens
        if (token == "<" || token == ">" || token == "&")
        {
            i++;
        }
        // handle known shell command tokens
        else if (token == "cd" || token == "pwd")
        {
            run_shell_cmd(tokens, i);
        }
        // handle othersystem commands
        else if (token != ";")
        {
            run_system_cmd(tokens, i);
        }
    }
    cout << '\n';
}