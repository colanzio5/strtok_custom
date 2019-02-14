

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

    // if were in the child pid...
    if (child_pid == 0)
    {
        child_status = execvp(argv[0], &argv[0]);
        cout << "\n Unable to execute" << argv[1] << '\n';
    }
    // otherwise were in the parent pid...
    else
    {
        waitpid(child_pid, &loc, WUNTRACED);
    }
    // once parent is done...
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

void execute_commands(vector<string> tokens)
{

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
        cout << "\n Pipe not implemented \n";

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
}