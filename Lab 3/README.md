
# Description

This is the final Lab project. All the needed tools and tutorials are given and explained throughout the previous Labs. Hence, this lab only contains the assignment.

## Assignment
Previously, you have worked with **perf**, **LTTng** and **TraceCompass**; you now know that you can use `perf record` to capture kernel events and `perf stat` to observe the raised events for better understanding of what parts of kernel is being engaged when a program is run.

A compiled program is provided 
[Here](https://github.com/ShamsAli-fathi/OS-Tracing/tree/main/Lab%203/src)_.

1. Run the program first and see what it does. According to the output (and of course the name of the program itself :) ) , write your observation and prediction of what this program does and might be.

2. In order to fully understand what this program does, we need to pinpoint a certain kernel system call and trace it using LTTng and TraceCompass. But finding this system call is the main challenge here. Find the system call(s) behind this program and explain why.
    - **HINT 1:** Tracing system calls in LTTng is done a bit differently; you can trace a certain system call with `lttng enable-event -k --syscall <systemcall>` or simply `lttng enable-event -k --syscall --all` to trace all system calls. Then look into the program in TraceCompass to find the key system call.
    - **HINT 2:** `perf stat` could be of use if you want to narrow down or verify your answers.
    - **HINT 3:**  Always trace the **Scheduling** events!

3. Go to *Uaddr vs Waiter* section in *views*. Find the program's PID(s). Zoom out and simply by looking at it, explain what is happening and what can be concluded based on the bars.