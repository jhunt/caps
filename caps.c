#include <stdio.h>
#include <string.h>
#include "caps.inc"

#define cap_name(c)    (CAPS[(c)].name)
#define cap_summary(c) (CAPS[(c)].summary)
#define cap_bit(c)     (CAPS[(c)].bit)
#define cap_flag(c)    (1llu << cap_bit(c))

#define for_each_cap(c) for (c = 0; cap_name(c) != NULL; c++)
#define has(v,c) ((v) & cap_flag(c))

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
		}

		n++;
		if (i != j) {
			argv[j] = argv[i];
			j++;
		}
	}

	for (i = 0; i < n; i++) {
		unsigned long long v = 0;
		char *raw = argv[i+1];
		int x = 0;
		int cap, count;

		for (j = 0; raw[j]; j++) {
			v = (v << 4);
			switch (raw[j]) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				v += raw[j] - '0';
				break;

			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
				v += raw[j] - 'a' + 10;
				break;

			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
				v += raw[j] - 'A' + 10;
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
			if (has(v, cap)) {
				count++;
			}
		}

		printf("%s%s (%d capabilit%s):\n", (negate ? "~" : ""), raw, count, (count == 1 ? "y" : "ies"));
		for_each_cap(cap) {
			if (has(v, cap)) {
				printf("  %-20s  %2.1d (%#016llx)", cap_name(cap), cap_bit(cap), cap_flag(cap));
				if (cap_summary(cap)) {
					printf("  %s", cap_summary(cap));
				}
				printf("\n");
			}
		}
	}

	return 0;
}
