
#include "twincode.h"

bool cond_1 (UINT64 ebp_0, UINT64 esp_0) {
	return (((((esp_0 + UINT128 (0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffc)) & 0xffffffff) + UINT128 (0xffffffff, 0xffffffff, 0xffffffff, 0xffdd33e8)) & 0xffffffffffffffff) /*0x0*/ == 0)
		&& (((ebp_0 + UINT128 (0xffffffff, 0xffffffff, 0xffffffff, 0xffdd33b8)) & 0xffffffffffffffff) /*0x0*/ == 0);
}

int main (int argc, char *argv[]) {
	struct RegistersSet regs;
	SAVE_REGISTERS (regs);
	const UINT32 eax_0 = regs.eax, ebx_0 = regs.ebx, ecx_0 = regs.ecx, edx_0 = regs.edx, edi_0 = regs.edi, esi_0 = regs.esi, esp_0 = regs.esp, ebp_0 = regs.ebp;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7);
	*((UINT32 *) 0x402008) = 0x403060;
	*((UINT32 *) 0x40200c) = 0x403080;
	*((UINT32 *) 0x402010) = 0x4030a0;
	*((UINT32 *) 0x402014) = 0x4030c0;
	*((UINT32 *) 0x402018) = 0x4030e0;
	*((UINT32 *) 0x40201c) = 0x403104;
	*((UINT32 *) 0x402020) = 0x403124;
	*((UINT32 *) 0x402024) = 0x403148;
	*((UINT32 *) 0x402028) = 0x40316c;
	*((UINT32 *) 0x40202c) = 0x403190;
	*((UINT32 *) 0x4061a4) = 0x0;
	*((UINT64 *) 0x4061a8) = 0x0;
	const UINT32 m22cc70_0_32 = *((UINT32 *) 0x22cc70);
	const UINT8 m22cca5_0_8 = *((UINT8 *) 0x22cca5);
	const UINT8 m22cca6_0_8 = *((UINT8 *) 0x22cca6);
	const UINT8 m22cca7_0_8 = *((UINT8 *) 0x22cca7);
	const UINT32 m402008_0_32 = *((UINT32 *) 0x402008);
	const UINT32 m40200c_0_32 = *((UINT32 *) 0x40200c);
	const UINT32 m402010_0_32 = *((UINT32 *) 0x402010);
	const UINT32 m402014_0_32 = *((UINT32 *) 0x402014);
	const UINT32 m402018_0_32 = *((UINT32 *) 0x402018);
	const UINT32 m40201c_0_32 = *((UINT32 *) 0x40201c);
	const UINT32 m402020_0_32 = *((UINT32 *) 0x402020);
	const UINT32 m402024_0_32 = *((UINT32 *) 0x402024);
	const UINT32 m402028_0_32 = *((UINT32 *) 0x402028);
	const UINT32 m40202c_0_32 = *((UINT32 *) 0x40202c);
	const UINT32 m4061a4_0_32 = *((UINT32 *) 0x4061a4);
	const UINT64 m4061a8_0_64 = *((UINT64 *) 0x4061a8);
	if (cond_1 (UINT32 (ebp_0), UINT32 (esp_0))) {
		if ((((((UINT32 (m22cc70_0_32) | (UINT32 (m22cc70_0_32) & 0xffffff)) & 0xffffffff) + UINT128 (0xffffffff, 0xffffffff, 0xffffffff, 0xffdd335b)) & 0xffffffffffffffff) /*0x0*/ == 0)) {
			if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x35)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (UINT32 (m22cca5_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x34)*//*}*/)) {
				if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x35)/*}*/ < /*signed {*/signExtend_0x80_0x8 (UINT32 (m22cca6_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x37)*//*}*/)) {
					if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x35)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (UINT32 (m22cca7_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x35)*//*}*/)) {
						{
							/*Memory Changes*/
							*((UINT32 *) 0x22cbc0) = UINT32 (0x4061430040612f);
							*((UINT32 *) 0x22cbc8) = UINT32 (0x2401ba000406042);
							*((UINT32 *) 0x22cbd0) = UINT32 (0x611ccfaf00401ba0);
							*((UINT32 *) 0x22cbd8) = UINT32 ((((esp_0 - 0x4) & 0xffffffff) | 0x40145500000000) /*0x4014550022cc18*/);
							*((UINT32 *) 0x22cbe0) = UINT32 ((((signExtend_0x20_0x8 (m22cca7_0_8) << 0x20) & 0xffffffff00000000) | 0x22cc3c) /*0x350022cc3c*/);
							*((UINT32 *) 0x22cbe8) = UINT32 (0x35);
							*((UINT32 *) 0x22cbf0) = UINT32 (((((m402020_0_32 & 0xff) | m402020_0_32) & 0xffffffff) | 0x611ccfaf00000000) /*0x611ccfaf00403124*/);
							*((UINT32 *) 0x22cbf8) = UINT32 (0xf0022cc48);
							*((UINT32 *) 0x22cc00) = UINT32 (0x4900000000);
							*((UINT32 *) 0x22cc08) = UINT32 (0x40615700000035);
							*((UINT32 *) 0x22cc10) = UINT32 ((((edi_0 & 0xffffffff) << 0x20) | (esi_0 & 0xffffffff)) /*0x611ccfaf0022cca1*/);
							*((UINT32 *) 0x22cc18) = UINT32 (((ebp_0 & 0xffffffff) | 0x40134b00000000) /*0x40134b0022cc48*/);
							*((UINT32 *) 0x22cc20) = UINT32 (0x22cc6c0022cc3c);
							*((UINT32 *) 0x22cc28) = UINT32 (0x22cc4c00000000);
							*((UINT32 *) 0x22cc30) = UINT32 (0x10000002f);
							*((UINT32 *) 0x22cc38) = UINT32 (0x40615f61300968);
							*((UINT32 *) 0x406040) = UINT32 (0xffff0022cc500101);
							*((UINT32 *) 0x406048) = UINT32 (0x10000000202ffff);
							*((UINT32 *) 0x406050) = UINT32 (0x15020000000002);
							*((UINT32 *) 0x406058) = UINT32 (((((m402008_0_32 << 0x18) & 0xffffff00000000) | 0xff00000000000000) | (((m402008_0_32 << 0x18) & 0xffffffff) | 0x20000)) /*0xff00403060020000*/);
							*((UINT32 *) 0x406060) = UINT32 (0xffffff0200ffffff);
							*((UINT32 *) 0x406068) = UINT32 (0x1000000000200ff);
							*((UINT32 *) 0x406070) = UINT32 (((((m22cc70_0_32 << 0x8) & 0xff00000000) | 0xffffff0000000000) | (((m22cc70_0_32 << 0x8) & 0xffffffff) | 0x3)) /*0xffffff0022cca503*/);
							*((UINT32 *) 0x406078) = UINT32 (0x2030000003502ff);
							*((UINT32 *) 0x406080) = UINT32 (0x8a0200000000);
							*((UINT32 *) 0x406088) = UINT32 ((((((m22cc70_0_32 + 0x1) << 0x18) & 0xffffff00000000) | 0xff00000000000000) | ((((m22cc70_0_32 + 0x1) << 0x18) & 0xff000000) | 0x30100)) /*0xff0022cca6030100*/);
							*((UINT32 *) 0x406090) = UINT32 (0x3502ffffff);
							*((UINT32 *) 0x406098) = UINT32 (0x3802000000000203);
							*((UINT32 *) 0x4060a0) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
							*((UINT32 *) 0x4060a8) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
							*((UINT32 *) 0x4060b0) = UINT32 (0x2030000);
							*((UINT32 *) 0x4060b8) = UINT32 ((((m40200c_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x3080020000000f02*/);
							*((UINT32 *) 0x4060c0) = UINT32 (((logicalShiftToRight (m40200c_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
							*((UINT32 *) 0x4060c8) = UINT32 ((((m402010_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030a00200000009*/);
							*((UINT32 *) 0x4060d0) = UINT32 (((logicalShiftToRight (m402010_0_32, 0x18) & 0xff) | 0x320203ffffffff00) /*0x320203ffffffff00*/);
							*((UINT32 *) 0x4060d8) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
							*((UINT32 *) 0x4060e0) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
							*((UINT32 *) 0x4060e8) = UINT32 (0x2030000);
							*((UINT32 *) 0x4060f0) = UINT32 ((((m402014_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x30c0020000000f02*/);
							*((UINT32 *) 0x4060f8) = UINT32 (((logicalShiftToRight (m402014_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
							*((UINT32 *) 0x406100) = UINT32 ((((m402018_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030e00200000009*/);
							*((UINT32 *) 0x406108) = UINT32 (((logicalShiftToRight (m402018_0_32, 0x18) & 0xff) | 0x840203ffffffff00) /*0x840203ffffffff00*/);
							*((UINT32 *) 0x406110) = UINT32 (((((m22cc70_0_32 + 0x1) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca60301000000*/);
							*((UINT32 *) 0x406118) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x1), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
							*((UINT32 *) 0x406120) = UINT32 (0x2030000);
							*((UINT32 *) 0x406128) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003802) /*0xa703010000003802*/);
							*((UINT32 *) 0x406130) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
							*((UINT32 *) 0x406138) = UINT32 (0x20300000035);
							*((UINT32 *) 0x406140) = UINT32 (0x20000000f020000);
							*((UINT32 *) 0x406148) = UINT32 ((m40201c_0_32 | 0xffffffff00000000) /*0xffffffff00403104*/);
							*((UINT32 *) 0x406150) = UINT32 ((((m402020_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x2402000000090203*/);
							*((UINT32 *) 0x406158) = UINT32 (((logicalShiftToRight (m402020_0_32, 0x8) & 0xffffff) | 0x3ffffffff000000) /*0x3ffffffff004031*/);
							*((UINT32 *) 0x406160) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003202) /*0xa703010000003202*/);
							*((UINT32 *) 0x406168) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
							*((UINT32 *) 0x406170) = UINT32 (0x20300000035);
							*((UINT32 *) 0x406178) = UINT32 (0x20000000f020000);
							*((UINT32 *) 0x406180) = UINT32 ((m402024_0_32 | 0xffffffff00000000) /*0xffffffff00403148*/);
							*((UINT32 *) 0x406188) = UINT32 ((((m402028_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x6c02000000090203*/);
							*((UINT32 *) 0x406190) = UINT32 (((logicalShiftToRight (m402028_0_32, 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff004031*/);
							*((UINT32 *) 0x406198) = UINT32 ((m40202c_0_32 | 0xffffffff00000000) /*0xffffffff00403190*/);
							*((UINT32 *) 0x4061a0) = UINT32 ((((m4061a4_0_32 & 0xff000000) << 0x20) | 0x200) /*0x200*/);
							*((UINT32 *) 0x4061a8) = UINT32 (m4061a8_0_64 /*0x0*/);
							/*Registers Changes*/
							regs.eax = UINT32 ((((m402020_0_32 & 0xff) | m402020_0_32) & 0xffffffff) /*0x403124*/);
							regs.ecx = UINT32 (0x0);
							regs.edx = UINT32 (0x40615f);
							regs.edi = UINT32 ((edi_0 & 0xffffffff) /*0x611ccfaf*/);
							regs.esi = UINT32 ((esi_0 & 0xffffffff) /*0x22cca1*/);
							regs.esp = UINT32 (((esp_0 - 0x2c) & 0xffffffff) /*0x22cbf0*/);
							regs.ebp = UINT32 (((esp_0 - 0x4) & 0xffffffff) /*0x22cc18*/);
							printf (/*"a <= 5, b > 5, c <= 5 -- case 6\n"*/ (const char *) (((m402020_0_32 & 0xff) | m402020_0_32) & 0xffffffff) /*0x403124*/);
							const UINT32 eax_1 = regs.eax, ebx_1 = regs.ebx, ecx_1 = regs.ecx, edx_1 = regs.edx, edi_1 = regs.edi, esi_1 = regs.esi, esp_1 = regs.esp, ebp_1 = regs.ebp;
							const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7);
							{
								/*Memory Changes*/
								/*Registers Changes*/
								return int (regs.eax);
							}
						}
					} else {
						/*Memory Changes*/
						*((UINT32 *) 0x22cbb0) = UINT32 (0x6fcd0000611b6708);
						*((UINT32 *) 0x22cbb8) = UINT32 (0x40612f6fcd0000);
						*((UINT32 *) 0x22cbc0) = UINT32 (0x40604200406143);
						*((UINT32 *) 0x22cbc8) = UINT32 (0x401ba002000000);
						*((UINT32 *) 0x22cbd0) = UINT32 (((((esp_0 - 0x8) << 0x20) & 0xffffffff00000000) | 0x401ba0) /*0x22cc1400401ba0*/);
						*((UINT32 *) 0x22cbd8) = UINT32 (0x22cc3800401455);
						*((UINT32 *) 0x22cbe0) = UINT32 ((((0x0 | (signExtend_0x20_0x8 (m22cca7_0_8) & 0xffffffff)) & 0xffffffff) | 0x3500000000) /*0x3500000040*/);
						*((UINT32 *) 0x22cbe8) = UINT32 ((m40201c_0_32 << 0x20) /*0x40310400000000*/);
						*((UINT32 *) 0x22cbf0) = UINT32 (0x611ccfaf00401ba0);
						*((UINT32 *) 0x22cbf8) = UINT32 (0xf);
						*((UINT32 *) 0x22cc00) = UINT32 (0x3501006610);
						*((UINT32 *) 0x22cc08) = UINT32 ((((esi_0 & 0xffffffff) << 0x20) | 0x406148) /*0x22cca100406148*/);
						*((UINT32 *) 0x22cc10) = UINT32 ((((ebp_0 & 0xffffffff) << 0x20) | ((0x0 | ((0x0 | ((0x0 | ((0x0 | ((0x0 | (edi_0 & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) /*0x22cc48611ccfaf*/);
						*((UINT32 *) 0x22cc18) = UINT32 (0x22cc380040134b);
						*((UINT32 *) 0x22cc20) = UINT32 (0x611cd07e0022cc6c);
						*((UINT32 *) 0x22cc28) = UINT32 (0x22cc4c00000000);
						*((UINT32 *) 0x22cc30) = UINT32 (0x10000002f);
						*((UINT32 *) 0x22cc38) = UINT32 (0x22cc6c00406150);
						*((UINT32 *) 0x406040) = UINT32 (0xffff0022cc500101);
						*((UINT32 *) 0x406048) = UINT32 (0x10000000202ffff);
						*((UINT32 *) 0x406050) = UINT32 (0x15020000000002);
						*((UINT32 *) 0x406058) = UINT32 (((((m402008_0_32 << 0x18) & 0xffffff00000000) | 0xff00000000000000) | (((m402008_0_32 << 0x18) & 0xffffffff) | 0x20000)) /*0xff00000000020000*/);
						*((UINT32 *) 0x406060) = UINT32 (0xffffff0200ffffff);
						*((UINT32 *) 0x406068) = UINT32 (0x1000000000200ff);
						*((UINT32 *) 0x406070) = UINT32 (((((m22cc70_0_32 << 0x8) & 0xff00000000) | 0xffffff0000000000) | (((m22cc70_0_32 << 0x8) & 0xffffffff) | 0x3)) /*0xffffff0022cca503*/);
						*((UINT32 *) 0x406078) = UINT32 (0x2030000003502ff);
						*((UINT32 *) 0x406080) = UINT32 (0x8a0200000000);
						*((UINT32 *) 0x406088) = UINT32 ((((((m22cc70_0_32 + 0x1) << 0x18) & 0xffffff00000000) | 0xff00000000000000) | ((((m22cc70_0_32 + 0x1) << 0x18) & 0xff000000) | 0x30100)) /*0xff0022cca6030100*/);
						*((UINT32 *) 0x406090) = UINT32 (0x3502ffffff);
						*((UINT32 *) 0x406098) = UINT32 (0x3802000000000203);
						*((UINT32 *) 0x4060a0) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
						*((UINT32 *) 0x4060a8) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x4060b0) = UINT32 (0x2030000);
						*((UINT32 *) 0x4060b8) = UINT32 ((((m40200c_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x3080020000000f02*/);
						*((UINT32 *) 0x4060c0) = UINT32 (((logicalShiftToRight (m40200c_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
						*((UINT32 *) 0x4060c8) = UINT32 ((((m402010_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030a00200000009*/);
						*((UINT32 *) 0x4060d0) = UINT32 (((logicalShiftToRight (m402010_0_32, 0x18) & 0xff) | 0x320203ffffffff00) /*0x320203ffffffff00*/);
						*((UINT32 *) 0x4060d8) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
						*((UINT32 *) 0x4060e0) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x4060e8) = UINT32 (0x2030000);
						*((UINT32 *) 0x4060f0) = UINT32 ((((m402014_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x30c0020000000f02*/);
						*((UINT32 *) 0x4060f8) = UINT32 (((logicalShiftToRight (m402014_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
						*((UINT32 *) 0x406100) = UINT32 ((((m402018_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030e00200000009*/);
						*((UINT32 *) 0x406108) = UINT32 (((logicalShiftToRight (m402018_0_32, 0x18) & 0xff) | 0x840203ffffffff00) /*0x840203ffffffff00*/);
						*((UINT32 *) 0x406110) = UINT32 (((((m22cc70_0_32 + 0x1) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca60301000000*/);
						*((UINT32 *) 0x406118) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x1), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x406120) = UINT32 (0x2030000);
						*((UINT32 *) 0x406128) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003802) /*0xa703010000003802*/);
						*((UINT32 *) 0x406130) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
						*((UINT32 *) 0x406138) = UINT32 (0x20300000035);
						*((UINT32 *) 0x406140) = UINT32 (0x20000000f020000);
						*((UINT32 *) 0x406148) = UINT32 ((m40201c_0_32 | 0xffffffff00000000) /*0xffffffff00403104*/);
						*((UINT32 *) 0x406150) = UINT32 ((((m402020_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x2402000000090203*/);
						*((UINT32 *) 0x406158) = UINT32 (((logicalShiftToRight (m402020_0_32, 0x8) & 0xffffff) | 0x3ffffffff000000) /*0x3ffffffff004031*/);
						*((UINT32 *) 0x406160) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003202) /*0xa703010000003202*/);
						*((UINT32 *) 0x406168) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
						*((UINT32 *) 0x406170) = UINT32 (0x20300000035);
						*((UINT32 *) 0x406178) = UINT32 (0x20000000f020000);
						*((UINT32 *) 0x406180) = UINT32 ((m402024_0_32 | 0xffffffff00000000) /*0xffffffff00403148*/);
						*((UINT32 *) 0x406188) = UINT32 ((((m402028_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x6c02000000090203*/);
						*((UINT32 *) 0x406190) = UINT32 (((logicalShiftToRight (m402028_0_32, 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff004031*/);
						*((UINT32 *) 0x406198) = UINT32 ((m40202c_0_32 | 0xffffffff00000000) /*0xffffffff00403190*/);
						*((UINT32 *) 0x4061a0) = UINT32 ((((m4061a4_0_32 & 0xff000000) << 0x20) | 0x200) /*0x200*/);
						*((UINT32 *) 0x4061a8) = UINT32 (m4061a8_0_64 /*0x0*/);
						/*Registers Changes*/
						regs.eax = UINT32 (m40201c_0_32 /*0x403104*/);
						regs.ecx = UINT32 (0x0);
						regs.edx = UINT32 (0x406150);
						regs.edi = UINT32 ((edi_0 & 0xffffffff) /*0x611ccfaf*/);
						regs.esi = UINT32 ((esi_0 & 0xffffffff) /*0x22cca1*/);
						regs.esp = UINT32 (((esp_0 - 0x30) & 0xffffffff) /*0x22cbec*/);
						regs.ebp = UINT32 (((esp_0 - 0x8) & 0xffffffff) /*0x22cc14*/);
						printf (/*"a <= 5, b > 5, c > 5 -- case 5\n"*/ (const char *) m40201c_0_32 /*0x403104*/);
						const UINT32 eax_1 = regs.eax, ebx_1 = regs.ebx, ecx_1 = regs.ecx, edx_1 = regs.edx, edi_1 = regs.edi, esi_1 = regs.esi, esp_1 = regs.esp, ebp_1 = regs.ebp;
						const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7);
						{
							/*Memory Changes*/
							/*Registers Changes*/
							return int (regs.eax);
						}
					}
				} else {
					if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x35)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (UINT32 (m22cca7_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x35)*//*}*/)) {
						{
							/*Memory Changes*/
							*((UINT32 *) 0x22cbb0) = UINT32 (0x6fcd0000611b6708);
							*((UINT32 *) 0x22cbb8) = UINT32 (0x4061676fcd0000);
							*((UINT32 *) 0x22cbc0) = UINT32 (0x4060420040617b);
							*((UINT32 *) 0x22cbc8) = UINT32 (0x401ba002000000);
							*((UINT32 *) 0x22cbd0) = UINT32 (((((esp_0 - 0x8) << 0x20) & 0xffffffff00000000) | 0x401ba0) /*0x22cc1400401ba0*/);
							*((UINT32 *) 0x22cbd8) = UINT32 (0x22cc3800401455);
							*((UINT32 *) 0x22cbe0) = UINT32 ((((0x0 | (signExtend_0x20_0x8 (m22cca7_0_8) & 0xffffffff)) & 0xffffffff) | 0x3500000000) /*0x3500000035*/);
							*((UINT32 *) 0x22cbe8) = UINT32 (((((m402028_0_32 & 0xff) | ((m402028_0_32 | 0xff00000000) & 0xffffff00)) & 0xffffffff) << 0x20) /*0x40316c00000000*/);
							*((UINT32 *) 0x22cbf0) = UINT32 (0x611ccfaf00401ba0);
							*((UINT32 *) 0x22cbf8) = UINT32 (0xf);
							*((UINT32 *) 0x22cc00) = UINT32 (0x3500006610);
							*((UINT32 *) 0x22cc08) = UINT32 ((((esi_0 & 0xffffffff) << 0x20) | 0x40618f) /*0x22cca10040618f*/);
							*((UINT32 *) 0x22cc10) = UINT32 ((((ebp_0 & 0xffffffff) << 0x20) | ((0x0 | ((0x0 | ((0x0 | ((0x0 | ((0x0 | (edi_0 & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) /*0x22cc48611ccfaf*/);
							*((UINT32 *) 0x22cc18) = UINT32 (0x22cc380040134b);
							*((UINT32 *) 0x22cc20) = UINT32 (0x611cd07e0022cc6c);
							*((UINT32 *) 0x22cc28) = UINT32 (0x22cc4c00000000);
							*((UINT32 *) 0x22cc30) = UINT32 (0x10000002f);
							*((UINT32 *) 0x22cc38) = UINT32 (0x22cc6c00406197);
							*((UINT32 *) 0x406040) = UINT32 (0xffff0022cc500101);
							*((UINT32 *) 0x406048) = UINT32 (0x10000000202ffff);
							*((UINT32 *) 0x406050) = UINT32 (0x15020000000002);
							*((UINT32 *) 0x406058) = UINT32 (((((m402008_0_32 << 0x18) & 0xffffff00000000) | 0xff00000000000000) | (((m402008_0_32 << 0x18) & 0xffffffff) | 0x20000)) /*0xff00000000020000*/);
							*((UINT32 *) 0x406060) = UINT32 (0xffffff0200ffffff);
							*((UINT32 *) 0x406068) = UINT32 (0x1000000000200ff);
							*((UINT32 *) 0x406070) = UINT32 (((((m22cc70_0_32 << 0x8) & 0xff00000000) | 0xffffff0000000000) | (((m22cc70_0_32 << 0x8) & 0xffffffff) | 0x3)) /*0xffffff0022cca503*/);
							*((UINT32 *) 0x406078) = UINT32 (0x2030000003502ff);
							*((UINT32 *) 0x406080) = UINT32 (0x8a0200000000);
							*((UINT32 *) 0x406088) = UINT32 ((((((m22cc70_0_32 + 0x1) << 0x18) & 0xffffff00000000) | 0xff00000000000000) | ((((m22cc70_0_32 + 0x1) << 0x18) & 0xff000000) | 0x30100)) /*0xff0022cca6030100*/);
							*((UINT32 *) 0x406090) = UINT32 (0x3502ffffff);
							*((UINT32 *) 0x406098) = UINT32 (0x3802000000000203);
							*((UINT32 *) 0x4060a0) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
							*((UINT32 *) 0x4060a8) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
							*((UINT32 *) 0x4060b0) = UINT32 (0x2030000);
							*((UINT32 *) 0x4060b8) = UINT32 ((((m40200c_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x3080020000000f02*/);
							*((UINT32 *) 0x4060c0) = UINT32 (((logicalShiftToRight (m40200c_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
							*((UINT32 *) 0x4060c8) = UINT32 ((((m402010_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030a00200000009*/);
							*((UINT32 *) 0x4060d0) = UINT32 (((logicalShiftToRight (m402010_0_32, 0x18) & 0xff) | 0x320203ffffffff00) /*0x320203ffffffff00*/);
							*((UINT32 *) 0x4060d8) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
							*((UINT32 *) 0x4060e0) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
							*((UINT32 *) 0x4060e8) = UINT32 (0x2030000);
							*((UINT32 *) 0x4060f0) = UINT32 ((((m402014_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x30c0020000000f02*/);
							*((UINT32 *) 0x4060f8) = UINT32 (((logicalShiftToRight (m402014_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
							*((UINT32 *) 0x406100) = UINT32 ((((m402018_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030e00200000009*/);
							*((UINT32 *) 0x406108) = UINT32 (((logicalShiftToRight (m402018_0_32, 0x18) & 0xff) | 0x840203ffffffff00) /*0x840203ffffffff00*/);
							*((UINT32 *) 0x406110) = UINT32 (((((m22cc70_0_32 + 0x1) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca60301000000*/);
							*((UINT32 *) 0x406118) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x1), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
							*((UINT32 *) 0x406120) = UINT32 (0x2030000);
							*((UINT32 *) 0x406128) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003802) /*0xa703010000003802*/);
							*((UINT32 *) 0x406130) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
							*((UINT32 *) 0x406138) = UINT32 (0x20300000035);
							*((UINT32 *) 0x406140) = UINT32 (0x20000000f020000);
							*((UINT32 *) 0x406148) = UINT32 ((m40201c_0_32 | 0xffffffff00000000) /*0xffffffff00403104*/);
							*((UINT32 *) 0x406150) = UINT32 ((((m402020_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x2402000000090203*/);
							*((UINT32 *) 0x406158) = UINT32 (((logicalShiftToRight (m402020_0_32, 0x8) & 0xffffff) | 0x3ffffffff000000) /*0x3ffffffff004031*/);
							*((UINT32 *) 0x406160) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003202) /*0xa703010000003202*/);
							*((UINT32 *) 0x406168) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
							*((UINT32 *) 0x406170) = UINT32 (0x20300000035);
							*((UINT32 *) 0x406178) = UINT32 (0x20000000f020000);
							*((UINT32 *) 0x406180) = UINT32 ((m402024_0_32 | 0xffffffff00000000) /*0xffffffff00403148*/);
							*((UINT32 *) 0x406188) = UINT32 ((((m402028_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x6c02000000090203*/);
							*((UINT32 *) 0x406190) = UINT32 (((logicalShiftToRight (m402028_0_32, 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff004031*/);
							*((UINT32 *) 0x406198) = UINT32 ((m40202c_0_32 | 0xffffffff00000000) /*0xffffffff00403190*/);
							*((UINT32 *) 0x4061a0) = UINT32 ((((m4061a4_0_32 & 0xff000000) << 0x20) | 0x200) /*0x200*/);
							*((UINT32 *) 0x4061a8) = UINT32 (m4061a8_0_64 /*0x0*/);
							/*Registers Changes*/
							regs.eax = UINT32 ((((m402028_0_32 & 0xff) | ((m402028_0_32 | 0xff00000000) & 0xffffff00)) & 0xffffffff) /*0x40316c*/);
							regs.ecx = UINT32 (0x0);
							regs.edx = UINT32 (0x406197);
							regs.edi = UINT32 ((edi_0 & 0xffffffff) /*0x611ccfaf*/);
							regs.esi = UINT32 ((esi_0 & 0xffffffff) /*0x22cca1*/);
							regs.esp = UINT32 (((esp_0 - 0x30) & 0xffffffff) /*0x22cbec*/);
							regs.ebp = UINT32 (((esp_0 - 0x8) & 0xffffffff) /*0x22cc14*/);
							printf (/*"a <= 5, b <= 5, c <= 5 -- case 8\n"*/ (const char *) (((m402028_0_32 & 0xff) | ((m402028_0_32 | 0xff00000000) & 0xffffff00)) & 0xffffffff) /*0x40316c*/);
							const UINT32 eax_1 = regs.eax, ebx_1 = regs.ebx, ecx_1 = regs.ecx, edx_1 = regs.edx, edi_1 = regs.edi, esi_1 = regs.esi, esp_1 = regs.esp, ebp_1 = regs.ebp;
							const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7);
							{
								/*Memory Changes*/
								/*Registers Changes*/
								return int (regs.eax);
							}
						}
					} else {
						/*Memory Changes*/
						*((UINT32 *) 0x22cbb0) = UINT32 (0x6fcd0000611b6708);
						*((UINT32 *) 0x22cbb8) = UINT32 (0x4061676fcd0000);
						*((UINT32 *) 0x22cbc0) = UINT32 (0x4060420040617b);
						*((UINT32 *) 0x22cbc8) = UINT32 (0x401ba002000000);
						*((UINT32 *) 0x22cbd0) = UINT32 (((((esp_0 - 0x8) << 0x20) & 0xffffffff00000000) | 0x401ba0) /*0x22cc1400401ba0*/);
						*((UINT32 *) 0x22cbd8) = UINT32 (0x22cc3800401455);
						*((UINT32 *) 0x22cbe0) = UINT32 ((((0x0 | (signExtend_0x20_0x8 (m22cca7_0_8) & 0xffffffff)) & 0xffffffff) | 0x3500000000) /*0x3500000040*/);
						*((UINT32 *) 0x22cbe8) = UINT32 ((((m402024_0_32 | 0xffffffff00000000) & 0xffffffff) << 0x20) /*0x40314800000000*/);
						*((UINT32 *) 0x22cbf0) = UINT32 (0x611ccfaf00401ba0);
						*((UINT32 *) 0x22cbf8) = UINT32 (0xf);
						*((UINT32 *) 0x22cc00) = UINT32 (0x3501006610);
						*((UINT32 *) 0x22cc08) = UINT32 ((((esi_0 & 0xffffffff) << 0x20) | 0x406180) /*0x22cca100406180*/);
						*((UINT32 *) 0x22cc10) = UINT32 ((((ebp_0 & 0xffffffff) << 0x20) | ((0x0 | ((0x0 | ((0x0 | ((0x0 | ((0x0 | (edi_0 & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) /*0x22cc48611ccfaf*/);
						*((UINT32 *) 0x22cc18) = UINT32 (0x22cc380040134b);
						*((UINT32 *) 0x22cc20) = UINT32 (0x611cd07e0022cc6c);
						*((UINT32 *) 0x22cc28) = UINT32 (0x22cc4c00000000);
						*((UINT32 *) 0x22cc30) = UINT32 (0x10000002f);
						*((UINT32 *) 0x22cc38) = UINT32 (0x22cc6c00406188);
						*((UINT32 *) 0x406040) = UINT32 (0xffff0022cc500101);
						*((UINT32 *) 0x406048) = UINT32 (0x10000000202ffff);
						*((UINT32 *) 0x406050) = UINT32 (0x15020000000002);
						*((UINT32 *) 0x406058) = UINT32 (((((m402008_0_32 << 0x18) & 0xffffff00000000) | 0xff00000000000000) | (((m402008_0_32 << 0x18) & 0xffffffff) | 0x20000)) /*0xff00000000020000*/);
						*((UINT32 *) 0x406060) = UINT32 (0xffffff0200ffffff);
						*((UINT32 *) 0x406068) = UINT32 (0x1000000000200ff);
						*((UINT32 *) 0x406070) = UINT32 (((((m22cc70_0_32 << 0x8) & 0xff00000000) | 0xffffff0000000000) | (((m22cc70_0_32 << 0x8) & 0xffffffff) | 0x3)) /*0xffffff0022cca503*/);
						*((UINT32 *) 0x406078) = UINT32 (0x2030000003502ff);
						*((UINT32 *) 0x406080) = UINT32 (0x8a0200000000);
						*((UINT32 *) 0x406088) = UINT32 ((((((m22cc70_0_32 + 0x1) << 0x18) & 0xffffff00000000) | 0xff00000000000000) | ((((m22cc70_0_32 + 0x1) << 0x18) & 0xff000000) | 0x30100)) /*0xff0022cca6030100*/);
						*((UINT32 *) 0x406090) = UINT32 (0x3502ffffff);
						*((UINT32 *) 0x406098) = UINT32 (0x3802000000000203);
						*((UINT32 *) 0x4060a0) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
						*((UINT32 *) 0x4060a8) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x4060b0) = UINT32 (0x2030000);
						*((UINT32 *) 0x4060b8) = UINT32 ((((m40200c_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x3080020000000f02*/);
						*((UINT32 *) 0x4060c0) = UINT32 (((logicalShiftToRight (m40200c_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
						*((UINT32 *) 0x4060c8) = UINT32 ((((m402010_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030a00200000009*/);
						*((UINT32 *) 0x4060d0) = UINT32 (((logicalShiftToRight (m402010_0_32, 0x18) & 0xff) | 0x320203ffffffff00) /*0x320203ffffffff00*/);
						*((UINT32 *) 0x4060d8) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
						*((UINT32 *) 0x4060e0) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x4060e8) = UINT32 (0x2030000);
						*((UINT32 *) 0x4060f0) = UINT32 ((((m402014_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x30c0020000000f02*/);
						*((UINT32 *) 0x4060f8) = UINT32 (((logicalShiftToRight (m402014_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
						*((UINT32 *) 0x406100) = UINT32 ((((m402018_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030e00200000009*/);
						*((UINT32 *) 0x406108) = UINT32 (((logicalShiftToRight (m402018_0_32, 0x18) & 0xff) | 0x840203ffffffff00) /*0x840203ffffffff00*/);
						*((UINT32 *) 0x406110) = UINT32 (((((m22cc70_0_32 + 0x1) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca60301000000*/);
						*((UINT32 *) 0x406118) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x1), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x406120) = UINT32 (0x2030000);
						*((UINT32 *) 0x406128) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003802) /*0xa703010000003802*/);
						*((UINT32 *) 0x406130) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
						*((UINT32 *) 0x406138) = UINT32 (0x20300000035);
						*((UINT32 *) 0x406140) = UINT32 (0x20000000f020000);
						*((UINT32 *) 0x406148) = UINT32 ((m40201c_0_32 | 0xffffffff00000000) /*0xffffffff00403104*/);
						*((UINT32 *) 0x406150) = UINT32 ((((m402020_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x2402000000090203*/);
						*((UINT32 *) 0x406158) = UINT32 (((logicalShiftToRight (m402020_0_32, 0x8) & 0xffffff) | 0x3ffffffff000000) /*0x3ffffffff004031*/);
						*((UINT32 *) 0x406160) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003202) /*0xa703010000003202*/);
						*((UINT32 *) 0x406168) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
						*((UINT32 *) 0x406170) = UINT32 (0x20300000035);
						*((UINT32 *) 0x406178) = UINT32 (0x20000000f020000);
						*((UINT32 *) 0x406180) = UINT32 ((m402024_0_32 | 0xffffffff00000000) /*0xffffffff00403148*/);
						*((UINT32 *) 0x406188) = UINT32 ((((m402028_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x6c02000000090203*/);
						*((UINT32 *) 0x406190) = UINT32 (((logicalShiftToRight (m402028_0_32, 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff004031*/);
						*((UINT32 *) 0x406198) = UINT32 ((m40202c_0_32 | 0xffffffff00000000) /*0xffffffff00403190*/);
						*((UINT32 *) 0x4061a0) = UINT32 ((((m4061a4_0_32 & 0xff000000) << 0x20) | 0x200) /*0x200*/);
						*((UINT32 *) 0x4061a8) = UINT32 (m4061a8_0_64 /*0x0*/);
						/*Registers Changes*/
						regs.eax = UINT32 (((m402024_0_32 | 0xffffffff00000000) & 0xffffffff) /*0x403148*/);
						regs.ecx = UINT32 (0x0);
						regs.edx = UINT32 (0x406188);
						regs.edi = UINT32 ((edi_0 & 0xffffffff) /*0x611ccfaf*/);
						regs.esi = UINT32 ((esi_0 & 0xffffffff) /*0x22cca1*/);
						regs.esp = UINT32 (((esp_0 - 0x30) & 0xffffffff) /*0x22cbec*/);
						regs.ebp = UINT32 (((esp_0 - 0x8) & 0xffffffff) /*0x22cc14*/);
						printf (/*"a <= 5, b <= 5, c > 5 -- case 7\n"*/ (const char *) ((m402024_0_32 | 0xffffffff00000000) & 0xffffffff) /*0x403148*/);
						const UINT32 eax_1 = regs.eax, ebx_1 = regs.ebx, ecx_1 = regs.ecx, edx_1 = regs.edx, edi_1 = regs.edi, esi_1 = regs.esi, esp_1 = regs.esp, ebp_1 = regs.ebp;
						const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7);
						{
							/*Memory Changes*/
							/*Registers Changes*/
							return int (regs.eax);
						}
					}
				}
			} else {
				if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x35)/*}*/ < /*signed {*/signExtend_0x80_0x8 (UINT32 (m22cca6_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x37)*//*}*/)) {
					if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x35)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (UINT32 (m22cca7_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x35)*//*}*/)) {
						{
							{
								/*Memory Changes*/
								*((UINT32 *) 0x22cbb0) = UINT32 (0x6fcd0000611b6708);
								*((UINT32 *) 0x22cbb8) = UINT32 (0x4060a56fcd0000);
								*((UINT32 *) 0x22cbc0) = UINT32 (0x406042004060b9);
								*((UINT32 *) 0x22cbc8) = UINT32 (0x401ba002000000);
								*((UINT32 *) 0x22cbd0) = UINT32 (((((esp_0 - 0x8) << 0x20) & 0xffffffff00000000) | 0x401ba0) /*0x22cc1400401ba0*/);
								*((UINT32 *) 0x22cbd8) = UINT32 (0x22cc3800401455);
								*((UINT32 *) 0x22cbe0) = UINT32 ((((0x0 | (signExtend_0x20_0x8 (m22cca7_0_8) & 0xffffffff)) & 0xffffffff) | 0x3500000000) /*0x3500000035*/);
								*((UINT32 *) 0x22cbe8) = UINT32 (((((m402010_0_32 & 0xffffff) | ((m402010_0_32 | 0xffffff00000000) & 0xff000000)) & 0xffffffff) << 0x20) /*0x4030a000000000*/);
								*((UINT32 *) 0x22cbf0) = UINT32 (0x611ccfaf00401ba0);
								*((UINT32 *) 0x22cbf8) = UINT32 ((((m40200c_0_32 << 0x28) | 0xf) & 0xff00000f) /*0xf*/);
								*((UINT32 *) 0x22cc00) = UINT32 (0x3500006610);
								*((UINT32 *) 0x22cc08) = UINT32 ((((esi_0 & 0xffffffff) << 0x20) | 0x4060cd) /*0x22cca1004060cd*/);
								*((UINT32 *) 0x22cc10) = UINT32 ((((ebp_0 & 0xffffffff) << 0x20) | ((0x0 | ((0x0 | ((0x0 | ((0x0 | ((0x0 | (edi_0 & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) /*0x22cc48611ccfaf*/);
								*((UINT32 *) 0x22cc18) = UINT32 (0x22cc380040134b);
								*((UINT32 *) 0x22cc20) = UINT32 (0x611cd07e0022cc6c);
								*((UINT32 *) 0x22cc28) = UINT32 (0x22cc4c00000000);
								*((UINT32 *) 0x22cc30) = UINT32 (0x10000002f);
								*((UINT32 *) 0x22cc38) = UINT32 (0x22cc6c004060d5);
								*((UINT32 *) 0x406040) = UINT32 (0xffff0022cc500101);
								*((UINT32 *) 0x406048) = UINT32 (0x10000000202ffff);
								*((UINT32 *) 0x406050) = UINT32 (0x15020000000002);
								*((UINT32 *) 0x406058) = UINT32 (((((m402008_0_32 << 0x18) & 0xffffff00000000) | 0xff00000000000000) | (((m402008_0_32 << 0x18) & 0xffffffff) | 0x20000)) /*0xff00000000020000*/);
								*((UINT32 *) 0x406060) = UINT32 (0xffffff0200ffffff);
								*((UINT32 *) 0x406068) = UINT32 (0x1000000000200ff);
								*((UINT32 *) 0x406070) = UINT32 (((((m22cc70_0_32 << 0x8) & 0xff00000000) | 0xffffff0000000000) | (((m22cc70_0_32 << 0x8) & 0xffffffff) | 0x3)) /*0xffffff0022cca503*/);
								*((UINT32 *) 0x406078) = UINT32 (0x2030000003502ff);
								*((UINT32 *) 0x406080) = UINT32 (0x8a0200000000);
								*((UINT32 *) 0x406088) = UINT32 ((((((m22cc70_0_32 + 0x1) << 0x18) & 0xffffff00000000) | 0xff00000000000000) | ((((m22cc70_0_32 + 0x1) << 0x18) & 0xff000000) | 0x30100)) /*0xff0022cca6030100*/);
								*((UINT32 *) 0x406090) = UINT32 (0x3502ffffff);
								*((UINT32 *) 0x406098) = UINT32 (0x3802000000000203);
								*((UINT32 *) 0x4060a0) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
								*((UINT32 *) 0x4060a8) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
								*((UINT32 *) 0x4060b0) = UINT32 (0x2030000);
								*((UINT32 *) 0x4060b8) = UINT32 ((((m40200c_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x3080020000000f02*/);
								*((UINT32 *) 0x4060c0) = UINT32 (((logicalShiftToRight (m40200c_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
								*((UINT32 *) 0x4060c8) = UINT32 ((((m402010_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030a00200000009*/);
								*((UINT32 *) 0x4060d0) = UINT32 (((logicalShiftToRight (m402010_0_32, 0x18) & 0xff) | 0x320203ffffffff00) /*0x320203ffffffff00*/);
								*((UINT32 *) 0x4060d8) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
								*((UINT32 *) 0x4060e0) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
								*((UINT32 *) 0x4060e8) = UINT32 (0x2030000);
								*((UINT32 *) 0x4060f0) = UINT32 ((((m402014_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x30c0020000000f02*/);
								*((UINT32 *) 0x4060f8) = UINT32 (((logicalShiftToRight (m402014_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
								*((UINT32 *) 0x406100) = UINT32 ((((m402018_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030e00200000009*/);
								*((UINT32 *) 0x406108) = UINT32 (((logicalShiftToRight (m402018_0_32, 0x18) & 0xff) | 0x840203ffffffff00) /*0x840203ffffffff00*/);
								*((UINT32 *) 0x406110) = UINT32 (((((m22cc70_0_32 + 0x1) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca60301000000*/);
								*((UINT32 *) 0x406118) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x1), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
								*((UINT32 *) 0x406120) = UINT32 (0x2030000);
								*((UINT32 *) 0x406128) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003802) /*0xa703010000003802*/);
								*((UINT32 *) 0x406130) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
								*((UINT32 *) 0x406138) = UINT32 (0x20300000035);
								*((UINT32 *) 0x406140) = UINT32 (0x20000000f020000);
								*((UINT32 *) 0x406148) = UINT32 ((m40201c_0_32 | 0xffffffff00000000) /*0xffffffff00403104*/);
								*((UINT32 *) 0x406150) = UINT32 ((((m402020_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x2402000000090203*/);
								*((UINT32 *) 0x406158) = UINT32 (((logicalShiftToRight (m402020_0_32, 0x8) & 0xffffff) | 0x3ffffffff000000) /*0x3ffffffff004031*/);
								*((UINT32 *) 0x406160) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003202) /*0xa703010000003202*/);
								*((UINT32 *) 0x406168) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
								*((UINT32 *) 0x406170) = UINT32 (0x20300000035);
								*((UINT32 *) 0x406178) = UINT32 (0x20000000f020000);
								*((UINT32 *) 0x406180) = UINT32 ((m402024_0_32 | 0xffffffff00000000) /*0xffffffff00403148*/);
								*((UINT32 *) 0x406188) = UINT32 ((((m402028_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x6c02000000090203*/);
								*((UINT32 *) 0x406190) = UINT32 (((logicalShiftToRight (m402028_0_32, 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff004031*/);
								*((UINT32 *) 0x406198) = UINT32 ((m40202c_0_32 | 0xffffffff00000000) /*0xffffffff00403190*/);
								*((UINT32 *) 0x4061a0) = UINT32 ((((m4061a4_0_32 & 0xff000000) << 0x20) | 0x200) /*0x200*/);
								*((UINT32 *) 0x4061a8) = UINT32 (m4061a8_0_64 /*0x0*/);
								/*Registers Changes*/
								regs.eax = UINT32 ((((m402010_0_32 & 0xffffff) | ((m402010_0_32 | 0xffffff00000000) & 0xff000000)) & 0xffffffff) /*0x4030a0*/);
								regs.ecx = UINT32 (0x0);
								regs.edx = UINT32 (0x4060d5);
								regs.edi = UINT32 ((edi_0 & 0xffffffff) /*0x611ccfaf*/);
								regs.esi = UINT32 ((esi_0 & 0xffffffff) /*0x22cca1*/);
								regs.esp = UINT32 (((esp_0 - 0x30) & 0xffffffff) /*0x22cbec*/);
								regs.ebp = UINT32 (((esp_0 - 0x8) & 0xffffffff) /*0x22cc14*/);
								printf (/*"a > 5, b > 5, c <= 5 -- case 2\n"*/ (const char *) (((m402010_0_32 & 0xffffff) | ((m402010_0_32 | 0xffffff00000000) & 0xff000000)) & 0xffffffff) /*0x4030a0*/);
								const UINT32 eax_1 = regs.eax, ebx_1 = regs.ebx, ecx_1 = regs.ecx, edx_1 = regs.edx, edi_1 = regs.edi, esi_1 = regs.esi, esp_1 = regs.esp, ebp_1 = regs.ebp;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7);
								{
									/*Memory Changes*/
									/*Registers Changes*/
									return int (regs.eax);
								}
							}
						}
					} else {
						/*Memory Changes*/
						*((UINT32 *) 0x22cbb0) = UINT32 (0x6fcd0000611b6708);
						*((UINT32 *) 0x22cbb8) = UINT32 (0x4060a56fcd0000);
						*((UINT32 *) 0x22cbc0) = UINT32 (0x406042004060b9);
						*((UINT32 *) 0x22cbc8) = UINT32 (0x401ba002000000);
						*((UINT32 *) 0x22cbd0) = UINT32 (((((esp_0 - 0x8) << 0x20) & 0xffffffff00000000) | 0x401ba0) /*0x22cc1400401ba0*/);
						*((UINT32 *) 0x22cbd8) = UINT32 (0x22cc3800401455);
						*((UINT32 *) 0x22cbe0) = UINT32 ((((0x0 | (signExtend_0x20_0x8 (m22cca7_0_8) & 0xffffffff)) & 0xffffffff) | 0x3500000000) /*0x3500000040*/);
						*((UINT32 *) 0x22cbe8) = UINT32 (((((m40200c_0_32 & 0xffff) | ((m40200c_0_32 | 0xffff00000000) & 0xffff0000)) & 0xffffffff) << 0x20) /*0x40308000000000*/);
						*((UINT32 *) 0x22cbf0) = UINT32 (0x611ccfaf00401ba0);
						*((UINT32 *) 0x22cbf8) = UINT32 ((((m40200c_0_32 << 0x28) | 0xf) & 0xff00000f) /*0xf*/);
						*((UINT32 *) 0x22cc00) = UINT32 (0x3501006610);
						*((UINT32 *) 0x22cc08) = UINT32 ((((esi_0 & 0xffffffff) << 0x20) | 0x4060be) /*0x22cca1004060be*/);
						*((UINT32 *) 0x22cc10) = UINT32 ((((ebp_0 & 0xffffffff) << 0x20) | ((0x0 | ((0x0 | ((0x0 | ((0x0 | ((0x0 | (edi_0 & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) /*0x22cc48611ccfaf*/);
						*((UINT32 *) 0x22cc18) = UINT32 (0x22cc380040134b);
						*((UINT32 *) 0x22cc20) = UINT32 (0x611cd07e0022cc6c);
						*((UINT32 *) 0x22cc28) = UINT32 (0x22cc4c00000000);
						*((UINT32 *) 0x22cc30) = UINT32 (0x10000002f);
						*((UINT32 *) 0x22cc38) = UINT32 (0x22cc6c004060c6);
						*((UINT32 *) 0x406040) = UINT32 (0xffff0022cc500101);
						*((UINT32 *) 0x406048) = UINT32 (0x10000000202ffff);
						*((UINT32 *) 0x406050) = UINT32 (0x15020000000002);
						*((UINT32 *) 0x406058) = UINT32 (((((m402008_0_32 << 0x18) & 0xffffff00000000) | 0xff00000000000000) | (((m402008_0_32 << 0x18) & 0xffffffff) | 0x20000)) /*0xff00000000020000*/);
						*((UINT32 *) 0x406060) = UINT32 (0xffffff0200ffffff);
						*((UINT32 *) 0x406068) = UINT32 (0x1000000000200ff);
						*((UINT32 *) 0x406070) = UINT32 (((((m22cc70_0_32 << 0x8) & 0xff00000000) | 0xffffff0000000000) | (((m22cc70_0_32 << 0x8) & 0xffffffff) | 0x3)) /*0xffffff0022cca503*/);
						*((UINT32 *) 0x406078) = UINT32 (0x2030000003502ff);
						*((UINT32 *) 0x406080) = UINT32 (0x8a0200000000);
						*((UINT32 *) 0x406088) = UINT32 ((((((m22cc70_0_32 + 0x1) << 0x18) & 0xffffff00000000) | 0xff00000000000000) | ((((m22cc70_0_32 + 0x1) << 0x18) & 0xff000000) | 0x30100)) /*0xff0022cca6030100*/);
						*((UINT32 *) 0x406090) = UINT32 (0x3502ffffff);
						*((UINT32 *) 0x406098) = UINT32 (0x3802000000000203);
						*((UINT32 *) 0x4060a0) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
						*((UINT32 *) 0x4060a8) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x4060b0) = UINT32 (0x2030000);
						*((UINT32 *) 0x4060b8) = UINT32 ((((m40200c_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x3080020000000f02*/);
						*((UINT32 *) 0x4060c0) = UINT32 (((logicalShiftToRight (m40200c_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
						*((UINT32 *) 0x4060c8) = UINT32 ((((m402010_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030a00200000009*/);
						*((UINT32 *) 0x4060d0) = UINT32 (((logicalShiftToRight (m402010_0_32, 0x18) & 0xff) | 0x320203ffffffff00) /*0x320203ffffffff00*/);
						*((UINT32 *) 0x4060d8) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
						*((UINT32 *) 0x4060e0) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x4060e8) = UINT32 (0x2030000);
						*((UINT32 *) 0x4060f0) = UINT32 ((((m402014_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x30c0020000000f02*/);
						*((UINT32 *) 0x4060f8) = UINT32 (((logicalShiftToRight (m402014_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
						*((UINT32 *) 0x406100) = UINT32 ((((m402018_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030e00200000009*/);
						*((UINT32 *) 0x406108) = UINT32 (((logicalShiftToRight (m402018_0_32, 0x18) & 0xff) | 0x840203ffffffff00) /*0x840203ffffffff00*/);
						*((UINT32 *) 0x406110) = UINT32 (((((m22cc70_0_32 + 0x1) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca60301000000*/);
						*((UINT32 *) 0x406118) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x1), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x406120) = UINT32 (0x2030000);
						*((UINT32 *) 0x406128) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003802) /*0xa703010000003802*/);
						*((UINT32 *) 0x406130) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
						*((UINT32 *) 0x406138) = UINT32 (0x20300000035);
						*((UINT32 *) 0x406140) = UINT32 (0x20000000f020000);
						*((UINT32 *) 0x406148) = UINT32 ((m40201c_0_32 | 0xffffffff00000000) /*0xffffffff00403104*/);
						*((UINT32 *) 0x406150) = UINT32 ((((m402020_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x2402000000090203*/);
						*((UINT32 *) 0x406158) = UINT32 (((logicalShiftToRight (m402020_0_32, 0x8) & 0xffffff) | 0x3ffffffff000000) /*0x3ffffffff004031*/);
						*((UINT32 *) 0x406160) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003202) /*0xa703010000003202*/);
						*((UINT32 *) 0x406168) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
						*((UINT32 *) 0x406170) = UINT32 (0x20300000035);
						*((UINT32 *) 0x406178) = UINT32 (0x20000000f020000);
						*((UINT32 *) 0x406180) = UINT32 ((m402024_0_32 | 0xffffffff00000000) /*0xffffffff00403148*/);
						*((UINT32 *) 0x406188) = UINT32 ((((m402028_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x6c02000000090203*/);
						*((UINT32 *) 0x406190) = UINT32 (((logicalShiftToRight (m402028_0_32, 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff004031*/);
						*((UINT32 *) 0x406198) = UINT32 ((m40202c_0_32 | 0xffffffff00000000) /*0xffffffff00403190*/);
						*((UINT32 *) 0x4061a0) = UINT32 ((((m4061a4_0_32 & 0xff000000) << 0x20) | 0x200) /*0x200*/);
						*((UINT32 *) 0x4061a8) = UINT32 (m4061a8_0_64 /*0x0*/);
						/*Registers Changes*/
						regs.eax = UINT32 ((((m40200c_0_32 & 0xffff) | ((m40200c_0_32 | 0xffff00000000) & 0xffff0000)) & 0xffffffff) /*0x403080*/);
						regs.ecx = UINT32 (0x0);
						regs.edx = UINT32 (0x4060c6);
						regs.edi = UINT32 ((edi_0 & 0xffffffff) /*0x611ccfaf*/);
						regs.esi = UINT32 ((esi_0 & 0xffffffff) /*0x22cca1*/);
						regs.esp = UINT32 (((esp_0 - 0x30) & 0xffffffff) /*0x22cbec*/);
						regs.ebp = UINT32 (((esp_0 - 0x8) & 0xffffffff) /*0x22cc14*/);
						printf (/*"a > 5, b > 5, c > 5 -- case 1\n"*/ (const char *) (((m40200c_0_32 & 0xffff) | ((m40200c_0_32 | 0xffff00000000) & 0xffff0000)) & 0xffffffff) /*0x403080*/);
						const UINT32 eax_1 = regs.eax, ebx_1 = regs.ebx, ecx_1 = regs.ecx, edx_1 = regs.edx, edi_1 = regs.edi, esi_1 = regs.esi, esp_1 = regs.esp, ebp_1 = regs.ebp;
						const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7);
						{
							/*Memory Changes*/
							/*Registers Changes*/
							return int (regs.eax);
						}
					}
				} else {
					if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x35)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (UINT32 (m22cca7_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x35)*//*}*/)) {
						{
							{
								/*Memory Changes*/
								*((UINT32 *) 0x22cbb0) = UINT32 (0x6fcd0000611b6708);
								*((UINT32 *) 0x22cbb8) = UINT32 (0x4060dd6fcd0000);
								*((UINT32 *) 0x22cbc0) = UINT32 (0x406042004060f1);
								*((UINT32 *) 0x22cbc8) = UINT32 (0x401ba002000000);
								*((UINT32 *) 0x22cbd0) = UINT32 (((((esp_0 - 0x8) << 0x20) & 0xffffffff00000000) | 0x401ba0) /*0x22cc1400401ba0*/);
								*((UINT32 *) 0x22cbd8) = UINT32 (0x22cc3800401455);
								*((UINT32 *) 0x22cbe0) = UINT32 ((((0x0 | (signExtend_0x20_0x8 (m22cca7_0_8) & 0xffffffff)) & 0xffffffff) | 0x3500000000) /*0x3500000035*/);
								*((UINT32 *) 0x22cbe8) = UINT32 (((((m402018_0_32 & 0xffffff) | m402018_0_32) & 0xffffffff) << 0x20) /*0x4030e000000000*/);
								*((UINT32 *) 0x22cbf0) = UINT32 (0x611ccfaf00401ba0);
								*((UINT32 *) 0x22cbf8) = UINT32 ((((m402014_0_32 << 0x28) | 0xf) & 0xff00000f) /*0xf*/);
								*((UINT32 *) 0x22cc00) = UINT32 (0x3500006610);
								*((UINT32 *) 0x22cc08) = UINT32 ((((esi_0 & 0xffffffff) << 0x20) | 0x406105) /*0x22cca100406105*/);
								*((UINT32 *) 0x22cc10) = UINT32 ((((ebp_0 & 0xffffffff) << 0x20) | ((0x0 | ((0x0 | ((0x0 | ((0x0 | ((0x0 | (edi_0 & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) /*0x22cc48611ccfaf*/);
								*((UINT32 *) 0x22cc18) = UINT32 (0x22cc380040134b);
								*((UINT32 *) 0x22cc20) = UINT32 (0x611cd07e0022cc6c);
								*((UINT32 *) 0x22cc28) = UINT32 (0x22cc4c00000000);
								*((UINT32 *) 0x22cc30) = UINT32 (0x10000002f);
								*((UINT32 *) 0x22cc38) = UINT32 (0x22cc6c0040610d);
								*((UINT32 *) 0x406040) = UINT32 (0xffff0022cc500101);
								*((UINT32 *) 0x406048) = UINT32 (0x10000000202ffff);
								*((UINT32 *) 0x406050) = UINT32 (0x15020000000002);
								*((UINT32 *) 0x406058) = UINT32 (((((m402008_0_32 << 0x18) & 0xffffff00000000) | 0xff00000000000000) | (((m402008_0_32 << 0x18) & 0xffffffff) | 0x20000)) /*0xff00000000020000*/);
								*((UINT32 *) 0x406060) = UINT32 (0xffffff0200ffffff);
								*((UINT32 *) 0x406068) = UINT32 (0x1000000000200ff);
								*((UINT32 *) 0x406070) = UINT32 (((((m22cc70_0_32 << 0x8) & 0xff00000000) | 0xffffff0000000000) | (((m22cc70_0_32 << 0x8) & 0xffffffff) | 0x3)) /*0xffffff0022cca503*/);
								*((UINT32 *) 0x406078) = UINT32 (0x2030000003502ff);
								*((UINT32 *) 0x406080) = UINT32 (0x8a0200000000);
								*((UINT32 *) 0x406088) = UINT32 ((((((m22cc70_0_32 + 0x1) << 0x18) & 0xffffff00000000) | 0xff00000000000000) | ((((m22cc70_0_32 + 0x1) << 0x18) & 0xff000000) | 0x30100)) /*0xff0022cca6030100*/);
								*((UINT32 *) 0x406090) = UINT32 (0x3502ffffff);
								*((UINT32 *) 0x406098) = UINT32 (0x3802000000000203);
								*((UINT32 *) 0x4060a0) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
								*((UINT32 *) 0x4060a8) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
								*((UINT32 *) 0x4060b0) = UINT32 (0x2030000);
								*((UINT32 *) 0x4060b8) = UINT32 ((((m40200c_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x3080020000000f02*/);
								*((UINT32 *) 0x4060c0) = UINT32 (((logicalShiftToRight (m40200c_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
								*((UINT32 *) 0x4060c8) = UINT32 ((((m402010_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030a00200000009*/);
								*((UINT32 *) 0x4060d0) = UINT32 (((logicalShiftToRight (m402010_0_32, 0x18) & 0xff) | 0x320203ffffffff00) /*0x320203ffffffff00*/);
								*((UINT32 *) 0x4060d8) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
								*((UINT32 *) 0x4060e0) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
								*((UINT32 *) 0x4060e8) = UINT32 (0x2030000);
								*((UINT32 *) 0x4060f0) = UINT32 ((((m402014_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x30c0020000000f02*/);
								*((UINT32 *) 0x4060f8) = UINT32 (((logicalShiftToRight (m402014_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
								*((UINT32 *) 0x406100) = UINT32 ((((m402018_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030e00200000009*/);
								*((UINT32 *) 0x406108) = UINT32 (((logicalShiftToRight (m402018_0_32, 0x18) & 0xff) | 0x840203ffffffff00) /*0x840203ffffffff00*/);
								*((UINT32 *) 0x406110) = UINT32 (((((m22cc70_0_32 + 0x1) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca60301000000*/);
								*((UINT32 *) 0x406118) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x1), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
								*((UINT32 *) 0x406120) = UINT32 (0x2030000);
								*((UINT32 *) 0x406128) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003802) /*0xa703010000003802*/);
								*((UINT32 *) 0x406130) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
								*((UINT32 *) 0x406138) = UINT32 (0x20300000035);
								*((UINT32 *) 0x406140) = UINT32 (0x20000000f020000);
								*((UINT32 *) 0x406148) = UINT32 ((m40201c_0_32 | 0xffffffff00000000) /*0xffffffff00403104*/);
								*((UINT32 *) 0x406150) = UINT32 ((((m402020_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x2402000000090203*/);
								*((UINT32 *) 0x406158) = UINT32 (((logicalShiftToRight (m402020_0_32, 0x8) & 0xffffff) | 0x3ffffffff000000) /*0x3ffffffff004031*/);
								*((UINT32 *) 0x406160) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003202) /*0xa703010000003202*/);
								*((UINT32 *) 0x406168) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
								*((UINT32 *) 0x406170) = UINT32 (0x20300000035);
								*((UINT32 *) 0x406178) = UINT32 (0x20000000f020000);
								*((UINT32 *) 0x406180) = UINT32 ((m402024_0_32 | 0xffffffff00000000) /*0xffffffff00403148*/);
								*((UINT32 *) 0x406188) = UINT32 ((((m402028_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x6c02000000090203*/);
								*((UINT32 *) 0x406190) = UINT32 (((logicalShiftToRight (m402028_0_32, 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff004031*/);
								*((UINT32 *) 0x406198) = UINT32 ((m40202c_0_32 | 0xffffffff00000000) /*0xffffffff00403190*/);
								*((UINT32 *) 0x4061a0) = UINT32 ((((m4061a4_0_32 & 0xff000000) << 0x20) | 0x200) /*0x200*/);
								*((UINT32 *) 0x4061a8) = UINT32 (m4061a8_0_64 /*0x0*/);
								/*Registers Changes*/
								regs.eax = UINT32 ((((m402018_0_32 & 0xffffff) | m402018_0_32) & 0xffffffff) /*0x4030e0*/);
								regs.ecx = UINT32 (0x0);
								regs.edx = UINT32 (0x40610d);
								regs.edi = UINT32 ((edi_0 & 0xffffffff) /*0x611ccfaf*/);
								regs.esi = UINT32 ((esi_0 & 0xffffffff) /*0x22cca1*/);
								regs.esp = UINT32 (((esp_0 - 0x30) & 0xffffffff) /*0x22cbec*/);
								regs.ebp = UINT32 (((esp_0 - 0x8) & 0xffffffff) /*0x22cc14*/);
								printf (/*"a > 5, b <= 5, c <= 5 -- case 4\n"*/ (const char *) (((m402018_0_32 & 0xffffff) | m402018_0_32) & 0xffffffff) /*0x4030e0*/);
								const UINT32 eax_1 = regs.eax, ebx_1 = regs.ebx, ecx_1 = regs.ecx, edx_1 = regs.edx, edi_1 = regs.edi, esi_1 = regs.esi, esp_1 = regs.esp, ebp_1 = regs.ebp;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7);
								{
									/*Memory Changes*/
									/*Registers Changes*/
									return int (regs.eax);
								}
							}
						}
					} else {
						/*Memory Changes*/
						*((UINT32 *) 0x22cbb0) = UINT32 (0x6fcd0000611b6708);
						*((UINT32 *) 0x22cbb8) = UINT32 (0x4060dd6fcd0000);
						*((UINT32 *) 0x22cbc0) = UINT32 (0x406042004060f1);
						*((UINT32 *) 0x22cbc8) = UINT32 (0x401ba002000000);
						*((UINT32 *) 0x22cbd0) = UINT32 (((((esp_0 - 0x8) << 0x20) & 0xffffffff00000000) | 0x401ba0) /*0x22cc1400401ba0*/);
						*((UINT32 *) 0x22cbd8) = UINT32 (0x22cc3800401455);
						*((UINT32 *) 0x22cbe0) = UINT32 ((((0x0 | (signExtend_0x20_0x8 (m22cca7_0_8) & 0xffffffff)) & 0xffffffff) | 0x3500000000) /*0x3500000040*/);
						*((UINT32 *) 0x22cbe8) = UINT32 (((((m402014_0_32 & 0xffff) | m402014_0_32) & 0xffffffff) << 0x20) /*0x4030c000000000*/);
						*((UINT32 *) 0x22cbf0) = UINT32 (0x611ccfaf00401ba0);
						*((UINT32 *) 0x22cbf8) = UINT32 ((((m402014_0_32 << 0x28) | 0xf) & 0xff00000f) /*0xf*/);
						*((UINT32 *) 0x22cc00) = UINT32 (0x3501006610);
						*((UINT32 *) 0x22cc08) = UINT32 ((((esi_0 & 0xffffffff) << 0x20) | 0x4060f6) /*0x22cca1004060f6*/);
						*((UINT32 *) 0x22cc10) = UINT32 ((((ebp_0 & 0xffffffff) << 0x20) | ((0x0 | ((0x0 | ((0x0 | ((0x0 | ((0x0 | (edi_0 & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) & 0xffffffff)) /*0x22cc48611ccfaf*/);
						*((UINT32 *) 0x22cc18) = UINT32 (0x22cc380040134b);
						*((UINT32 *) 0x22cc20) = UINT32 (0x611cd07e0022cc6c);
						*((UINT32 *) 0x22cc28) = UINT32 (0x22cc4c00000000);
						*((UINT32 *) 0x22cc30) = UINT32 (0x10000002f);
						*((UINT32 *) 0x22cc38) = UINT32 (0x22cc6c004060fe);
						*((UINT32 *) 0x406040) = UINT32 (0xffff0022cc500101);
						*((UINT32 *) 0x406048) = UINT32 (0x10000000202ffff);
						*((UINT32 *) 0x406050) = UINT32 (0x15020000000002);
						*((UINT32 *) 0x406058) = UINT32 (((((m402008_0_32 << 0x18) & 0xffffff00000000) | 0xff00000000000000) | (((m402008_0_32 << 0x18) & 0xffffffff) | 0x20000)) /*0xff00000000020000*/);
						*((UINT32 *) 0x406060) = UINT32 (0xffffff0200ffffff);
						*((UINT32 *) 0x406068) = UINT32 (0x1000000000200ff);
						*((UINT32 *) 0x406070) = UINT32 (((((m22cc70_0_32 << 0x8) & 0xff00000000) | 0xffffff0000000000) | (((m22cc70_0_32 << 0x8) & 0xffffffff) | 0x3)) /*0xffffff0022cca503*/);
						*((UINT32 *) 0x406078) = UINT32 (0x2030000003502ff);
						*((UINT32 *) 0x406080) = UINT32 (0x8a0200000000);
						*((UINT32 *) 0x406088) = UINT32 ((((((m22cc70_0_32 + 0x1) << 0x18) & 0xffffff00000000) | 0xff00000000000000) | ((((m22cc70_0_32 + 0x1) << 0x18) & 0xff000000) | 0x30100)) /*0xff0022cca6030100*/);
						*((UINT32 *) 0x406090) = UINT32 (0x3502ffffff);
						*((UINT32 *) 0x406098) = UINT32 (0x3802000000000203);
						*((UINT32 *) 0x4060a0) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
						*((UINT32 *) 0x4060a8) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x4060b0) = UINT32 (0x2030000);
						*((UINT32 *) 0x4060b8) = UINT32 ((((m40200c_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x3080020000000f02*/);
						*((UINT32 *) 0x4060c0) = UINT32 (((logicalShiftToRight (m40200c_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
						*((UINT32 *) 0x4060c8) = UINT32 ((((m402010_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030a00200000009*/);
						*((UINT32 *) 0x4060d0) = UINT32 (((logicalShiftToRight (m402010_0_32, 0x18) & 0xff) | 0x320203ffffffff00) /*0x320203ffffffff00*/);
						*((UINT32 *) 0x4060d8) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca70301000000*/);
						*((UINT32 *) 0x4060e0) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x4060e8) = UINT32 (0x2030000);
						*((UINT32 *) 0x4060f0) = UINT32 ((((m402014_0_32 << 0x30) & 0xffffffff00000000) | 0x20000000f02) /*0x30c0020000000f02*/);
						*((UINT32 *) 0x4060f8) = UINT32 (((logicalShiftToRight (m402014_0_32, 0x10) & 0xffff) | 0x203ffffffff0000) /*0x203ffffffff0040*/);
						*((UINT32 *) 0x406100) = UINT32 ((((m402018_0_32 << 0x28) & 0xffffffff00000000) | 0x200000009) /*0x4030e00200000009*/);
						*((UINT32 *) 0x406108) = UINT32 (((logicalShiftToRight (m402018_0_32, 0x18) & 0xff) | 0x840203ffffffff00) /*0x840203ffffffff00*/);
						*((UINT32 *) 0x406110) = UINT32 (((((m22cc70_0_32 + 0x1) << 0x28) & 0xffffff0000000000) | 0x301000000) /*0x22cca60301000000*/);
						*((UINT32 *) 0x406118) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x1), 0x18) & 0xff) | 0x3502ffffffff00) /*0x3502ffffffff00*/);
						*((UINT32 *) 0x406120) = UINT32 (0x2030000);
						*((UINT32 *) 0x406128) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003802) /*0xa703010000003802*/);
						*((UINT32 *) 0x406130) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
						*((UINT32 *) 0x406138) = UINT32 (0x20300000035);
						*((UINT32 *) 0x406140) = UINT32 (0x20000000f020000);
						*((UINT32 *) 0x406148) = UINT32 ((m40201c_0_32 | 0xffffffff00000000) /*0xffffffff00403104*/);
						*((UINT32 *) 0x406150) = UINT32 ((((m402020_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x2402000000090203*/);
						*((UINT32 *) 0x406158) = UINT32 (((logicalShiftToRight (m402020_0_32, 0x8) & 0xffffff) | 0x3ffffffff000000) /*0x3ffffffff004031*/);
						*((UINT32 *) 0x406160) = UINT32 (((((m22cc70_0_32 + 0x2) << 0x38) & 0xff00000000000000) | 0x3010000003202) /*0xa703010000003202*/);
						*((UINT32 *) 0x406168) = UINT32 (((logicalShiftToRight ((m22cc70_0_32 + 0x2), 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff0022cc*/);
						*((UINT32 *) 0x406170) = UINT32 (0x20300000035);
						*((UINT32 *) 0x406178) = UINT32 (0x20000000f020000);
						*((UINT32 *) 0x406180) = UINT32 ((m402024_0_32 | 0xffffffff00000000) /*0xffffffff00403148*/);
						*((UINT32 *) 0x406188) = UINT32 ((((m402028_0_32 << 0x38) & 0xffffffff00000000) | 0x2000000090203) /*0x6c02000000090203*/);
						*((UINT32 *) 0x406190) = UINT32 (((logicalShiftToRight (m402028_0_32, 0x8) & 0xffffff) | 0x2ffffffff000000) /*0x2ffffffff004031*/);
						*((UINT32 *) 0x406198) = UINT32 ((m40202c_0_32 | 0xffffffff00000000) /*0xffffffff00403190*/);
						*((UINT32 *) 0x4061a0) = UINT32 ((((m4061a4_0_32 & 0xff000000) << 0x20) | 0x200) /*0x200*/);
						*((UINT32 *) 0x4061a8) = UINT32 (m4061a8_0_64 /*0x0*/);
						/*Registers Changes*/
						regs.eax = UINT32 ((((m402014_0_32 & 0xffff) | m402014_0_32) & 0xffffffff) /*0x4030c0*/);
						regs.ecx = UINT32 (0x0);
						regs.edx = UINT32 (0x4060fe);
						regs.edi = UINT32 ((edi_0 & 0xffffffff) /*0x611ccfaf*/);
						regs.esi = UINT32 ((esi_0 & 0xffffffff) /*0x22cca1*/);
						regs.esp = UINT32 (((esp_0 - 0x30) & 0xffffffff) /*0x22cbec*/);
						regs.ebp = UINT32 (((esp_0 - 0x8) & 0xffffffff) /*0x22cc14*/);
						printf (/*"a > 5, b <= 5, c > 5 -- case 3\n"*/ (const char *) (((m402014_0_32 & 0xffff) | m402014_0_32) & 0xffffffff) /*0x4030c0*/);
						const UINT32 eax_1 = regs.eax, ebx_1 = regs.ebx, ecx_1 = regs.ecx, edx_1 = regs.edx, edi_1 = regs.edi, esi_1 = regs.esi, esp_1 = regs.esp, ebp_1 = regs.ebp;
						const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7);
						{
							/*Memory Changes*/
							/*Registers Changes*/
							return int (regs.eax);
						}
					}
				}
			}
		} else {
		}
	}
}
