/*
 * File:		cpuid.h
 * Description:	Interface with functionality offered by the cpuid instruction
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

/*
enum CPUIDFeatureSet {	// EAX=7, ECX=0, EBX set
	CPUID_		= 1 << 0, 
	CPUID_		= 1 << 1,
	CPUID_		= 1 << 2,
	CPUID_		= 1 << 3,
	CPUID_		= 1 << 4,
	CPUID_		= 1 << 5,
	CPUID_		= 1 << 6,
	CPUID_		= 1 << 7,
	CPUID_		= 1 << 8,
	CPUID_		= 1 << 9,
	CPUID_		= 1 << 10,
	CPUID_		= 1 << 11,
	CPUID_		= 1 << 12,
	CPUID_		= 1 << 13, 
	CPUID_		= 1 << 14, 
	CPUID_		= 1 << 15, 
	CPUID_		= 1 << 16, 
	CPUID_		= 1 << 17, 
	CPUID_		= 1 << 18, 
	CPUID_		= 1 << 19, 
	CPUID_		= 1 << 20, 
	CPUID_		= 1 << 21, 
	CPUID_		= 1 << 22, 
	CPUID_		= 1 << 23, 
	CPUID_		= 1 << 24, 
	CPUID_		= 1 << 25, 
	CPUID_		= 1 << 26, 
	CPUID_		= 1 << 27, 
	CPUID_		= 1 << 28,
	CPUID_		= 1 << 29,
	CPUID_		= 1 << 30,
	CPUID_		= 1 << 31
};
*/


enum CPUIDFeatureSet1 {	// EAX=1, EDX set
	CPUID_FPU	= 1 << 0,
	CPUID_VME	= 1 << 1,
	CPUID_DE	= 1 << 2,
	CPUID_PSE	= 1 << 3,
	CPUID_TSC	= 1 << 4,
	CPUID_MSR	= 1 << 5,
	CPUID_PAE	= 1 << 6,
	CPUID_MCE	= 1 << 7,
	CPUID_CX8	= 1 << 8,
	CPUID_APIC	= 1 << 9,
	CPUID_SEP	= 1 << 11, 
	CPUID_MTRR	= 1 << 12, 
	CPUID_PGE	= 1 << 13, 
	CPUID_MCA	= 1 << 14, 
	CPUID_CMOV	= 1 << 15, 
	CPUID_PAT	= 1 << 16, 
	CPUID_PSE36	= 1 << 17, 
	CPUID_PSN	= 1 << 18, 
	CPUID_CLF	= 1 << 19, 
	CPUID_DTES	= 1 << 21, 
	CPUID_ACPI	= 1 << 22, 
	CPUID_MMX	= 1 << 23, 
	CPUID_FXSR	= 1 << 24, 
	CPUID_SSE	= 1 << 25, 
	CPUID_SSE2	= 1 << 26, 
	CPUID_SS	= 1 << 27, 
	CPUID_HTT	= 1 << 28, 
	CPUID_TM1	= 1 << 29, 
	CPUID_IA64	= 1 << 30,
	CPUID_PBE	= 1 << 31
};

enum CPUIDFeatureSet2 {	// EAX=1, ECX set
	CPUID_SSE3		= 1 << 0, 
	CPUID_PCLMUL	= 1 << 1,
	CPUID_DTES64	= 1 << 2,
	CPUID_MONITOR	= 1 << 3,
	CPUID_DS_CPL	= 1 << 4,
	CPUID_VMX		= 1 << 5,
	CPUID_SMX		= 1 << 6,
	CPUID_EST		= 1 << 7,
	CPUID_TM2		= 1 << 8,
	CPUID_SSSE3		= 1 << 9,
	CPUID_CID		= 1 << 10,
	CPUID_FMA		= 1 << 12,
	CPUID_CX16		= 1 << 13, 
	CPUID_ETPRD		= 1 << 14, 
	CPUID_PDCM		= 1 << 15, 
	CPUID_PCID		= 1 << 17, 
	CPUID_DCA		= 1 << 18, 
	CPUID_SSE4_1	= 1 << 19, 
	CPUID_SSE4_2	= 1 << 20, 
	CPUID_X2APIC	= 1 << 21, 
	CPUID_MOVBE		= 1 << 22, 
	CPUID_POPCNT	= 1 << 23, 
	CPUID_AES		= 1 << 25, 
	CPUID_XSAVE		= 1 << 26, 
	CPUID_OSXSAVE	= 1 << 27, 
	CPUID_AVX		= 1 << 28,
	CPUID_F16C		= 1 << 29,
	CPUID_RDRAND	= 1 << 30,
	CPUID_HYPERV	= 1 << 31
};

