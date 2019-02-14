#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

using std::string;
using std::vector;

void get_pwd();

void run_shell_cmd(vector<string> tokens, int &index);

void run_system_cmd(vector<string> tokens, int &index);

void execute_commands(vector<string> tokens);

#endif // COMMAND_H
