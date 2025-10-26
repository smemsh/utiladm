/*
 * kubectlpty
 *   wrap kubectl with pty, for use in pipeline with terminal formatting
 *
 * desc:
 *   - runs kubectl using a pty so termios will work
 *   - copies the invoking terminal termios and winsize to the slave
 *   - aids the scenario when kubectl called with its stdout attached to pipe
 *   - we use this as the kubectl bin in ~/.kube/color.yml
 *   - see kubecolor issue 279 for more details
 *
 * todo:
 *   - not sure how to package this, stuffing the exe in utiladm is suboptimal
 *   - should probably replace our ~setup/bin/gpg wrapper
 *   - this can be generalized for other purposes into some kind of ptywrap exe
 *   - avoid hardcoded binpath: use argv0, path lookup less invocation dirname
 *
 * scott@smemsh.net
 * https://github.com/smemsh/utiladm/
 * https://spdx.org/licenses/GPL-2.0
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <pty.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

extern char **environ;
static char *kubectlbin = "/bin/kubectl";

int
main(int argc, char **argv)
{
	int fd, status;
	char buf[BUFSIZ];
	struct termios termattrs;
	struct winsize winattrs;
	ssize_t len;
	pid_t pid;

	int testfd = STDIN_FILENO;

	if (tcgetattr(testfd, &termattrs) == -1) {
		perror("tcgetattr");
		goto fail;
	}
	if (ioctl(testfd, TIOCGWINSZ, &winattrs) == -1) {
		perror("ioctl");
		goto fail;
	}

	if ((pid = forkpty(&fd, NULL, &termattrs, &winattrs)) == -1) {
		perror("forkpty");
		goto fail;
	}
	if (!pid) {
		argv[0] = strrchr(kubectlbin, '/') + 1;
		execve(kubectlbin, argv, environ);
		perror("execvp");
		goto fail;
	}

	while ((len = read(fd, buf, sizeof(buf))) > 0) {
		if (write(STDOUT_FILENO, buf, len) != len) {
			perror("write");
			close(fd);
			goto fail;
		}
	}
	if (len == -1 && errno != EIO)
		perror("read");

	close(fd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		return WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		return 128 + WTERMSIG(status);

	exit(EXIT_SUCCESS);
fail:
	exit(EXIT_FAILURE);
}
