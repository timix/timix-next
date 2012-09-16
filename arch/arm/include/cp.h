//
//  Copyright(C) 2012, Tim Detwiler <timdetwiler@gmail.com>
//
//  This file is part of Timix.
//
//  Timix is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Timix is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Timix.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TIMIX_CP_H_
#define TIMIX_CP_H_

//
// Since the MRC/MCR instructions have special formatting, we use some macro
// magic with inline assembly to get this into some C-friendly form.
//
#define cp_read_raw(var, cp, op1, crn, crm, op2) \
  asm volatile("mrc p" #cp ", "\
                       #op1 ", %0, c"\
                       #crn ", c"\
                       #crm ", "\
                       #op2 ";"\
                       : "=r"(var) : )

#define cp_write_raw(var, cp, op1, crn, crm, op2) \
  asm volatile("mcr p" #cp ", "\
                       #op1 ", %0, c"\
                       #crn ", c"\
                       #crm ", "\
                       #op2 ";"\
                       : : "r"(var) )

//
// Oh so CPP is going to be a little bitch and not expand my nice macros? We'll
// see about that...
//
#define cp_write(var, ...)  cp_write_raw(var, __VA_ARGS__)
#define cp_read(var, ...)  cp_read_raw(var, __VA_ARGS__)

//
// Macro-names for registers
//
// Use cp_read(var, MIDR);
//

//
// cr0
//
#define MIDR            15,0,0,0,0
#define CTR             15,0,0,0,1
#define TCMTR           15,0,0,0,2
#define TLBTR           15,0,0,0,3
#define MPIDR           15,0,0,0,5
#define REVIDR          15,0,0,0,6
#define ID_PFR0         15,0,0,1,0
#define ID_PFR1         15,0,0,1,1
#define ID_DFR0         15,0,0,1,2
#define ID_AFR0         15,0,0,1,3
#define ID_MMFR0        15,0,0,1,4
#define ID_MMFR1        15,0,0,1,5
#define ID_MMFR2        15,0,0,1,6
#define ID_MMFR3        15,0,0,1,7
#define ID_ISAR0        15,0,0,2,0
#define ID_ISAR1        15,0,0,2,1
#define ID_ISAR2        15,0,0,2,2
#define ID_ISAR3        15,0,0,2,3
#define ID_ISAR4        15,0,0,2,4
#define ID_ISAR5        15,0,0,2,5
#define CCSIDR          15,1,0,0,0
#define CLIDR           15,1,0,0,1
#define AIDR            15,1,0,0,7
#define CSSELR          15,2,0,0,0
#define VPIDR           15,4,0,0,0
#define VMPIDR          15,4,0,0,5

//
// cr1
//
#define SCTLR           15,0,1,0,0
#define ACTLR           15,0,1,0,1
#define CPACR           15,0,1,0,2
#define SCR             15,0,1,1,0
#define SDER            15,0,1,1,1
#define NSACR           15,0,1,1,2
#define HSCTLR          15,4,1,0,0
#define HACTLR          15,4,1,0,1
#define HCR             15,4,1,1,0
#define HDCR            15,4,1,1,1
#define HCPTR           15,4,1,1,2
#define HSTR            15,4,1,1,3
#define HACR            15,4,1,1,7

//
// cr2
//
#define TTBR0           15,0,2,0,0
#define TTBR1           15,0,2,0,1
#define TTBCR           15,0,2,0,2
#define HTCR            15,4,2,0,2
#define VTCR            15,4,2,1,2

//
// cr3
//
#define DACR            15,0,3,0,0

//
// cr5
//
#define DFSR            15,0,5,0,0
#define IFSR            15,0,5,0,1
#define ADFSR           15,0,5,1,0
#define AIFSR           15,0,5,1,1
#define HADFSR          15,4,5,1,0
#define HAIFSR          15,4,5,1,1
#define HSR             15,4,5,2,0

//
// cr6
//
#define DFAR            15,0,6,0,0
#define IFAR            15,0,6,0,2
#define HDFAR           15,4,6,0,0
#define HIFAR           15,4,6,0,2
#define HPFAR           15,4,6,0,4

