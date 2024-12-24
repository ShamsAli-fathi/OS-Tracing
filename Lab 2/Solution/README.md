## Solution

First we need to use `perf stat` to be informed of the raised events after we run the program:

```bash
sudo perf stat -ae 'kmem:*,net:netif*,syscalls:sys_enter_mmap,syscalls:sys_exit_mmap,tcp:*,i2c:*' ./program
```

![stat](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%202/src/perf_stat_sol.png)

We can simply see that **syscalls:sys_enter_mmap** and **syscalls:sys_exit_mmap** are the only raised events and as the names indicate, they are related to **mmap**.

Now it's time to record the kernel stake based on these events:

```bash
sudo perf record -ae 'kmem:*,syscalls:sys_enter_mmap,syscalls:sys_exit_mmap' --call-graph fp ./program
```

![record](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%202/src/perf_record_sol.png)

By looking at the stack, we can vividly see that a set of different **mmap** functions are being called which give us the idea of managing page offsets, setting up memory map and performing the operations. In order to specifically see how *do_mmap* works, we can *ftrace*.

![ftrace](https://github.com/ShamsAli-fathi/OS-Tracing/blob/main/Lab%202/src/ftrace_sol.png)

The nested functions surrounding *do_mmap* can give us a thorough idea about how it works. The *do_mmap* function handles the creation of a new memory mapping in a process’s address space, typically invoked during operations like mmap system calls or binary loading. It begins by calling *get_unmapped_area* to find a suitable address range, using functions like *thp_get_unmapped_area* for Transparent Huge Pages and *security_mmap_addr* to validate the address from a security perspective. It then performs checks such as *mlock_future_check* to ensure the mapping adheres to locked memory limits and *path_noexec* to verify the mapping is executable. Next, *mmap_region* finalizes the mapping by setting up Virtual Memory Area (VMA) structures, involving calls to *may_expand_vm* to ensure the virtual memory size is within limits, *is_file_shm_hugepages* to check for huge page compatibility, and vma_merge to merge adjacent VMAs. Additionally, it uses vm_area_alloc to allocate a new VMA structure, which involves memory allocation via *kmem_cache_alloc* and kernel synchronization checks like *__cond_resched* and *rcu_all_qs*. Each step contributes to efficiently managing memory mappings while adhering to system policies and resource constraints.