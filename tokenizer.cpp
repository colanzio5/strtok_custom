#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main(int argc, char **argv)
{
  tokenize();
  return 0;
}

/**

> let\'s do some "crazy \"air quotes\""
[{let's},{do},{some},{crazy "air quotes"}]
    
> ls | sort >sorted_files.txt 
[{ls},{|},{sort},{>},{sorted_files.txt}]

> cat -n /etc/fstab 
[{cat},{-n},{/etc/fstab}]
*/
