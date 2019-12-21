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


Building From Source
--------------------

This repository contains a Makefile that will compile a static
executable, suitable for use in an otherwise empty Docker image.

    make build

To make the docker image locally:

    make docker

To fork and run it under your own DockerHub account:

    make IMAGE=yourusername docker push


Running Via Docker
------------------

I provide this as a small (~900k) Docker image, as `huntprod/caps`
on DockerHub.  There's nothing but this utility in the image, and
its entrypoint will (without arguments) introspect
`/proc/self/status` to get the permitted capability set.

Here's some fun stuff to try!

To get your baseline privileges / capabilities:

    →  docker run --rm huntprod/caps
    (via /proc/self/status)
    00000000a80425fb (14 capabilities):
      chown                  0 (0x00000000000001)  Make arbitrary changes to file UIDs and GIDs
      dac_override           1 (0x00000000000002)  Bypass file read, write, and execute permission checks.
      fowner                 3 (0x00000000000008)  Bypass file ownership / process owner equality permission checks.
      fsetid                 4 (0x00000000000010)  Don't clear set-user-ID and set-group-ID mode bits when a file is modified
      kill                   5 (0x00000000000020)  Bypass permission checks for sending signals.
      setgid                 6 (0x00000000000040)  Make arbitrary manipulations of process GIDs and supplementary GID list.
      setuid                 7 (0x00000000000080)  Make arbitrary manipulations of process UIDs.
      setpcap                8 (0x00000000000100)  Manage capability sets (from bounded / inherited set).
      net_bind_service      10 (0x00000000000400)  Bind a socket to Internet domain privileged ports.
      net_raw               13 (0x00000000002000)  Use RAW and PACKET sockets.
      sys_chroot            18 (0x00000000040000)  Use chroot(2) and manage kernel namespaces.
      mknod                 27 (0x00000008000000)  Create special files using mknod(2).
      audit_write           29 (0x00000020000000)  Write records to kernel auditing log.
      setfcap               31 (0x00000080000000)  Set arbitrary capabilities on a file.

To verify that your `--cap-drop` and `--cap-add` flags are
working:

    →  docker run --rm --cap-drop all --cap-add net_bind_service --cap-add ipc_lock huntprod/caps
    (via /proc/self/status)
    0000000000004400 (2 capabilities):
      net_bind_service      10 (0x00000000000400)  Bind a socket to Internet domain privileged ports.
      ipc_lock              14 (0x00000000004000)  Lock memory, via mlock(2) and friends.

Ever tried to mix `--privileged` and `--cap-drop`?  Yeah, it
doesn't work out so well:

    →  docker run --rm --privileged --cap-drop net_admin huntprod/caps
    --negate
    (via /proc/self/status)
    ~0000003fffffffff (0 capabilities):

What you probably wanted was this:

    →  docker run --rm --cap-add all --cap-drop net_admin huntprod/caps --negate
    (via /proc/self/status)
    ~0000003fffffefff (1 capability):
      net_admin             12 (0x00000000001000)  Manage network configuration and interfaces (broad scope).
