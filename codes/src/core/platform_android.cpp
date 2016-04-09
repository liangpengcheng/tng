#include "core/arch_detect.h"
#ifdef TNG_ANDROID
#include "core/os.h"
#include "core/runtime_check.h"

namespace tng{

	//copy from gameplay
	void OS::ShowKeyboard(bool show)
	{
		// The following functions is supposed to show / hide functins from a native activity.. but currently do not work. 
		// ANativeActivity_showSoftInput(state__->activity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT);
		// ANativeActivity_hideSoftInput(state__->activity, ANATIVEACTIVITY_HIDE_SOFT_INPUT_IMPLICIT_ONLY);

		Assert(state__ && state__->activity && state__->activity->vm);

		// Show or hide the keyboard by calling the appropriate Java method through JNI instead.
		jint flags = 0;
		JavaVM* jvm = state__->activity->vm;
		JNIEnv* env = NULL;
		jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
		jint result = jvm->AttachCurrentThread(&env, NULL);
		if (result == JNI_ERR)
		{
			Log::GetLog()->Printf(Log::ERROR_CHN,"Failed to retrieve JVM environment to display keyboard.");
			return;
		}
		Assert(env);

		// Retrieves NativeActivity. 
		jobject lNativeActivity = state__->activity->clazz;
		jclass ClassNativeActivity = env->GetObjectClass(lNativeActivity);

		// Retrieves Context.INPUT_METHOD_SERVICE.
		jclass ClassContext = env->FindClass("android/content/Context");
		jfieldID FieldINPUT_METHOD_SERVICE = env->GetStaticFieldID(ClassContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
		jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(ClassContext, FieldINPUT_METHOD_SERVICE);

		// Runs getSystemService(Context.INPUT_METHOD_SERVICE).
		jclass ClassInputMethodManager = env->FindClass("android/view/inputmethod/InputMethodManager");
		jmethodID MethodGetSystemService = env->GetMethodID(ClassNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
		jobject lInputMethodManager = env->CallObjectMethod(lNativeActivity, MethodGetSystemService, INPUT_METHOD_SERVICE);

		// Runs getWindow().getDecorView().
		jmethodID MethodGetWindow = env->GetMethodID(ClassNativeActivity, "getWindow", "()Landroid/view/Window;");
		jobject lWindow = env->CallObjectMethod(lNativeActivity, MethodGetWindow);
		jclass ClassWindow = env->FindClass("android/view/Window");
		jmethodID MethodGetDecorView = env->GetMethodID(ClassWindow, "getDecorView", "()Landroid/view/View;");
		jobject lDecorView = env->CallObjectMethod(lWindow, MethodGetDecorView);
		if (show)
		{
			// Runs lInputMethodManager.showSoftInput(...).
			jmethodID MethodShowSoftInput = env->GetMethodID(ClassInputMethodManager, "showSoftInput", "(Landroid/view/View;I)Z");
			jboolean result = env->CallBooleanMethod(lInputMethodManager, MethodShowSoftInput, lDecorView, flags);
		}
		else
		{
			// Runs lWindow.getViewToken() 
			jclass ClassView = env->FindClass("android/view/View");
			jmethodID MethodGetWindowToken = env->GetMethodID(ClassView, "getWindowToken", "()Landroid/os/IBinder;");
			jobject lBinder = env->CallObjectMethod(lDecorView, MethodGetWindowToken);

			// lInputMethodManager.hideSoftInput(...). 
			jmethodID MethodHideSoftInput = env->GetMethodID(ClassInputMethodManager, "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z");
			jboolean lRes = env->CallBooleanMethod(lInputMethodManager, MethodHideSoftInput, lBinder, flags);
		}

		// Finished with the JVM.
		jvm->DetachCurrentThread();
	}
	//copy from gameplay
	bool OS::LaunchURL(const char *url)
	{
		if (url == NULL || *url == '\0')
			return false;

		bool result = true;

		android_app* state = state__;
		Assert(state && state->activity && state->activity->vm);
		JavaVM* jvm = state->activity->vm;
		JNIEnv* env = NULL;
		jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
		jint r = jvm->AttachCurrentThread(&env, NULL);
		if (r == JNI_ERR)
		{
			Log::GetLog()->Printf(Log::ERROR_CHN,"Failed to retrieve JVM environment to display keyboard.");
			return false;
		}
		Assert(env);

		jclass classActivity = env->FindClass("android/app/NativeActivity");
		jclass classIntent = env->FindClass("android/content/Intent");
		jclass classUri = env->FindClass("android/net/Uri");

		Assert(classActivity && classIntent && classUri);

		// Get static field ID Intent.ACTION_VIEW
		jfieldID fieldActionView = env->GetStaticFieldID(classIntent, "ACTION_VIEW", "Ljava/lang/String;");
		Assert(fieldActionView);

		// Get string value of Intent.ACTION_VIEW, we'll need that to pass to Intent's constructor later on
		jstring paramActionView = (jstring)env->GetStaticObjectField(classIntent, fieldActionView);
		Assert(paramActionView);

		// Get method ID Uri.parse, will be needed to parse the url given into Uri object
		jmethodID methodUriParse = env->GetStaticMethodID(classUri, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
		Assert(methodUriParse);

		// Get method ID Activity.startActivity, so we can start the appropriate activity for the View action of our Uri
		jmethodID methodActivityStartActivity = env->GetMethodID(classActivity, "startActivity", "(Landroid/content/Intent;)V");
		Assert(methodActivityStartActivity);

		// Get method ID Intent constructor, the one that takes action and uri (String;Uri)
		jmethodID methodIntentInit = env->GetMethodID(classIntent, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
		Assert(methodIntentInit);

		// Convert our url to Java's string and parse it to Uri
		jstring paramUrlString = env->NewStringUTF(url);
		jobject paramUri = env->CallStaticObjectMethod(classUri, methodUriParse, paramUrlString);
		Assert(paramUri);

		// Create Intent with Intent.ACTION_VIEW and parsed Uri arguments
		jobject paramIntent = env->NewObject(classIntent, methodIntentInit, paramActionView, paramUri);
		Assert(paramIntent);

		// Launch NativeActivity.startActivity with our intent to view the url! state->activity->clazz holds
		// our NativeActivity object
		env->CallVoidMethod(state->activity->clazz, methodActivityStartActivity, paramIntent);

		/* startActivity may throw a ActivitNotFoundException if, well, activity is not found.
		Example: http://<url> is passed to the intent but there is no browser installed in the system
		we need to handle it. */
		jobject exception = env->ExceptionOccurred();

		// We're not lucky here
		if (exception)
		{
			// Print out the exception data to logcat
			env->ExceptionDescribe();

			// Exception needs to be cleared
			env->ExceptionClear();

			// Launching the url failed
			result = false;
		}

		// See you Space Cowboy
		jvm->DetachCurrentThread();
		return result;
	}


}
#endif // TNG_ANDROID
