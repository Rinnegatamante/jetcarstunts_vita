/*
 * main.c
 *
 * ARMv7 Shared Libraries loader. Jet Car Stunts Edition
 *
 * Copyright (C) 2021 Andy Nguyen
 * Copyright (C) 2021-2023 Rinnegatamante
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "utils/init.h"
#include "utils/glutil.h"
#include "utils/settings.h"
#include "utils/logger.h"

#include <psp2/kernel/threadmgr.h>

#include <falso_jni/FalsoJNI.h>
#include <so_util/so_util.h>

#include <AFakeNative/AFakeNative.h>
#include <vitasdk.h>
#include <stdio.h>

int _newlib_heap_size_user = 256 * 1024 * 1024;

#ifdef USE_SCELIBC_IO
int sceLibcHeapSize = 24 * 1024 * 1024;
#endif

so_module so_mod;

extern void prepare_jni_dyn_fields();

static const char skybox_frag[] =
"uniform lowp samplerCube tex;\n"
"varying mediump vec3 v_v3TexCoord;\n"
"void main() {\n"
"	gl_FragColor = textureCube(tex, v_v3TexCoord);\n"
"	gl_FragDepth = 1.0f;\n"
"}\n";

extern void kill_sles();

int main() {	
	SceAppUtilInitParam appUtilParam;
	SceAppUtilBootParam appUtilBootParam;
	memset(&appUtilParam, 0, sizeof(SceAppUtilInitParam));
	memset(&appUtilBootParam, 0, sizeof(SceAppUtilBootParam));
	sceAppUtilInit(&appUtilParam, &appUtilBootParam);
	
	SceAppUtilAppEventParam eventParam;
	memset(&eventParam, 0, sizeof(SceAppUtilAppEventParam));
	sceAppUtilReceiveAppEvent(&eventParam);
	if (eventParam.type == 0x05) { // Game launched with no audio mode
		kill_sles();
	}
	
	// Patch skybox shader
	FILE *f = fopen("ux0:data/jetcarstunts/assets/shaders/xyz_uvw.frag", "rb");
	if (f) {
		void *buf = malloc(1024 * 1024);
		fread(buf, 1, buf, f);
		fclose(f);
		if (!strstr(buf, "gl_FragDepth")) {
			f = fopen("ux0:data/jetcarstunts/assets/shaders/xyz_uvw.frag", "wb");
			fwrite(skybox_frag, 1, strlen(skybox_frag), f);
			fclose(f);
		}
		free(buf);
	}
	
	soloader_init_all();
	
	prepare_jni_dyn_fields();

	int (*ANativeActivity_onCreate)(ANativeActivity *activity, void *savedState,
									size_t savedStateSize) = (void *) so_symbol(&so_mod, "ANativeActivity_onCreate");
	
	ANativeActivity *activity = ANativeActivity_create();
	log_info("Created NativeActivity object");

	ANativeActivity_onCreate(activity, NULL, 0);
	log_info("ANativeActivity_onCreate() passed");
	
	void (*SetOSScreenSize)(JNIEnv * env, jclass clazz, int width, int height) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_SetOSScreenSize");
	SetOSScreenSize(&jni, 0x0, 960, 544);
	
	void (*SetIsAboveIceCreamSandwich)(JNIEnv *env, jclass clazz) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_SetIsAboveIceCreamSandwich");
	SetIsAboveIceCreamSandwich(&jni, 0x0);
	
	void (*SetSyncInfo)(JNIEnv *env, jclass clazz, float rate, int vsync) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_SetSyncInfo");
	SetSyncInfo(&jni, 0x0, 60.0f, 0);
	
	void (*SetDeviceRotation)(JNIEnv *env, jclass clazz, int rotation) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_SetDeviceRotation");
	SetDeviceRotation(&jni, 0x0, 1);
	
	void (*SetMemoryLimits)(JNIEnv *env, jclass clazz, int rotation) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_SetMemoryLimits");
	SetMemoryLimits(&jni, 0x0, 0);
	
	void (*InitLowLatencySound)(JNIEnv *env, jclass clazz, int low_latency, int samplerate, int bufsize) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_initLowLatencySound");
	InitLowLatencySound(&jni, 0x0, 1, 44100, 1024);
	
	void (*DeviceTweaks)(JNIEnv *env, jclass clazz, int rgb888_framebuffer, int no_java_sound) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_DeviceTweaks");
	DeviceTweaks(&jni, 0x0, 1, 1);
	
	void (*JaypadIsSupported)(JNIEnv *env, jclass clazz) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_JaypadIsSupported");
	JaypadIsSupported(&jni, 0x0);
	
	void (*StopJoypad)(JNIEnv *env, jclass clazz) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_StopJoypad");
	StopJoypad(&jni, 0x0);
	
	void (*StartJoypad)(JNIEnv *env, jclass clazz, int type) = (void *)so_symbol(&so_mod, "Java_com_trueaxis_cLib_TrueaxisLib_StartJoypad");
	StartJoypad(&jni, 0x0, 1);
	
	activity->callbacks->onStart(activity);
	log_info("onStart() passed");

	AInputQueue *aInputQueue = AInputQueue_create();
	activity->callbacks->onInputQueueCreated(activity, aInputQueue);
	log_info("onInputQueueCreated() passed");

	ANativeWindow *aNativeWindow = ANativeWindow_create();
	activity->callbacks->onNativeWindowCreated(activity, aNativeWindow);
	log_info("onNativeWindowCreated() passed");

	activity->callbacks->onWindowFocusChanged(activity, 1);
	log_info("onWindowFocusChanged() passed");

	log_info("Main thread shutting down");
	
	// Disabling dynamic cubemaps
	*(int *)so_symbol(&so_mod, "g_bDisableDynamicCubeMapping") = 1;

	sceKernelExitDeleteThread(0);
}