//
// cr7
//
#define ICIALLUIS       15,0,7,1,0
#define BPIALLIS        15,0,7,1,6
#define PAR             15,0,7,4,0
#define ICIALLU         15,0,7,5,0
#define ICIMVAU         15,0,7,5,1
#define CP15ISB         15,0,7,5,4
#define BPIALL          15,0,7,5,6
#define BPIMVA          15,0,7,5,7
#define DCIMVAC         15,0,7,6,1
#define DCISW           15,0,7,6,2
#define ATS1CPR         15,0,7,8,0
#define ATS1CPW         15,0,7,8,1
#define ATS1CUR         15,0,7,8,2
#define ATS1CUW         15,0,7,8,3
#define ATS12NSOPR      15,0,7,8,4
#define ATS12NSOPW      15,0,7,8,5
#define ATS12NSOUR      15,0,7,8,6
#define ATS12NSOUW      15,0,7,8,7
#define DCCMVAC         15,0,7,10,1
#define DCCSW           15,0,7,10,2
#define CP15DSB         15,0,7,10,4
#define CP15DMB         15,0,7,10,5
#define DCCMVAU         15,0,7,11,1
#define DCCIMVAC        15,0,7,14,1
#define DCCISW          15,0,7,14,2
#define ATS1HR          15,4,7,8,0
#define ATS1HW          15,4,7,8,1

//
// cr8
//
#define TLBIALLIS       15,0,8,3,0
#define TLBIMVAIS       15,0,8,3,1
#define TLBIASIDIS      15,0,8,3,2
#define TLBIMVAAIS      15,0,8,3,3
#define ITLBIALL        15,0,8,5,0
#define ITLBIMVA        15,0,8,5,1
#define ITLBIASID       15,0,8,5,2
#define DTLBIALL        15,0,8,6,0
#define DTLBIMVA        15,0,8,6,1
#define DTLBIASID       15,0,8,6,2
#define TLBIALL         15,0,8,7,0
#define TLBIMVA         15,0,8,7,1
#define TLBIASID        15,0,8,7,2
#define TLBIMVAA        15,0,8,7,3
#define TLBIALLHIS      15,4,8,3,0
#define TLBIMVAHIS      15,4,8,3,1
#define TLBIALLNSNHIS   15,4,8,3,4
#define TLBIALLH        15,4,8,7,0
#define TLBIMVAH        15,4,8,7,1
#define TLBIALLNSNH     15,4,8,7,4

//
// cr9
//
#define PMCR            15,0,9,12,0
#define PMCNTENSET      15,0,9,12,1
#define PMCNTENCLR      15,0,9,12,2
#define PMOVSR          15,0,9,12,3
#define PMSWINC         15,0,9,12,4
#define PMSELR          15,0,9,12,5
#define PMCEID0         15,0,9,12,6
#define PMCEID1         15,0,9,12,7
#define PMCCNTR         15,0,9,13,0
#define PMXEVTYPER      15,0,9,13,1
#define PMXEVCNTR       15,0,9,13,2
#define PMUSERNR        15,0,9,14,0
#define PMINTENSET      15,0,9,14,1
#define PMINTENCLR      15,0,9,14,2
#define PMOVSSET        15,0,9,14,3
#define L2CTLR          15,1,9,0,2
#define L2ECTLR         15,1,9,0,3

//
// cr10
//
#define PRRR            15,0,10,2,0
#define MARI0           15,0,10,2,0
#define NMRR            15,0,10,2,1
#define MARI1           15,0,10,2,1
#define AMARI0          15,0,10,3,0
#define AMARI1          15,0,10,3,0
#define HMAIR0          15,4,10,2,0
#define HMAIR1          15,4,10,2,1
#define HAMAIR0         15,4,10,3,0
#define HAMAIR1         15,4,10,3,1

//
// cr12
//
#define VBAR            15,0,12,0,0
#define MVBAR           15,0,12,0,1
#define ISR             15,0,12,1,0
#define HVBAR           15,4,12,0,0

//
// cr13
//
#define FCSEIDR         15,0,13,0,0
#define CONTEXTIDR      15,0,13,0,1
#define TPIDRURW        15,0,13,0,2
#define TPIDRURO        15,0,13,0,3
#define TPIDRPRW        15,0,13,0,4
#define HTPIDR          15,4,13,0,2

//
// cr14 - Generic Timer
//
#define CNTFRQ          15,0,14,0,0
// CNTPCT - 64 bit
#define CNTKCTL         15,0,14,1,0
#define CNTP_TVAL       15,0,14,2,0
#define CNTP_CTL        15,0,14,2,1
#define CNTV_TVAL       15,0,14,3,0
#define CNTV_CTL        15,0,14,3,1
// CNTVCT - 64 bit
// CNTP_CVAL - 64 bit
// CNTV_CVAL - 64 bit
// CNTVOFF - 64 bit
#define CNTHCTL         15,4,14,1,0
#define CNTHP_TVAL      15,4,14,2,0
#define CNTHP_CTL       15,4,14,2,1
// CNTHP_CVAL - 64 bit

//
// cr15
//
#define IL1Data0        15,0,15,0,0
#define IL1Data1        15,0,15,0,1
#define IL1Data2        15,0,15,0,2
#define DL1Data0        15,0,15,1,0
#define DL1Data1        15,0,15,1,1
#define DL1Data2        15,0,15,1,2
#define DL1Data3        15,0,15,1,3
#define RAMINDEX        15,0,15,4,0
#define L2ACTLR         15,1,15,0,0
#define L2PFR           15,1,15,0,3
#define ACTLR2          15,1,15,0,4
#define CBAR            15,4,15,0,0

