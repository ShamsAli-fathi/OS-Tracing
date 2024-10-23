# Description

This lab will involve two main tasks: first, we will analyze system call behavior using **strace**, and second, we will trace the scheduler and CPU threads using **LTTng** and **TraceCompass**.

In the [last section of this Lab](#Assignment), an assignment is also provided.

## Tools

- Ubuntu Distro

- strace

- LTTng 2.13

- Trace Compass

## System Calls

**strace** is a diagnostic, debugging and instructional userspace utility for Linux. It is used to monitor and tamper with interactions between processes and the Linux kernel, which include system calls, signal deliveries, and changes of process state.

> strace always runs the program. So it is not suitable to trace programs that might contain viruses using strace.

To simply trace a program:

```shell
strace my_program
```

The output shows all the system calls made after the execution of the program. But we can level our tracing up using a couple of useful flags

> You can attach strace to an already running process using `-p` flag.

> The output can be written in a _txt_ file using `-o` flag.

> Filtering is possible using `-e trace = ''`. For instance, `-e trace = 'close'` or `-e trace = '!write'`.

> strace does not trace child processes by default; hence the use of `-f` flag.

The following is an example of tracing ping:

![Ping Trace](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%201/src/pingstrace.png)

## Tracing a Multi-thread process

**LTTng** is an open source framework for correlated tracing of the Linux kernel, applications and libraries. LTTng's value lies in its effective integration with **TraceCompass**. By initiating tracing sessions with LTTng and analyzing the results in TraceCompass, we can leverage LTTng's versatile features for valuable insights.

To initiate an LTTng tracing session, we should first use the _create_ command:

```shell
lttng create session1
```
After the session is created, we could then enable the events we intend to trace. 

> To see all the available kernel events, we can use `lttng list -k`.

For instance, in order to trace the _sched_switch_ and event all _irq_ events:

```shell
lttng enable-event -k sched_switch, irq_'*'
```

> To enable all system calls in kernel, use `--syscall --all`.

By including additional contexts, we can elevate our tracing session and extract more valuable information. To list all the available contexts, use `lttng add-context --list`. Say you wanted to trace Virtual Process ID and the Process Name:

```shell
lttng add-context -k -t procname -t vtid
```

After mentioning all these, we then proceed to start the tracing, wait for as long as we want, then stop the session and destroy it; respectively:

```shell
lttng start
lttng stop
lttng destroy
```

> It is best practice to create a bash script with all the needed commands and then run it.

> You could choose the output directory with `lttng create sessionName --output=/example/example`

After finalizing the tracing session, a folder is created which contains **Channels**. A channel is a logical grouping of trace events that defines how and where the events are written during a tracing session. Channels are useful for controlling the behavior of event collection, including the buffering, output, and filtering of events. Channels allow you to group related events. For example, you could have separate channels for different types of events, like file I/O events in one channel and network events in another and also, they control how events are buffered. You can tune the size of the buffer for each channel and decide whether events are dropped, overwritten, or flushed to disk.

> Make sure your user has access to the files.

The next step is to attach this folder to tracing compass. After creating a tracing project, _Right Click_ -> _Tracing_ -> _Trace Import_, and browse for the folder created by LTTng.

![TraceCompass 1](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%201/src/tracecompass1.png)

Next, open your project and double click on experiments to view the trace.

![TraceCompass 2](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%201/src/tracecompass2.png)

The **Control Flow** view is a LTTng-specific view that shows per-process events graphically. The Linux Kernel Analysis is executed the first time a LTTng Kernel is opened. After opening the trace, the element Control Flow is added under the Linux Kernel Analysis tree element in the Project Explorer. To open the view, double-click the Control Flow tree element. 

**Resources View** is specific to LTTng kernel traces. The Linux Kernel Analysis is executed the first time a LTTng Kernel is opened. After opening the trace, the element Resources is added under the Linux Kernel Analysis tree element of the Project Explorer. To open the view, double-click the Resources tree element. This view shows the state of system resources i.e. if changes occurred during the trace either on CPUs, IRQs or soft IRQs, it will appear in this view. The left side of the view present a list of resources that are affected by at least one event of the trace. The right side illustrate the state in which each resource is at some point in time. For state USERMODE it also prints the process name in the state bar. For state SYSCALL the name of the system call is displayed in the state region. 

### The full guide is available [Here](https://www.youtube.com/@deeplinux2248) and [Here](https://archive.eclipse.org/tracecompass/doc/stable/org.eclipse.tracecompass.doc.user/Trace-Compass-Main-Features.html) and [Here](https://archive.eclipse.org/tracecompass/doc/stable/org.eclipse.tracecompass.doc.user/LTTng-Kernel-Analysis.html)

## Assignment

#### Now do it yourself!

- A simple C program is provided 
[Here](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%201/src/simple_program.c)
. Compile the program and run/trace it using _strace_; Explain all the system calls that are made. Run it for a second time `-f` flag and see how different the output is.

- **stress-ng** is a tool used to stress-test various system resources (CPU, memory, I/O, etc.) to evaluate system performance or stability under load. One of the tests include matrix multiplication. The goal is to engage a few number of your cores with matrix multiplication and trace the load. For events, include **sched_switch** and all **sched_wak*** events, **interrupts**, **sched_process_fork** and **sched_process_exit**. For contexts, include **Virtual process ID**, **Virutal Thread ID**, **Process Name** and **Priority**. Explain how the scheduler works, observe state transitions and identify patterns.

```shell
stress-ng --matrix <number of cores> --matrix-size 128 --timeout 10s
```
