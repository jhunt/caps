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
    0000003fffffffff (38):
      CAP_CHOWN              0 (0x00000000000001)  Make arbitrary changes to file UIDs and GIDs
      CAP_DAC_OVERRIDE       1 (0x00000000000002)  Bypass file read, write, and execute permission checks.
      CAP_DAC_READ_SEARCH    2 (0x00000000000004)  Bypass file read permission checks and directory read and execute permission checks.
      CAP_FOWNER             3 (0x00000000000008)  Bypass file ownership / process owner equality permission checks.
      CAP_FSETID             4 (0x00000000000010)  Don't clear set-user-ID and set-group-ID mode bits when a file is modified
      ... (etc.) ...

You can also negate stuff, so if you have a process with a non-trivial set
of capabilities:

    →  caps 0000003fffffefff
    0000003fffffefff (37):
      CAP_CHOWN              0 (0x00000000000001)  Make arbitrary changes to file UIDs and GIDs
      CAP_DAC_OVERRIDE       1 (0x00000000000002)  Bypass file read, write, and execute permission checks.
      CAP_DAC_READ_SEARCH    2 (0x00000000000004)  Bypass file read permission checks and directory read and execute permission checks.
      CAP_FOWNER             3 (0x00000000000008)  Bypass file ownership / process owner equality permission checks.
      CAP_FSETID             4 (0x00000000000010)  Don't clear set-user-ID and set-group-ID mode bits when a file is modified
      CAP_KILL               5 (0x00000000000020)  Bypass permission checks for sending signals.
      ... (31 more) ...

Instead, it might be better to find out what _isn't_ allowed:

    →  caps -n 0000003fffffefff
    ~0000003fffffefff (1):
      CAP_NET_ADMIN         12 (0x00000000001000)  Manage network configuration and interfaces (broad scope).

Over time, I hope to flesh out the script with more explanation and detail
about the capabilities themselves, what they do, how to set them, etc.  For
now, check out `capabilities(7)`.

Requires Perl.
