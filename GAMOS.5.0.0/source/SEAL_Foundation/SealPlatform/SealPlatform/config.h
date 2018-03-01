/* SealPlatform/config.h.  Generated by configure.  */
// Looking for config.h but can't find it?  It is generated from this template
// and put under platform-specific includes in SEAL_X_Y_Z/$SCRAM_ARCH/include.

#ifndef SEAL_PLATFORM_CONFIG_H
# define SEAL_PLATFORM_CONFIG_H

# ifdef _WIN32
#  define _WIN32_WINNT 0x0500
# endif

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* C++ features --------------------------------------------------------- */

/* Define if you have working ANSI namespaces. */
#define HAVE_NAMESPACES 1

/* Define if standard library is in namespace `std'. */
#define HAVE_NAMESPACE_STD 1

/* Define if namespace `std::rel_ops' is defined. */
#define HAVE_NAMESPACE_STD_REL_OPS 1

/* Define if namespace `std::rel_ops' is not defined and relational
   operators are in namespace `std'.  */
/* #undef HAVE_REL_OPS_IN_STD */

/* Define if you have `__PRETTY_FUNCTION__'.  */
#define HAVE_PRETTY_FUNCTION 1

/* Define if you have `__FUNCTION__' but no `__PRETTY_FUNCTION__'.  */
/* #undef HAVE_FUNCTION */

/* Define if you have `__func__' but no `__PRETTY_FUNCTION__' nor
   `__FUNCTION__'  */
/* #undef HAVE_FUNC */


/* Define if you have long longs. */
#define HAVE_LONG_LONG 1

/* Define if you have __int64. */
/* #undef HAVE_INT64 */

/* Define if you have __int128. */
/* #undef HAVE_INT128 */

/* Define if you have bool keyword. */
#define HAVE_BOOL 1

/* Define if you have ANSI `explicit' keyword. */
#define HAVE_EXPLICIT 1

/* Define if you have ANSI `mutable' keyword. */
#define HAVE_MUTABLE 1

/* Define if you have ANSI `restrict' keyword. */
/* #undef HAVE_RESTRICT */

/* Define if you have ANSI-like `__restrict' keyword. */
#define HAVE_PRIVATE_RESTRICT 1

/* Define if you have ANSI `signed' keyword. */
#define HAVE_SIGNED 1

/* Define if you have working ANSI `using' directive. */
#define HAVE_USING 1

/* Define if you have ANSI `typename' keyword.  */
#define HAVE_TYPENAME 1

/* Define to `typename' if the keyword is required in base
   class declaration for nested types in templates, otherwise
   define to an empty string.  */
#define TYPENAME_IN_BASEDECL 

/* Define if you have new style casts (`static_cast',
   `const_cast' and `reinterpret_cast').  */
#define HAVE_NEW_STYLE_CASTS 1

/* Define if you have `dynamic_cast' operator.  */
#define HAVE_DYNAMIC_CAST 1

/* Define if you have `typeid' operator.  */
#define HAVE_TYPEID 1

/* Define to `if(0);else for' if `for' does not establish
   scope as per ANSI standard. */
/* #undef for */

/* Define if you have ANSI template instantiation. */
#define HAVE_ANSI_TEMPLATE_INSTANTIATION 1

/* Define if explicit template specialisation can specialise static
   data members of a template class without them being considered
   instantiation initialisations.  (Conforming compiler should have
   this set.) */
#define HAVE_SEPARATE_TEMPLATE_SPEC_INST 1

/* Define if you have template default arguments.  */
#define HAVE_TEMPLATE_DEFAULT_ARGS 1

/* Define if compiler allows null template arguments in functions.  */
#define HAVE_TEMPLATE_NULL_ARGS 1

/* Define if compiler allows null template arguments in specialisation.  */
#define HAVE_TEMPLATE_NULL_SPEC 1

/* Define if compiler allows partial template specialisation.  */
#define HAVE_TEMPLATE_PARTIAL_SPEC 1

/* Define if you have member templates.  */
#define HAVE_MEMBER_TEMPLATES 1

/* Define if you have ANSI operator-> for templates.  */
#define HAVE_ANSI_OPERATOR_ARROW 1

/* Define if class access rights are broken.  */
/* #undef HAVE_BROKEN_CLASS_ACCESS */

