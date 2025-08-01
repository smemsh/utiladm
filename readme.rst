utiladm
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- repository for administrative scripts that accumulate while doing things.
- they typically require privileges or build or clean things ("adm").

| scott@smemsh.net
| https://github.com/smemsh/utiladm/
| https://spdx.org/licenses/GPL-2.0

.. contents::


pylinks
------------------------------------------------------------------------------

set up symlinks in $2 (default /usr/local) to installed python root $1

notes:

- aborts on attempt to clobber any regular files in target
- symlinks allowed to exist already in the target, get overwritten
- audits at finish to make sure all files represented


klogpoll
------------------------------------------------------------------------------

read kernel logs since last poll, write cleaned up lines to stdout

- reads kernel logs, as deposited by rsyslogd/syslog-ng, since last poll
- run as cron job, outputs email-escaped lines, for cron to send in emails
- uses state file to determine where we left off last poll
- properly handles truncation and rotation
- requires syslogd configured to write to KMSGFILE
- do message filtering (ie, for common false-alarms) in syslog config


pybuild
------------------------------------------------------------------------------

| builds static or dynamically linked Python + OpenSSL
| always statically links libssl/libcrypto

- script to configure and build python, makes tar.zst
- last tested with python 3.12 line, openssl 3.0 line
- has some hardcoded paths, confined to variables

arg1:

- whether to build 'static' or 'dynamic' interpreter


pydeps
------------------------------------------------------------------------------

audit a python build archive for extension module link deps

- list modules with runtime link dependencies OTHER THAN LIBC
- intended to assist in developing a Setup.local "*static*" section
- uses a temporary directory to extract all .so files
- removes tmpdir when done

arg1:

- path to python build archive (tar.zst format)


cmdline
------------------------------------------------------------------------------

displays columnated key-value rows of current kernel's command line args


lvmchk
------------------------------------------------------------------------------

- checks all lvm volumes for thin data/metadata usage
- prints volume name if exceeds threshold
- run in cron job to mail when thin pool out-of-space approaches


dfmon
------------------------------------------------------------------------------

- checks disk space on all mounted ext4 filesystems
- prints mount point if exceeds threshold
- run in cron job to mail when disks reach critical


smartchk
------------------------------------------------------------------------------

- enumerates all disks that might support smart
- checks smart health status, emit message if currently in bad state
- run in cron job to mail when disk is deemed bad by its own smart counters
