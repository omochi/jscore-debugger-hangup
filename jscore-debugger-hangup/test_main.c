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

JSObjectRef CreateFunctionFromClass(JSContextRef context) {
    JSClassDefinition defn = kJSClassDefinitionEmpty;
    JSStaticFunction static_functions[] = {
        { "f", &TestNativeFunc, 0 },
        { 0, 0, 0 }
    };
    defn.staticFunctions = static_functions;
    JSClassRef klass = JSClassCreate(&defn);
    JSObjectRef obj = JSObjectMake(context, klass, NULL);
    JSValueProtect(context, obj);
    JSStringRef f_str = JSStringCreateWithUTF8CString("f");
    
    JSObjectRef f = JSValueToObject(context, JSObjectGetProperty(context, obj, f_str, NULL), NULL);
    JSValueProtect(context, f);
    
    JSStringRelease(f_str);
    JSValueUnprotect(context, obj);
    JSClassRelease(klass);
    
    return f;
}

void TestMain() {
    JSGlobalContextRef context = JSGlobalContextCreate(NULL);
    
    JSObjectRef func = JSObjectMakeFunctionWithCallback(context, NULL, &TestNativeFunc);
    JSValueProtect(context, func);

//    JSObjectRef func = CreateFunctionFromClass(context);
    
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