/* Define if you have <new> or <new.h> header file.  */
#define HAVE_NEW 1
/* #undef HAVE_NEW_H */

/* Define if `new_handler' and `set_new_handler' are in namespace `std'.  */
#define HAVE_STD_NEW_HANDLER 1

/* Define if you have new ANSI iostreams. */
#define HAVE_NEW_IOSTREAMS 1

/* Define if you have streambuf declared in <iosfwd>. */
#define HAVE_STREAMBUF_IN_IOSFWD 1

/* Define if ostream is overloaded for char. */
#define HAVE_OSTREAM_CHAR_OVERLOAD 1

/* Define if ostream is overloaded for long double. */
#define HAVE_OSTREAM_LONG_DOUBLE_OVERLOAD 1

/* Define if ostream is overloaded for long longs. */
#define HAVE_OSTREAM_LONG_LONG_OVERLOAD 1

/* Define if you have usable `iterator_traits' in <iterator>. */
#define HAVE_ITERATOR_TRAITS 1

/* Define if you have usable `iterator' in <iterator>. */
#define HAVE_ITERATOR 1

/* Define to 1 (iterator), 2 (all w/tag), or 3 (all w/o tag) depending
   on what kind of argument list `reverse_iterator' takes.  The first
   of means that HAVE_ITERATOR is also set.  */
#define HAVE_REVERSE_ITERATOR_STYLE 1

/* Define if you have standard C++ C headers like <cstdlib>. */
#define HAVE_CXX_STDC_HEADERS 1

/* Define if you have <typeinfo> or <typeinfo.h> header file.  */
#define HAVE_TYPEINFO 1
/* #undef HAVE_TYPEINFO_H */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* General system properties -------------------------------------------- */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <strings.h> header file.  */
#define HAVE_STRINGS_H 1

/* Define if you have the <sys/types.h> header file.  */
#define HAVE_SYS_TYPES_H 1

/* Define if you have the <sys/sysctl.h> header file.  */
#define HAVE_SYS_SYSCTL_H 1

/* Define if you have the <vm/vm_param.h> header file.  */
/* #undef HAVE_VM_VM_PARAM_H */

/* Define if you have the <sys/vmmeter.h> header file.  */
#define HAVE_SYS_VMMETER_H 1

/* Define if you have the <crt_externs.h> header file.  */
#define HAVE_CRT_EXTERNS_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the <windows.h> header file.  */
/* #undef HAVE_WINDOWS_H */

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the setenv function.  */
#define HAVE_SETENV 1

/* Define if you have the unsetenv function.  */
#define HAVE_UNSETENV 1

/* Define if utsname has member domainname. */
/* #undef HAVE_UTSNAME_DOMAINNAME */

/* Define if <sys/types.h> defines caddr_t.  */
#define HAVE_CADDR_T 1

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* System limit constants copied from stdint.h -------------------------- */

#define HAVE_STDINT_H 1

#define SEAL_INT8_MIN -128
#define SEAL_INT8_MAX 127
#define SEAL_UINT8_MAX 255

#define SEAL_INT16_MIN -32768
#define SEAL_INT16_MAX 32767
#define SEAL_UINT16_MAX 65535

#define SEAL_INT32_MIN (-2147483647-1)
#define SEAL_INT32_MAX 2147483647
#define SEAL_UINT32_MAX 4294967295U

#define SEAL_INT64_MIN (-9223372036854775807LL-1)
#define SEAL_INT64_MAX 9223372036854775807LL
#define SEAL_UINT64_MAX 18446744073709551615ULL

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* Signals -------------------------------------------------------------- */

/* Define to the signal handler return type.  */
#define RETTYPE_SIGHANDLER void

/* Define to the signal handler argument list type.  */
#define ARGS_SIGHANDLER int

/* Define if you have POSIX signal stuff.  */
#define HAVE_POSIX_SIGNALS 1

/* Define if you have POSIX real-time signal stuff.  */
/* #undef HAVE_POSIX_RT_SIGNALS */

/* Define if you have the strsignal function.  */
#define HAVE_STRSIGNAL 1
#define HAVE_STRSIGNAL_DECL 1

/* Define if you have raise. */
#define HAVE_RAISE 1
#define HAVE_RAISE_DECL 1

/* Define if you have sys_siglist in <signal.h>.  */
#define HAVE_SYS_SIGLIST 1

