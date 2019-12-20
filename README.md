Linux Capabilities!
===================

`caps` is a utility I wrote to make sense of the Linux Capability bitflags
you get from the lovely `/proc` filesystem.

Normally, those look like this:

    →  sudo cat /proc/self/status | grep Cap
    CapInh:  0000000000000000
    CapPrm:  0000003fffffffff
    CapEff:  0000003fffffffff
    CapBnd:  0000003fffffffff
    CapAmb:  0000000000000000

While I _now_ know that `0000003fffffffff` is synonyous with root, I didn't
always, and that always annoyed me.  `caps` to the rescue!

    →  caps 0000003fffffffff
    0000003fffffffff (274877906943):
    CAP_DAC_OVERRIDE       1 (0x01)  Bypass file read, write, and execute permission checks.
    CAP_DAC_READ_SEARCH    2 (0x02)  Bypass file read permission checks and directory read and execute permission checks.
    CAP_FOWNER             3 (0x03)
    CAP_FSETID             4 (0x04)  Don't clear set-user-ID and set-group-ID mode bits when a file is modified
    CAP_KILL               5 (0x05)  Bypass permission checks for sending signals.
    CAP_SETGID             6 (0x06)  Make arbitrary manipulations of process GIDs and supplementary GID list.
    CAP_SETUID             7 (0x07)  Make arbitrary manipulations of process UIDs.
    CAP_SETPCAP            8 (0x08)
    CAP_LINUX_IMMUTABLE    9 (0x09)
    CAP_NET_BIND_SERVICE  10 (0x0a)
    CAP_NET_BROADCAST     11 (0x0b)
    CAP_NET_ADMIN         12 (0x0c)
    CAP_NET_RAW           13 (0x0d)
    CAP_IPC_LOCK          14 (0x0e)
    CAP_IPC_OWNER         15 (0x0f)
    CAP_SYS_MODULE        16 (0x10)
    CAP_SYS_RAWIO         17 (0x11)
    CAP_SYS_CHROOT        18 (0x12)
    CAP_SYS_PTRACE        19 (0x13)
    CAP_SYS_PACCT         20 (0x14)
    CAP_SYS_ADMIN         21 (0x15)
    CAP_SYS_ROOT          22 (0x16)
    CAP_SYS_NICE          23 (0x17)
    CAP_SYS_RESOURCE      24 (0x18)
    CAP_SYS_TIME          25 (0x19)
    CAP_SYS_TTY_CONFIG    26 (0x1a)
    CAP_MKNOD             27 (0x1b)
    CAP_LEASE             28 (0x1c)
    CAP_AUDIT_WRITE       29 (0x1d)
    CAP_AUDIT_CONTROL     30 (0x1e)
    CAP_SETFCAP           31 (0x1f)
    CAP_MAC_OVERRIDE      32 (0x20)
    CAP_MAC_ADMIN         33 (0x21)
    CAP_SYSLOG            34 (0x22)
    CAP_WAKE_ALARM        35 (0x23)
    CAP_BLOCK_SUSPEND     36 (0x24)
    CAP_AUDIT_READ        37 (0x25)

Over time, I hope to flesh out the script with more explanation and detail
about the capabilities themselves, what they do, how to set them, etc.  For
now, check out `capabilities(7)`.

Requires Perl.
