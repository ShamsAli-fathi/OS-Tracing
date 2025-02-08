
# Description

This lab will involve two main tasks: first, we are introduced to one of the most important tracing tools, **perf**, and try to understand how its different features could be used. Then we proceed with another useful tool, **ftrace** and combine all these for a small tracing assignment.

In the [last section of this Lab](#Assignment), the assignment is provided.

## Tools

- Ubuntu Distro

- perf

- ftrace

## Perf
Perf is a powerful performance analysis toolset that allows you to profile and analyze the performance of your Linux system, both at the kernel and user-space levels. It provides a comprehensive suite of tools to identify performance bottlenecks, measure resource utilization, and optimize your system for maximum performance.

Using perf, we can record various tracing events, the number of times an event is raised, record the instruction stack and many more. The list of all the events can be seen:

```bash
sudo perf list
```
- Always use perf as sudo!

In order to simply record the events, we use the *record* command:

```bash
sudo perf record
```

By doing so, we record the whole kernel; But we always need to filter and narrow down our traces. Let's say we aim to trace all the scheduling events:

```bash
sudo perf record -ae 'sched:*' --call-graph fp
```

| command        | Description                                 |
| -------------- | ------------------------------------------- |
| -ae            | uses all CPUs and selects events            |
| --call-graph   | enables stack chain                         |
| fp             | a mode in call graph                        |

- Remember to exit and stop the record. It takes a lot of space if you let it keep going.
- If you want to record a program, append the program's directory at the end.

Afterwards, by using `perf script`, you can easily observe the stack of instructions.
The result is:

<!-- perf example -->
![perf](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%202/src/perf-record-example.png)

But most of the times, we are not sure where to actually look into! Maybe we want to see what events are specifically raised when we do a certain action or run a certain program. In such situations, it's better to use the _stat_ feature. By assigning a timer to _stat_ command, we can see the number of events being raised in these intervals:

```bash
sudo perf stat -ae 'sched:*' -I 1000
```
- If you want to record a program, append the program's directory at the end.

<!--perf stat gif-->
![perf stat](https://github.com/ShamsAli-fathi/Linux-Kernel-Tracing/blob/main/Kernel%20Net%20Tracing/src/Perf%20idle.gif)

There are lots of other features such as probes that you can use for more practical scenarios.


## ftrace

Ftrace is a tracing framework embedded within the Linux kernel. It provides a comprehensive view of the kernel's inner workings, enabling developers and system administrators to analyze performance bottlenecks, identify system issues, and gain insights into kernel behavior. We can records the entry and exit points of kernel functions and track the call stack, allowing for detailed analysis of function call sequences.

In order to access ftrace, we need to be in the root environment; `sudo su`. Then we go the following directory:

```bash
cd /sys/kernel/debug/tracing/
```
Then by simply typing `ls`, we see different folders and hooks, all with useful usages.

- The hooks we want to learn and work with here are the following:
    - available_tracers
    - available_filter_functions
    - tracing_on
    - current_tracer
    - set_ftrace_pid
    - trace
    - events

| hook                      | Description                                    |
| --------------            | -------------------------------------------    |
| available_tracers         | all the available tracing modes                |
| available_filter_functions| all the available function you could filter out|
| set_ftrace_filter         | the filtered functions are put here            |
| set_function_graph        | same as _set_ftrace_filer_, but for function graph tracing mode|
| tracing_on                | enable or disable tracing                      |
| current_tracer            | the tracing mode is put here                   |
| set_ftrace_pid            | assign the process id                          |
| trace                     | the report of the trace                        |
| events                    | the available tracing events                   |

- **All the hooks with enable and disable functionality are altered using `echo 1 > ...` or `echo 0 > ...` .**

For instance, the following filters certain functions and starts tracing:

```bash
echo 0 > tracing_on
echo function_graph > current_tracer
echo 'net* tcp*' > set_graph_function
echo 1 > tracing_on
```

Hence the result:

![ftrace](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%202/src/ftrace_example.png)
<!--ftrace-->

- To reset the trace report, `echo nop > current_tracer` each time.

## Assignment
#### Now do it yourself!

- A compiled program is provided 
[Here](https://github.com/ShamsAli-fathi/OS-Tracing/tree/main/Lab%202/src).
    - Find what events are being raised by the program; look into `kmem:*,net:netif*,syscalls:sys_enter_mmap,syscalls:sys_exit_mmap,tcp:*,i2c:*` events.
    - Based on what you found out, record the appropriate events using _perf_.
    - Find a function called **do_mmap**. Explain the whole stack it is located in and what it is actually doing.
    - Using _ftrace_, **enable the same events** you found out and trace the program, find the nested functions of `do_mmap` and verify how mapping works.

- **HINT:** using a pre-written script is recommended for using _ftrace_ here, in order to run and attach the program to ftrace. A sample is provided that you need to complete:

```bash
echo 0 > /sys/kernel/debug/tracing/tracing_on
echo nop > /sys/kernel/debug/tracing/current_tracer
echo ... > /sys/kernel/debug/tracing/current_tracer
echo 1 > ...
./program & echo $! > ...
sleep 3
echo 0 > /sys/kernel/debug/tracing/ ...
```