/* Define if siginfo_t has member si_utime.  */
/* #undef HAVE_SIGINFO_SI_UTIME */

/* Define if siginfo_t has member si_int.  */
/* #undef HAVE_SIGINFO_SI_INT */

/* Define if siginfo_t has member si_value.  */
/* #undef HAVE_SIGINFO_SI_VALUE */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* General I/O ---------------------------------------------------------- */

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <io.h> header file.  */
/* #undef HAVE_IO_H */

/* Define if you have the <sys/filio.h> header file.  */
#define HAVE_SYS_FILIO_H 1

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/uio.h> header file.  */
#define HAVE_SYS_UIO_H 1

/* Define if you have the <sys/wait.h> header file.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the <sys/param.h> header file.  */
#define HAVE_SYS_PARAM_H 1

/* Define if you have the <sys/mman.h> header file.  */
#define HAVE_SYS_MMAN_H 1

/* Define if you have solaris-compatible sendfile().  */
/* #undef HAVE_SOLARIS_SENDFILE */

/* Define if you have linux-compatible sendfile().  */
/* #undef HAVE_LINUX_SENDFILE */

/* Define if you have freebsd-compatible sendfile().  */
/* #undef HAVE_FREEBSD_SENDFILE */

/* Define if you have the pread function.  */
#define HAVE_PREAD 1
#define HAVE_PREAD_DECL 1

/* Define if you have the popen function.  */
#define HAVE_POPEN 1

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* File system ---------------------------------------------------------- */

/* Define if you have the <pwd.h> header file.  */
#define HAVE_PWD_H 1

/* Define if `fsync' exists. */
#define HAVE_FSYNC 1

/* Define if `fdatasync' exists. */
#define HAVE_FDATASYNC 1

/* Define if you have the getcwd function.  */
#define HAVE_GETCWD 1

/* Define if `mkstemp' exists. */
#define HAVE_MKSTEMP 1
#define HAVE_MKSTEMP_DECL 1

/* Define if `mkdtemp' exists. */
#define HAVE_MKDTEMP 1
/* #undef HAVE_MKDTEMP_DECL */

/* Define if `glob' exists. */
#define HAVE_GLOB 1

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* Directory iterator --------------------------------------------------- */

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if the closedir function returns void instead of int.  */
/* #undef CLOSEDIR_VOID */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* I/O Selector --------------------------------------------------------- */

/* Define if you have the <sys/devpoll.h> header file.  */
/* #undef HAVE_SYS_DEVPOLL_H */

/* Define if you have the <sys/poll.h> header file.  */
#define HAVE_SYS_POLL_H 1

/* Define if you have the <sys/select.h> header file.  */
#define HAVE_SYS_SELECT_H 1

/* Define if you have the poll function.  */
#define HAVE_POLL 1

/* Define if you have the kqueue function.  */
#define HAVE_KQUEUE 1

/* Cast for file descriptor sets for select. */
#define SELECT_ARG_TYPE fd_set*

/* Define if <fcntl.h> defines F_SETSIG.  */
/* #undef HAVE_F_SETSIG */

/* Define if <fcntl.h> defines F_SETAUXFL.  */
/* #undef HAVE_F_SETAUXFL */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* Networking ----------------------------------------------------------- */

/* Define if you have the <sys/socket.h> header file.  */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the <sys/sockio.h> header file.  */
#define HAVE_SYS_SOCKIO_H 1

/* Define if you have the <sys/un.h> header file.  */
#define HAVE_SYS_UN_H 1

/* Define if you have the <netinet/in.h> header file.  */
#define HAVE_NETINET_IN_H 1

/* Define if you have the <netdb.h> header file.  */
#define HAVE_NETDB_H 1

/* Define if you have the <arpa/inet.h> header file.  */
#define HAVE_ARPA_INET_H 1

/* Cast for argument type for data type argument of recv and friends. */
#define RECV_ARG_TYPE void *

/* The type of the last argument to socket option functions. */
#define SOCKOPT_LEN_TYPE socklen_t

/* Define to the type expected by socket functions.  */
#define SOCKET_LEN_TYPE socklen_t

/* Define if you have the sockaddr_un structure.  */
#define HAVE_SOCKADDR_UN 1

/* Define if you have the inet_aton function.  */
#define HAVE_INET_ATON 1
#define HAVE_INET_ATON_DECL 1

