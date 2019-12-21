#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef PROC_SELF_STATUS
#  define PROC_SELF_STATUS "/proc/self/status"
#endif

#ifndef PROC_LINE_SIZE
#  define PROC_LINE_SIZE 4096
#endif

#include "caps.inc"

#define cap_name(c)    (CAPS[(c)].name)
#define cap_summary(c) (CAPS[(c)].summary)
#define cap_bit(c)     (CAPS[(c)].bit)
#define cap_flag(c)    (1llu << cap_bit(c))

#define for_each_cap(c) for (c = 0; cap_name(c) != NULL; c++)
#define has(v,c) ((v) & cap_flag(c) ? 1 : 0)

int display(int negate, const char *raw) {
	unsigned long long v = 0;
	int x = 0;
	int i, cap, count;
	int expected = negate ? 0 : 1;

	for (i = 0; raw[i]; i++) {
		v = (v << 4);
		switch (raw[i]) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			v += raw[i] - '0';
			break;

		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			v += raw[i] - 'a' + 10;
			break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			v += raw[i] - 'A' + 10;
			break;

		case 'x': case 'X':
			x++;
			if (x == 1 && v == 0) break;
		default:
			fprintf(stderr, "Unrecognized hex literal: '%s'\n", raw);
			return 2;
		}
	}

	count = 0;
	for_each_cap(cap) {
		if (has(v, cap) == expected) {
			count++;
		}
	}

	printf("%s%s (%d capabilit%s):\n", (negate ? "~" : ""), raw, count, (count == 1 ? "y" : "ies"));
	for_each_cap(cap) {
		if (has(v, cap) == expected) {
			printf("  %-20s  %2.1d (%#016llx)", cap_name(cap), cap_bit(cap), cap_flag(cap));
			if (cap_summary(cap)) {
				printf("  %s", cap_summary(cap));
			}
			printf("\n");
		}
	}
}

int main(int argc, char **argv) {
	int i, j, n, negate;

	negate = 0;
	n = 0;
	for (i = j = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			fprintf(stderr, "USAGE: %s [-n] capset [capset ...]\n", argv[0]);
			return 1;
		}

		if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--negate") == 0) {
			negate = 1;
			continue;
		}

		n++;
		if (i != j) {
			argv[j] = argv[i];
			j++;
		}
	}

	if (n == 0) {
		// pull cap set from /proc/self
		FILE *in;
		char *p, buf[PROC_LINE_SIZE];

		in = fopen(PROC_SELF_STATUS, "r");
		if (!in) {
			fprintf(stderr, "Unable to open '%s' for reading: %s (error %d)\n", PROC_SELF_STATUS, strerror(errno), errno);
			return 2;
		}

		while (fgets(buf, PROC_LINE_SIZE, in) != NULL) {
			/* 8 = length of "CapPrm:\t" */
			if (strncmp(buf, "CapPrm:\t", 8) == 0) {
				fclose(in);

				/* null-terminate the CapPrm line... */
				for (p = buf; *p != '\r' && *p != '\n'; p++); *p = '\0';

				printf("(via %s)\n", PROC_SELF_STATUS);
				return display(negate, buf+8);
			}
		}

		fprintf(stderr, "Unable to parse '%s' for CapPrm: corrupted /proc/<pid>/status file found\n", PROC_SELF_STATUS);
		return 2;
	}

	for (i = 0; i < n; i++) {
		int rc;

		rc = display(negate, argv[i+1]);
		if (rc != 0) {
			return rc;
		}
	}

	return 0;
}
