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
 * takes in a the set of tokens and an index of the token to run
 * shell commands are commands that we support directly with system api calls
 * pwd, cd
 */
void run_shell_cmd(vector<string> tokens, int &index)
{

    // get the token we're running
    string token = tokens[index];

    // if the token is cd, were changing directories
    if (token == "cd")
    {
        // if a single argument exists
        if (++index < tokens.size())
        {
            string cmd = tokens[index];
            if (chdir(cmd.c_str()) == -1)
            {
                cout << "Directory does not exist or is not accessible." << '\n';
            }
        }
        // alert user that cd accepts only one argument
        else
        {
            cout << token << " - accepts exactly one argument" << '\n';
        }
    }
    // if the token is pwd, we're getting the current directory path
    // todo: replace the try catch block with something more fault aware
    else if (token == "pwd")
    {
        try
        {
            char temp[10000];
            cout << (getcwd(temp, sizeof(temp)) ? string(temp) : string("Unable to obtain current directory")) << '\n';
        }
        catch (const exception &e)
        {
            cout << "Unable to obtain current directory " << '\n';
        }
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

    pid_t pid;
    int status;

    // fork the program
    // if fork returns < 0 the forking the main process has failed
    if ((pid = fork()) < 0)
    {
        cout << "\nUnable to spawn program";
        exit(1);
    }
    // if were in the child process
    else if (pid == 0)
    {
        // if execvp returns a status of < 0  an error has occured
        if (execvp(argv[0], &argv[0]) < 0)
        {
            cout << "\nProcess exited with error";
            exit(1);
        }
    }
    // otherwise we're in the parent process
    else
    {
        // wait fot the process to complete
        while (wait(&status) != pid)
            ;
        // if the child process exited with an ok status
        // alert the user the process has completed
        if (status == 0)
        {
            cout << "\nProcess exited successfully";
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