/* Define if gethostname is declared in unistd.h.  */
#define HAVE_GETHOSTNAME 1
#define HAVE_GETHOSTNAME_DECL 1

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* Resource information ------------------------------------------------- */

/* Define if you have the <sys/resource.h> header file.  */
#define HAVE_SYS_RESOURCE_H 1

/* Define if linker defines these symbols.  */
/* #undef HAVE_SYM_DATA_START */
/* #undef HAVE_SYM_BSS_START */
/* #undef HAVE_SYM_EDATA */
/* #undef HAVE_SYM_ETEXT */
/* #undef HAVE_SYM_END */

/* #undef HAVE_SYM_DL_ARGV */
/* #undef HAVE_SYM_ARGV */
/* #undef HAVE_SYM_ARGC */

/* Define if you have program_invocation_name.  */
/* #undef HAVE_PROGRAM_INVOCATION_NAME */

/* Define if `getrusage' exists.  */
#define HAVE_GETRUSAGE 1

/* Define if `getrlimit' exists.  */
#define HAVE_GETRLIMIT 1

/* Define if `getpagesize' exists.  */
#define HAVE_GETPAGESIZE 1

/* Define if `getdtablesize' exists.  */
/* #undef HAVE_GETDTABELSIZE */

/* Define if `getdtablehi' exists.  */
/* #undef HAVE_GETDTABLEHI */

/* Define if `ulimit ()' exists.  */
#define HAVE_ULIMIT 1

/* Define if `sysconf (_SC_OPEN_MAX)' exists.  */
#define HAVE_SC_OPEN_MAX 1

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* Time ----------------------------------------------------------------- */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define if your <sys/time.h> declares struct tm.  */
/* #undef TM_IN_SYS_TIME */

/* Define if struct tm has a member tm_gmtoff.  */
#define HAVE_TM_GMTOFF 1

/* Define if struct tm has a member tm_zone.  */
#define HAVE_TM_ZONE 1

/* Define if utime(file, NULL) sets file's timestamp to the present.  */
#define HAVE_UTIME_NULL 1

/* Define if you have the timegm function.  */
#define HAVE_TIMEGM 1

/* Define if you have the strftime function.  */
#define HAVE_STRFTIME 1

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* High-res timers ------------------------------------------------------ */

/* Define if you have <invent.h> header file (sgi irix).  */
/* #undef HAVE_INVENT_H */

/* Define if you have <sys/systemcfg.h> (aix/powerpc). */
/* #undef HAVE_SYS_SYSTEMCFG_H */

/* Define if you have <sys/processor.h> (solaris, aix/powerpc).  */
/* #undef HAVE_SYS_PROCESSOR_H */

/* Define if you have <sys/times.h> (e.g. nanosleep).  */
#define HAVE_SYS_TIMES_H 1

/* Define if you have <sys/pstat.h> (hp-ux).  */
/* #undef HAVE_SYS_PSTAT_H */

/* Define if you have <machine/inline.h> (hp-ux).  */
/* #undef HAVE_MACHINE_INLINE_H */

/* Define if you have <pmapi.h> (aix).  */
/* #undef HAVE_PMAPI_H */

/* Define if you have <procfs.h> (solaris).  */
/* #undef HAVE_PROCFS_H */

/* Define if you have <mach/mach_time.h> (darwin).  */
#define HAVE_MACH_MACH_TIME_H 1

/* Define if you have POSIX nanosleep() (unistd.h).  */
#define HAVE_NANOSLEEP 1

/* Define if you have BSD usleep() (unistd.h).  */
#define HAVE_USLEEP 1

/* Define if you have the posix times().  */
#define HAVE_TIMES 1

/* Define if you have gethrtime()/gethrvtime() (solaris).  */
/* #undef HAVE_GETHRTIME */

/* Define if you have the posix clock_gettime().  */
/* #undef HAVE_CLOCK_GETTIME */

/* Define if you have CLOCK_REALTIME for clock_gettime().  */
/* #undef HAVE_CLOCK_REALTIME */

/* Define if you have CLOCK_MONOTONIC for clock_gettime().  */
/* #undef HAVE_CLOCK_MONOTONIC */

/* Define if you have CLOCK_PROCESS_CPUTIME_ID for clock_gettime().  */
/* #undef HAVE_CLOCK_PROCESS_CPUTIME_ID */

