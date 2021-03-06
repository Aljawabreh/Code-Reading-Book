/*	$OpenBSD: db_machdep.h,v 1.2 1997/03/21 00:48:48 niklas Exp $	*/
/*	$NetBSD: db_machdep.h,v 1.1 1997/10/14 06:48:14 sakamoto Exp $	*/

/* 
 * Mach Operating System
 * Copyright (c) 1992 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon 
 * the rights to redistribute these changes.
 */

/*
 * Machine-dependent defines for new kernel debugger.
 */
#ifndef	_PPC_DB_MACHDEP_H_
#define	_PPC_DB_MACHDEP_H_

#include <vm/vm_prot.h>
#include <vm/vm_param.h>
#include <vm/vm_inherit.h>
#include <machine/trap.h>

#define	DB_ELF_SYMBOLS
#define	DB_ELFSIZE	32

typedef	vm_offset_t	db_addr_t;	/* address - unsigned */
typedef	int		db_expr_t;	/* expression - signed */
struct powerpc_saved_state {
	u_int32_t	r0;		/* data registers */
	u_int32_t	r1;
	u_int32_t	r2;
	u_int32_t	r3;
	u_int32_t	r4;
	u_int32_t	r5;
	u_int32_t	r6;
	u_int32_t	r7;
	u_int32_t	r8;
	u_int32_t	r9;
	u_int32_t	r10;
	u_int32_t	r11;
	u_int32_t	r12;
	u_int32_t	r13;
	u_int32_t	r14;
	u_int32_t	r15;
	u_int32_t	r16;
	u_int32_t	r17;
	u_int32_t	r18;
	u_int32_t	r19;
	u_int32_t	r20;
	u_int32_t	r21;
	u_int32_t	r22;
	u_int32_t	r23;
	u_int32_t	r24;
	u_int32_t	r25;
	u_int32_t	r26;
	u_int32_t	r27;
	u_int32_t	r28;
	u_int32_t	r29;
	u_int32_t	r30;
	u_int32_t	r31;
	u_int32_t	r32;
	u_int32_t	iar;
	u_int32_t	msr;
};
typedef struct powerpc_saved_state db_regs_t;
db_regs_t	ddb_regs;		/* register state */
#define DDB_REGS	(&ddb_regs)

#define	PC_REGS(regs)	((db_addr_t)(regs)->iar)

#define	BKPT_INST	0x7C810808	/* breakpoint instruction */

#define	BKPT_SIZE	(4)		/* size of breakpoint inst */
#define	BKPT_SET(inst)	(BKPT_INST)

#define	FIXUP_PC_AFTER_BREAK(regs)	((regs)->iar -= 4)

#define SR_SINGLESTEP	0x400
#define	db_clear_single_step(regs)	((regs)->msr &= ~SR_SINGLESTEP)
#define	db_set_single_step(regs)	((regs)->msr |=  SR_SINGLESTEP)

#define T_BREAKPOINT	0xffff
#define	IS_BREAKPOINT_TRAP(type, code)	((type) == T_BREAKPOINT)

#define T_WATCHPOINT	0xeeee
#ifdef T_WATCHPOINT
#define	IS_WATCHPOINT_TRAP(type, code)	((type) == T_WATCHPOINT)
#else
#define	IS_WATCHPOINT_TRAP(type, code)	0
#endif

#define	M_RTS		0xfc0007fe
#define I_RTS		0x4c000020
#define M_BC		0xfc000000
#define I_BC		0x40000000
#define M_B		0xfc000000
#define I_B		0x50000000
#define	M_RFI		0xfc0007fe
#define	I_RFI		0x4c000064

#define	inst_trap_return(ins)	(((ins)&M_RFI) == I_RFI)
#define	inst_return(ins)	(((ins)&M_RTS) == I_RTS)
#define	inst_call(ins)		(((ins)&M_BC ) == I_BC  || \
				 ((ins)&M_B  ) == I_B )
#define inst_load(ins)		0
#define inst_store(ins)		0

#ifdef _KERNEL

void	kdb_kintr __P((void *));
int	kdb_trap __P((int, void *));

#endif /* _KERNEL */

#endif	/* _PPC_DB_MACHDEP_H_ */
