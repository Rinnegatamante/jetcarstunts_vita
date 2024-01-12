/*
 * patch.c
 *
 * Patching some of the .so internal functions or bridging them to native for
 * better compatibility.
 *
 * Copyright (C) 2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "patch.h"

#include <kubridge.h>
#include <so_util/so_util.h>
#include <stdio.h>
#include <vitasdk.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
extern so_module so_mod;
#ifdef __cplusplus
};
#endif

#include "utils/logger.h"
#include <stdbool.h>

int ret0() { return 0; }
int ret1() { return 1; }

void CopyStats() { 
	FILE *src = fopen("ux0:data/jetcarstunts/assets/stats.bin", "rb");
	if (src) {
		FILE *dst = fopen("ux0:data/jetcarstunts/assets/statsbak.bin", "wb");
		fseek(src, 0, SEEK_END);
		size_t size = ftell(src);
		void *buf = malloc(size);
		fseek(src, 0, SEEK_SET);
		fread(buf, 1, size, src);
		fclose(src);
		fwrite(buf, 1, size, dst);
		fclose(dst);
	}
}

void so_patch(void) {
	hook_addr((uintptr_t)so_symbol(&so_mod, "zip_fopen"), (uintptr_t)&ret0);
	hook_addr((uintptr_t)so_symbol(&so_mod, "_ZN4Game9CopyStatsEv"), (uintptr_t)&CopyStats);
	hook_addr((uintptr_t)so_symbol(&so_mod, "_Z20Store_IsItemDlcTherePKc"), (uintptr_t)&ret1);
	hook_addr((uintptr_t)so_symbol(&so_mod, "_Z23Store_IsItemDlcRequiredPKc"), (uintptr_t)&ret0);
	hook_addr((uintptr_t)so_symbol(&so_mod, "_Z17Store_IsItemLocalP9StoreItem"), (uintptr_t)&ret1);
	hook_addr((uintptr_t)so_symbol(&so_mod, "_Z27OnStoreIsDlcLevelDownloadedi"), (uintptr_t)&ret1);
	hook_addr((uintptr_t)so_symbol(&so_mod, "_Z10IAPUnlocksN5Level4PackE"), (uintptr_t)&ret1);	
}