/* Define if you have CLOCK_THREAD_CPUTIME_ID for clock_gettime().  */
/* #undef HAVE_CLOCK_THREAD_CPUTIME_ID */

/* Define if you have CLOCK_PROFILE for clock_gettime().  */
/* #undef HAVE_CLOCK_PROFILE */

/* Define if you have CLOCK_PROF for clock_gettime().  */
/* #undef HAVE_CLOCK_PROF */

/* Define if you have CLOCK_SGI_CYCLE for clock_gettime().  */
/* #undef HAVE_CLOCK_SGI_CYCLE */

/* Define if you have read_real_time in sys/time.h (aix).  */
/* #undef HAVE_READ_REAL_TIME */

/* Define if you have processor_info in <sys/processor.h> (solaris).  */
#define HAVE_PROCESSOR_INFO 1

/* Define to a GCC asm instruction to get real clock cycles.  */
#define CPU_REAL_CYCLES_ASM "rdtsc" : "=a" (low), "=d" (high)

/* Define how to get the real cycles value from the counter.  */
#define CPU_REAL_CYCLES_VALUE ((unsigned long long) low) | ((unsigned long long) high << 32)

/* Define to a GCC asm instruction to get virtual clock cycles.  */
/* #undef CPU_VIRTUAL_CYCLES_ASM */

/* Define how to get the virtual cycles value from the counter.  */
/* #undef CPU_VIRTUAL_CYCLES_VALUE */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* Stack tracing -------------------------------------------------------- */

/* Define if you have pstack (usually in /usr/proc/bin) to the full
   path of that program.  */
/* #undef PROG_PSTACK */

/* Define if you have c++filt to the full path of that program.  */
#define PROG_CXXFILT "/usr/bin/c++filt"

/* Define if you have the <excpt.h> header file.  */
/* #undef HAVE_EXCPT_H */

/* Define if you have the <exception.h> header file.  */
/* #undef HAVE_EXCEPTION_H */

/* Define if you have the <rld_interface.h> header file.  */
/* #undef HAVE_RLD_INTERFACE_H */

/* Define if you have the <pdsc.h> header file.  */
/* #undef HAVE_PDSC_H */

/* Define if you have U_STACK_TRACE in -lcl.  */
/* #undef HAVE_U_STACK_TRACE */

/* Define if you have backtrace and backtrace_symbols_fd (glibc 2.1).  */
#define HAVE_BACKTRACE_SYMBOLS_FD 1

/* Define if you have xl__trkbk in -lxlf90.  */
/* #undef HAVE_XL_TRBK */

/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
/* Dynamic linker ------------------------------------------------------- */

/* Define if `dlopen' exists. */
#define HAVE_DLOPEN 1
#define HAVE_DLOPEN_DECL 1

/* Define if `shl_load' exists. */
/* #undef HAVE_SHL_LOAD */
/* #undef HAVE_SHL_LOAD_DECL */

/* Define if `load' exists. */
/* #undef HAVE_LOAD */
/* #undef HAVE_LOAD_DECL */

/* Define if you have the <loader.h> header file.  */
/* #undef HAVE_LOADER_H */

/* Define if `ldr_next_module' exists. */
/* #undef HAVE_LDR_NEXT_MODULE */
/* #undef HAVE_LDR_NEXT_MODULE_DECL */

/* Define if you have the <link.h> header file.  */
/* #undef HAVE_LINK_H */

/* Define if `dladdr' exists. */
#define HAVE_DLADDR 1
#define HAVE_DLADDR_DECL 1

/* Define if `dlinfo' exists. */
/* #undef HAVE_DLINFO */
/* #undef HAVE_DLINFO_DECL */

/* Define if `_r_debug' exists. */
/* #undef HAVE_R_DEBUG */

/* Define if `link_map' has `l_map_start' member. */
/* #undef HAVE_LINK_MAP_L_MAP_START */

/* Define if you have the <elf.h> header file.  */
/* #undef HAVE_ELF_H */

/* Define if you have the <sgidefs.h> header file.  */
/* #undef HAVE_SGIDEFS_H */

/* Define if you have the <mach-o/dyld.h> header file.  */
#define HAVE_MACH_O_DYLD_H 1

#endif /* SEAL_PLATFORM_CONFIG_H */
