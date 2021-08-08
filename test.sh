#!/bin/bash

test_line () {
	echo $@
	echo $@ >> out.res
	./microshell $@ >> out.res &
	sleep .250
	echo >> out.res
	leaks microshell > leaks.res 2> /dev/null
	if grep "ROOT LEAK" < leaks.res > /dev/null 2> /dev/null ; then
		printf "\e[0;31mLEAKS\n\e[0m"
	fi
	pid=$( pgrep microshell )
	printf "\e[0;31m"
	lsof -c microshell | grep $pid | grep -v cwd | grep -v txt | grep -v 0r | grep -v 1w | grep -v 2u | grep microshel
	printf "\e[0m"
	kill -9 $pid
	wait $pid 2>/dev/null
	#cat -e out.res > out
}

printf "\e[1;32mCompile\n"
gcc -g -Wall -Werror -Wextra -DTEST_SH microshell.c -o microshell
printf "\e[1;36mTest\n\e[0m"
rm -f out.res leaks.res out
test_line /bin/ls

printf "\e[1;32mDone\e[0m\n"
rm -rf microshell.dSYM leaks.res