enum CPUIDFeatureSet3 {	// EAX=7, ECX=0, EBX set
	CPUID_FSGSBASE		= 1 << 0, 
	CPUID_SGX			= 1 << 2,
	CPUID_BMI1			= 1 << 3,
	CPUID_HLE			= 1 << 4,
	CPUID_AVX2			= 1 << 5,
	CPUID_SMEP			= 1 << 7,
	CPUID_BMI2			= 1 << 8,
	CPUID_ERMS			= 1 << 9,
	CPUID_INVPCID		= 1 << 10,
	CPUID_RTM			= 1 << 11,
	CPUID_PQM			= 1 << 12, 
	CPUID_MPX			= 1 << 14, 
	CPUID_PQE			= 1 << 15, 
	CPUID_AVX512_F		= 1 << 16, 
	CPUID_AVX512_DQ		= 1 << 17, 
	CPUID_RDSEED		= 1 << 18, 
	CPUID_ADX			= 1 << 19, 
	CPUID_SMAP			= 1 << 20, 
	CPUID_AVX512_IFMA	= 1 << 21, 
	CPUID_PCOMMIT		= 1 << 22, 
	CPUID_CLFLUSHOPT	= 1 << 23, 
	CPUID_CLWB			= 1 << 24, 
	CPUID_INTEL_PT		= 1 << 25,
	CPUID_AVX512_PF		= 1 << 26, 
	CPUID_AVX512_ER		= 1 << 27, 
	CPUID_AVX512_CD		= 1 << 28,
	CPUID_SHA			= 1 << 29,
	CPUID_AVX512_BW		= 1 << 30,
	CPUID_AVX512_VL		= 1 << 31
};

enum CPUIDFeatureSet4 {	// EAX=7, ECX=0, ECX set
	CPUID_PREFETCHWT1		= 1 << 0, 
	CPUID_AVX512_VBMI		= 1 << 1,
	CPUID_UMIP				= 1 << 2,
	CPUID_PKU				= 1 << 3,
	CPUID_OSPKE				= 1 << 4,
	CPUID_WAITPKG			= 1 << 5,
	CPUID_AVX512_VBMI2		= 1 << 6,
	CPUID_CET_SS			= 1 << 7,
	CPUID_GFNI				= 1 << 8,
	CPUID_VAES				= 1 << 9,
	CPUID_VPCLMULQDQ		= 1 << 10,
	CPUID_AVX512_VNNI		= 1 << 11,
	CPUID_AVX512_BITALG		= 1 << 12,
	CPUID_AVX512_VPOPCNTDQ	= 1 << 14, 
	CPUID_RDPID				= 1 << 22, 
	CPUID_CLDEMOTE			= 1 << 25, 
	CPUID_MOVDIRI			= 1 << 27, 
	CPUID_MOVDIR64B			= 1 << 28,
	CPUID_ENQCMD			= 1 << 29,
	CPUID_SGX_LC			= 1 << 30,
	CPUID_PKS				= 1 << 31
};

enum CPUIDFeatureSet5 {	// EAX=7, ECX=0, EDX set
	CPUID_AVX512_4VNNIW			= 1 << 2,
	CPUID_AVX512_4FMAPS			= 1 << 3,
	CPUID_FSRM					= 1 << 4,
	CPUID_AVX512_VP2INTERSECT	= 1 << 8,
	CPUID_SRBDS_CTRL			= 1 << 9,
	CPUID_MD_CLEAR				= 1 << 10,
	CPUID_TSX_FORCE_ABORT		= 1 << 13, 
	CPUID_SERIALIZE				= 1 << 14, 
	CPUID_HYBRID				= 1 << 15, 
	CPUID_TSXLDTRK				= 1 << 16, 
	CPUID_PCONFIG				= 1 << 18, 
	CPUID_LBR					= 1 << 19, 
	CPUID_CET_IBT				= 1 << 20, 
	CPUID_AMX_BF16				= 1 << 22, 
	CPUID_AMX_TILE				= 1 << 24, 
	CPUID_AMX_INT8				= 1 << 25, 
	CPUID_IBRS_IBPB 			= 1 << 26, 
	CPUID_STIBP 				= 1 << 27, 
	CPUID_L1D_FLUSH				= 1 << 28,
	CPUID_SSBD					= 1 << 31
};

enum CPUIDFeatureSet6 {	// EAX=7, ECX=1, EAX set
	CPUID_AVX512_BF16	= 1 << 5, 
};

struct CPUIDFeatures {
	enum CPUIDFeatureSet1	set1;
	enum CPUIDFeatureSet2	set2;
	enum CPUIDFeatureSet3	set3;
	enum CPUIDFeatureSet4	set4;
	enum CPUIDFeatureSet5	set5;
	enum CPUIDFeatureSet6	set6;
};


// TODO
//extern bool cpuid_supported();

// Write the cpu vendor to [vendor_str] and the cpu brand to [brand_str]
// [vendor_str] must be 48 bytes and [brand_str] must be 13
extern void cpuid_get_str(char* vendor_str, char* brand_str);

// Write the cpu features supported to [features]
extern void cpuid_get_features(struct CPUIDFeatures* features);