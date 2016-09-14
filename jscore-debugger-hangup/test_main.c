//
//  test_main.c
//  jscore-debugger-hangup
//
//  Created by omochimetaru on 2016/09/14.
//  Copyright © 2016年 omochimetaru. All rights reserved.
//

#include "./test_main.h"

static JSValueRef TestNativeFunc(JSContextRef ctx,
                                 JSObjectRef function,
                                 JSObjectRef thisObject,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef* exception)
{
    printf("func\n");
    return JSValueMakeNull(ctx);
}

void TestMain() {
    JSGlobalContextRef context = JSGlobalContextCreate(NULL);
    
    JSObjectRef func = JSObjectMakeFunctionWithCallback(context, NULL, &TestNativeFunc);
    JSValueProtect(context, func);
    
    JSObjectCallAsFunction(context, func, NULL, 0, NULL, NULL);

    JSObjectRef global_object = JSContextGetGlobalObject(context);

    JSStringRef f_str = JSStringCreateWithUTF8CString("f");
    
    JSObjectSetProperty(context, global_object, f_str, func, 0, NULL);
    
    JSStringRef script = JSStringCreateWithUTF8CString("f();");
    JSEvaluateScript(context, script, NULL, NULL, 1, NULL);
    
    JSStringRelease(f_str);
    
    JSValueUnprotect(context, func);
    JSGlobalContextRelease(context);
}