//
// CP14 - Debug Registers
//
#define DBGDIDR         14,0,0,0,0
#define DBGWFAR         14,0,0,6,0
#define DBGVCR          14,0,0,7,0
#define DBGDTRRX_ext    14,0,0,0,2
#define DBGDSCR_ext     14,0,0,2,2
#define DBGDTRTX_ext    14,0,0,3,2
#define DBGBVR0         14,0,0,0,4
#define DBGBVR1         14,0,0,1,4
#define DBGBVR2         14,0,0,2,4
#define DBGBVR3         14,0,0,3,4
#define DBGBVR4         14,0,0,4,4
#define DBGBVR5         14,0,0,5,4
#define DBGBVR6         14,0,0,6,4
#define DBGBVR7         14,0,0,7,4
#define DBGBVR8         14,0,0,8,4
#define DBGBVR9         14,0,0,9,4
#define DBGBVR10        14,0,0,10,4
#define DBGBVR11        14,0,0,11,4
#define DBGBVR12        14,0,0,12,4
#define DBGBVR13        14,0,0,13,4
#define DBGBVR14        14,0,0,14,4
#define DBGBVR15        14,0,0,15,4
#define DBGBCR0         14,0,0,0,5
#define DBGBCR1         14,0,0,1,5
#define DBGBCR2         14,0,0,2,5
#define DBGBCR3         14,0,0,3,5
#define DBGBCR4         14,0,0,4,5
#define DBGBCR5         14,0,0,5,5
#define DBGBCR6         14,0,0,6,5
#define DBGBCR7         14,0,0,7,5
#define DBGBCR8         14,0,0,8,5
#define DBGBCR9         14,0,0,9,5
#define DBGBCR10        14,0,0,10,5
#define DBGBCR11        14,0,0,11,5
#define DBGBCR12        14,0,0,12,5
#define DBGBCR13        14,0,0,13,5
#define DBGBCR14        14,0,0,14,5
#define DBGBCR15        14,0,0,15,5
#define DBGWVR0         14,0,0,0,5
#define DBGWVR1         14,0,0,1,5
#define DBGWVR2         14,0,0,2,5
#define DBGWVR3         14,0,0,3,5
#define DBGWVR4         14,0,0,4,5
#define DBGWVR5         14,0,0,5,5
#define DBGWVR6         14,0,0,6,5
#define DBGWVR7         14,0,0,7,5
#define DBGWVR8         14,0,0,8,5
#define DBGWVR9         14,0,0,9,5
#define DBGWVR10        14,0,0,10,5
#define DBGWVR11        14,0,0,11,5
#define DBGWVR12        14,0,0,12,5
#define DBGWVR13        14,0,0,13,5
#define DBGWVR14        14,0,0,14,5
#define DBGWVR15        14,0,0,15,5
#define DBGWCR0         14,0,0,0,6
#define DBGWCR1         14,0,0,1,6
#define DBGWCR2         14,0,0,2,6
#define DBGWCR3         14,0,0,3,6
#define DBGWCR4         14,0,0,4,6
#define DBGWCR5         14,0,0,5,6
#define DBGWCR6         14,0,0,6,6
#define DBGWCR7         14,0,0,7,6
#define DBGWCR8         14,0,0,8,6
#define DBGWCR9         14,0,0,9,6
#define DBGWCR10        14,0,0,10,6
#define DBGWCR11        14,0,0,11,6
#define DBGWCR12        14,0,0,12,6
#define DBGWCR13        14,0,0,13,6
#define DBGWCR14        14,0,0,14,6
#define DBGWCR15        14,0,0,15,6
#define DBGXVR0         14,0,1,4,1
#define DBGXVR1         14,0,1,5,1
#define DBGOSLAR        14,0,1,0,4
#define DBGOSLSR        14,0,1,1,4
#define DBGPRCR         14,0,1,4,4,
#define DBGCLAIMSET     14,0,7,8,6
#define DBGCLAIMCLR     14,0,7,9,6
#define DBGAUTHSTATUS   14,0,7,14,6
#define DBGDEVID2       14,0,7,0,7
#define DBGDEVID1       14,0,7,1,7
#define DBGDEVID        14,0,7,2,7
#define DBGDSCR_int     14,0,0,1,0
#define DBGDTRRX_int    14,0,0,5,0
#define DGBDRAR         14,0,1,0,0
#define DGBOSDLR        14,0,1,3,4
#define DGBDSAR         14,0,2,0,0

//
// TODO: CP10/CP11
//

#endif // TIMIX_CP_H_
