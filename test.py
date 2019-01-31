expected = [
    """[{g++},{-o},{tokenizer},{tokenizer.cpp},{-lreadline}]""",
    """[{ssh},{theoden@edoras.rohan}]""",
    """[{echo},{There's the rub}]""",
    """[{echo},{Theres},{the},{rub penned Shakespeare.}]""",
    """[{cat},{names.txt},{|},{sort},{|},{cut},{-f2},{-d }]""",
    """[{emacs},{&}]""",
    """[{cd},{/etc},{;},{cat},{passwd},{;},{cd},{-}]""",
    """[{find},{.},{-name},{*.txt},{-exec},{grep},{{}},{/dev/null},{;}]""",
    """[{passwd}]""",
    """[{grep},{"Holy Trickery Batman!" said Robin},{foobar.txt}]""",
    """[{grep},{Martin Luther King's work on},{biographies.txt}]""",
    """[{cd},{Joseph "Buster" Keaton}]""",
    """[{echo},{backslash doesn\t},{work},{here}]""",
    """[{for},{$d},{in},{$(ls)},{;},{do},{echo},{$d},{;},{ls},{$d},{;},{done}]""",
    """[{cat},{<},{read_me_a_story.txt}]""",
    """[{echo},{Once},{upon},{a},{time},{>},{bedtime.txt}]""",
]

cases = [
    """g++ -o tokenizer tokenizer.cpp -lreadline""",
    """ssh theoden@edoras.rohan""",
    """echo \"There's the rub\"""",
    """echo 'There's the rub' penned Shakespeare.""",
    """cat names.txt | sort | cut -f2 -d\\""",
    """emacs& """,
    """cd /etc; cat passwd ; cd -""",
    """find . -name '*.txt' -exec grep {} /dev/null \;""",
    """     passwd""",
    """grep "\"Holy Trickery Batman!\" said Robin" foobar.txt""",
    """grep "Martin Luther King's work on" biographies.txt""",
    """cd 'Joseph "Buster" Keaton'""",
    """echo 'backslash doesn\'t work here'""",
    """for $d in $(ls) ; do echo $d; ls $d; done""",
    """cat <read_me_a_story.txt""",
    """echo Once upon a time >bedtime.txt""",
]

# cases = """let\'s do some "crazy \"air quotes\""
# ls | sort >sorted_files.txt
# cat -n /etc/fstab""".split('\n')

# expected = """[{let's},{do},{some},{crazy "air quotes"}]
# [{ls},{|},{sort},{>},{sorted_files.txt}]
# [{cat},{-n},{/etc/fstab}]""".split('\n')

import os
import time
import subprocess

failed = []
for index, test in enumerate(cases):
	
	print("--------------------------------")
	print("Tokenizer Should Tokenize String:")
	print("Case: " + str(test) + '\n')
	
	command_to_run = """ echo \"""" + test + """\" | ./tokenizer """
	result = [" 1 ", " 2 "]
	
	os.system(command_to_run + '> tmp')

	result = open('tmp','r').read().split('\n')

	# result = os.popen('echo "' + test + '" | ./tokenizer ').read()
	# print("RES" + result)

	if(result[1] == expected[index]):
		print("\nPASSED\n")

	else:
		print("\nFAILED\n")
		failed.append({
			"test": test,
			"output": result[1],
			"expected": expected[index]
		})

	print(result[0] + "\n");

	print("result:   " + result[1])
	print("expected: " + expected[index])

	print("--------------------------------")


print("\n--------------------------------")
print("--------------------------------")

print("TOTAL TESTS PASSED: " + str(len(cases) - len(failed)) + " out of " + str(len(cases)))
print("\nTESTS FAILED: \n")
for case in failed:
	print("\t------\n\tTEST CASE:")
	print("\tcase:     " + case['test'])
	print("\toutput:   " + case['output'])
	print("\texpected: " + case['expected'])
	print("\t------\n")

