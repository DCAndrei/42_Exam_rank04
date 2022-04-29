# 42_Exam_rank04
Exam rank04 at 42Wolfsburg

The exam for rank04 at 42 Wolfsburg school.

The solution is using the argv as argument for the execve, so there is no need for malloc and free, if the ";" and "|" are replaced by NULL.

The solution is using "serial" piping, as we are not allowed to use more than 30 open fds, even if we get tested with 82 pipes. If the pipes were run in parallel we would use for 82 commands, 82 x 2 + 3 fds, more than 30 allowed.

The solution was used in the exam passed with 100%.
