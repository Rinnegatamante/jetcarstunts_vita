#include <falso_jni/FalsoJNI_Impl.h>
#include <falso_jni/FalsoJNI_Logger.h>
#include <string.h>
#include <vitasdk.h>
#include <AFakeNative/keycodes.h>

// System-wide constant that's often used to determine Android version
// https://developer.android.com/reference/android/os/Build.VERSION.html#SDK_INT
// Possible values: https://developer.android.com/reference/android/os/Build.VERSION_CODES
const int SDK_INT = 19; // Android 4.4 / KitKat

/*
 * JNI Methods
*/

JavaDynArray gamepadAxisIndices, gamepadAxisMinVals, gamepadAxisMaxVals, gamepadButtonIndices;
  
void prepare_jni_dyn_fields() {
	jda_alloc_static(&gamepadAxisIndices, 4, FIELD_TYPE_INT);
	int *arr = gamepadAxisIndices.array;
	arr[0] = 0;
	arr[1] = 1;
	arr[2] = 2;
	arr[3] = 3;
	
	jda_alloc_static(&gamepadAxisMinVals, 4, FIELD_TYPE_FLOAT);
	float *arr2 = gamepadAxisMinVals.array;
	arr2[0] = -1.0f;
	arr2[1] = -1.0f;
	arr2[2] = -1.0f;
	arr2[3] = -1.0f;
	
	jda_alloc_static(&gamepadAxisMaxVals, 4, FIELD_TYPE_FLOAT);
	arr2 = gamepadAxisMaxVals.array;
	arr2[0] = 1.0f;
	arr2[1] = 1.0f;
	arr2[2] = 1.0f;
	arr2[3] = 1.0f;
	
	jda_alloc_static(&gamepadButtonIndices, 12, FIELD_TYPE_INT);
	arr = gamepadButtonIndices.array;
	arr[0] = AKEYCODE_DPAD_UP;
	arr[1] = AKEYCODE_DPAD_DOWN;
	arr[2] = AKEYCODE_DPAD_LEFT;
	arr[3] = AKEYCODE_DPAD_RIGHT;
	arr[4] = AKEYCODE_BUTTON_A;
	arr[5] = AKEYCODE_BUTTON_B;
	arr[6] = AKEYCODE_BUTTON_X;
	arr[7] = AKEYCODE_BUTTON_Y;
	arr[8] = AKEYCODE_BUTTON_L1;
	arr[9] = AKEYCODE_BUTTON_R1;
	arr[10] = AKEYCODE_BUTTON_START;
	arr[11] = AKEYCODE_BUTTON_SELECT;
}

NameToMethodID nameToMethodId[] = {
	{ 1, "getExternalFilesDir", METHOD_TYPE_OBJECT },
	{ 2, "getPath", METHOD_TYPE_OBJECT },
	{ 3, "isTablet", METHOD_TYPE_INT },
	{ 4, "getPackageCodePath", METHOD_TYPE_OBJECT },
	{ 5, "TaServer_InitialisePlatform", METHOD_TYPE_VOID },
	{ 6, "haveNetworkConnection", METHOD_TYPE_BOOLEAN },
};

jobject getExternalFilesDir(jmethodID id, va_list args) {
    return strdup("ux0:data/jetcarstunts/assets");
}

jobject getPackageCodePath(jmethodID id, va_list args) {
    return strdup("ux0:data/jetcarstunts/assets");
}

jobject getPath(jmethodID id, va_list args) {
    return strdup("ux0:data/jetcarstunts/assets");
}

jint isTablet(jmethodID id, va_list args) {
	return 0;
}

void dummy(jmethodID id, va_list args) {
	fjni_log_info("dummy");
}

jboolean haveNetworkConnection(jmethodID id, va_list args) {
    return JNI_FALSE;
}

MethodsBoolean methodsBoolean[] = {
	{ 3, haveNetworkConnection }, 
};

MethodsByte methodsByte[] = {};
MethodsChar methodsChar[] = {};
MethodsDouble methodsDouble[] = {};
MethodsFloat methodsFloat[] = {};
MethodsLong methodsLong[] = {};
MethodsShort methodsShort[] = {};

MethodsInt methodsInt[] = {
	{ 3, isTablet }, 
};

MethodsObject methodsObject[] = {
	{ 1, getExternalFilesDir }, 
	{ 2, getPath },
	{ 4, getPackageCodePath }, 
};

MethodsVoid methodsVoid[] = {
	{ 5, dummy }, 
};

/*
 * JNI Fields
*/

// System-wide constant that applications sometimes request
// https://developer.android.com/reference/android/content/Context.html#WINDOW_SERVICE
char WINDOW_SERVICE[] = "window";

NameToFieldID nameToFieldId[] = {
	{ 0, "WINDOW_SERVICE", FIELD_TYPE_OBJECT }, 
	{ 1, "SDK_INT", FIELD_TYPE_INT },
	{ 2, "gamepadAxisIndices", FIELD_TYPE_OBJECT },
	{ 3, "gamepadAxisMinVals", FIELD_TYPE_OBJECT },
	{ 4, "gamepadAxisMaxVals", FIELD_TYPE_OBJECT },
	{ 5, "gamepadButtonIndices", FIELD_TYPE_OBJECT },
};

FieldsBoolean fieldsBoolean[] = {};
FieldsByte fieldsByte[] = {};
FieldsChar fieldsChar[] = {};
FieldsDouble fieldsDouble[] = {};
FieldsFloat fieldsFloat[] = {};
FieldsInt fieldsInt[] = {
	{ 1, SDK_INT },
};
FieldsObject fieldsObject[] = {
	{ 0, WINDOW_SERVICE },
	{ 2, &gamepadAxisIndices },
	{ 3, &gamepadAxisMinVals },
	{ 4, &gamepadAxisMaxVals },
	{ 5, &gamepadButtonIndices },
};
FieldsLong fieldsLong[] = {};
FieldsShort fieldsShort[] = {};

__FALSOJNI_IMPL_CONTAINER_SIZES
