#include <sys/cdefs.h>

void illegal __P((int));
void mul32smem __P((int));
void mul32sreg __P((int));

void mul64sreg __P((int));
void mul64ureg __P((int));
void mul64smem __P((int));
void mul64umem __P((int));

void div64umem __P((int));
void div64smem __P((int));
void div64ureg __P((int));
void div64sreg __P((int));
