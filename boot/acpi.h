/*
 * acpi.h
 * Joel Wolfrath, 2014
 *
 * Functions and defs for find and parsing ACPI tables
 */

#ifndef ACPI_H
#define ACPI_H

#include "types.h"


/* Signatures for ACPI descriptor tables */

#define RSDP_SIG	("RSD PTR ")	/* Root System Description Pointer */

#define MADT_TBL_SIG	("APIC")	/* Multiple APIC Description TBL */
#define BERT_TBL_SIG	("BERT")	/* Boot Error Record TBL */
#define CPEP_TBL_SIG	("CPEP")	/* Corrected Platform Error Polling TBL */
#define DSDT_TBL_SIG	("DSDT")	/* Differentiated System Description TBL */
#define ECDT_TBL_SIG	("ECDT")	/* Embedded Controller Boot Resourses TBL */
#define EINJ_TBL_SIG	("EINJ")	/* Error Injection TBL */
#define ERST_TBL_SIG	("ERST")	/* Error Record Serialization TBL */
#define FACP_TBL_SIG	("FACP")	/* Fixed ACPI Description TBL */
#define FACS_TBL_SIG	("FACS")	/* Firmware ACPPI Control Structure */
#define HEST_TBL_SIG	("HEST")	/* Hardware Error Source TBL */
#define MSCT_TBL_SIG	("MSCT")	/* Maximum System Characteristics TBL */
#define MPST_TBL_SIG	("MPST")	/* Memory Power State TBL */
#define OEMX_TBL_SIG	("OEMx")	/* OEM Specific Information TBLS */
#define PMTT_TBL_SIG	("PMTT")	/* Platform Memory Topology TBL */
#define PSDT_TBL_SIG	("PSDT")	/* Persistent System Description TBL */
#define RASF_TBL_SIG	("RASF")	/* ACPI RAS Feature TBL */
#define RSDT_TBL_SIG	("RSDT")	/* Root System Description TBL */
#define SBST_TBL_SIG	("SBST")	/* Smart Battery Specification TBL */
#define SLIT_TBL_SIG	("SLIT")	/* System Locality System Info TBL */
#define SRAT_TBL_SIG	("SRAT")	/* System Resource Affinity TBL */
#define SSDT_TBL_SIG	("SSDT")	/* Secondary System Description TBL */
#define XSDT_TBL_SIG	("XSDT")	/* Extended System Descrption TBL */

typedef struct {

	char	signature[8]; /* RSDP_SIG */ /* RSDT_TBL_SIG */
	uint8	checksum;
	char	oemid[6];
	uint8	rev;
	uint32	rsdt_addr;

} RootSystemDescPointer;

typedef struct {

	char	signature[4];
	uint32	length;
	uint8	rev;
	uint8	checksum;
	char	oemid[6];
	char	oemtbl[8];
	uint32	oemrev;
	uint32	creatorid;
	uint32	crtrev;

} RootSystemDescTable;

void init_acpi();

#endif
