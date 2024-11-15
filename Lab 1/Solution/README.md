## Solution 1
After compiling the _C_ program, we then simply run and trace the program:

```bash
strace ./simple_program
```

The system calls are as follows (the more obvious ones are not mentioned) : 

| System Calls    | Description |
| -------- | ------- |
| execve  | executes the program    |
| brk | adjusts the heap; the returned number is the end of the heap     |
| access    | checks if a file exists and readable    |
| fstat | retrieves file meta data from file descriptor |
| mmap | maps the file contents into memory |
| mprotect | sets the memory region to a certain access state (here is read-only) |
| clone | creates a new thread or child process |


After using the `-f` flag, we can see that the system calls of the child process is also shown in
the trace output.

## Solution 2

In order to start the tracing session using LTTng, we can create a bash script:

```bash
lttng create $1 --output=/.../...//test
lttng enable-event -k sched_switch,sched_wak'*',sched_pr'*',irq_'*'
lttng add-context -k -t vtid -t vpid -t procname -t prio
lttng start
sleep 10
lttng stop
lttng destroy
```

During the trace, we then run the **stress-ng** test:

```bash
stress-ng --matrix <number of cores> --matrix-size 128 --timeout 10s
```

This initiates a load on the number of cores we have for 10 seconds.

After successfully ending the tracing session, we import it to TraceCompass.

In the Control Flow section, **stress-ng** process is found. Based on the number of cores our system runs with and whether its Native or Virtualized, We either see a set of new
 processes or a set of threads created. The difference is easily recognizable by looking at the PID and TID; for having the same IDs indicate new processes and if otherwise, new threads.
  **sched_process_fork** event is raised for each stress-ng process/thread.

Context switches are also visible in **sched_switch** events. These scheduling events show when threads are switched in and out of the CPU by the scheduler. These are followed by series
of interrupts before and after **sched** events.

By looking at the Resources tab, we see how different threads take turns executing, which demonstrates time-sharing. 