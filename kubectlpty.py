#!/usr/bin/env python3
#
# kubectlpty
#   python alternative to kubectlpty.c, see that file for details
#
# scott@smemsh.net
# https://github.com/smemsh/utiladm/
# https://spdx.org/licenses/GPL-2.0
#

import termios, sys, os, pty
from shutil import which
from os.path import basename

exepath = which('kubectl')
spawnv  = [basename(exepath)] + sys.argv[1:]
stdinfd = sys.stdin.fileno()
winsize = termios.tcgetwinsize(stdinfd)
tattrs  = termios.tcgetattr(stdinfd)

if not os.isatty(sys.stdin.fileno()):
    os.execv(exepath, spawnv)

pid, masterfd = pty.fork()
if pid == pty.CHILD:
    termios.tcsetwinsize(stdinfd, winsize)
    termios.tcsetattr(stdinfd, termios.TCSANOW, tattrs)
    os.execv(exepath, spawnv)

pty._copy(masterfd, pty._read, pty._read)
os.close(masterfd)
os.waitpid(pid, 0)[1]
