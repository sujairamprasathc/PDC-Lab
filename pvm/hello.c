static char rcsid[] =
	"$Id: hello.c,v 1.3 2009/01/23 01:12:48 pvmsrc Exp $";

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include "pvm3.h"

main()
{
	int cc, tid;
	char buf[100];

	printf("i'm t%x\n", pvm_mytid());

	cc = pvm_spawn("/home/suprasath/examples/hello_other", (char**)0, 0, "", 1, &tid);

	if (cc == 1) {
		cc = pvm_recv(-1, -1);
		pvm_bufinfo(cc, (int*)0, (int*)0, &tid);
		pvm_upkstr(buf);
		printf("from t%x: %s\n", tid, buf);

	} else
		printf("can't start hello_other\n");

	pvm_exit();
	exit(0);
}


