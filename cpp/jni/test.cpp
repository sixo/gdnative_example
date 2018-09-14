#define sysv_abi // get rid of some compiler warnings

#include <jni.h>
#include <gdnative_api_struct.gen.h>
#include <android/log.h>
#include <vector>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "gdnative", __VA_ARGS__))

extern "C" {
// Caled when NativeScript instance is created/destroyed 
void *android_gdnative_constructor(godot_object *p_instance, void *p_method_data);
void android_gdnative_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);

// Our custom function
godot_variant android_gdnative_test(godot_object *p_instance, void *p_method_data
    , void *p_user_data, int p_num_args, godot_variant **p_args);

// Gives access to various functions from Godot's api
const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;

// Called when the native library is loaded
// Here we initialized the previous 2 structs
void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) 
{
    api = p_options->api_struct;
    LOGI("godot_gdnative_init");

    // now find our extensions
    for (int i = 0; i < api->num_extensions; i++) 
    {
        // LOGI("api type=%d", api->extensions[i]->type); 
        switch (api->extensions[i]->type) 
        {
            case GDNATIVE_EXT_NATIVESCRIPT: 
            {                
                nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i];
            }; 
            break;
            default: break;
        }
    } 
}

// Called when native lib is unloaded
void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) 
{
    api = NULL;
    nativescript_api = NULL;
    LOGI("godot_gdnative_terminate");
}

// Used to register our custom classes and methods
void GDN_EXPORT godot_nativescript_init(void *p_handle) 
{
    LOGI("godot_nativescript_init");
    
    godot_instance_create_func create = { NULL, NULL, NULL };
    create.create_func = &android_gdnative_constructor;

    godot_instance_destroy_func destroy = { NULL, NULL, NULL };
    destroy.destroy_func = &android_gdnative_destructor;

    nativescript_api->godot_nativescript_register_class(p_handle, "AndroidGDNative", "Node2D",
        create, destroy);

    godot_instance_method test = { NULL, NULL, NULL };
    test.method = &android_gdnative_test;

    godot_method_attributes attributes = { GODOT_METHOD_RPC_MODE_DISABLED };

    nativescript_api->godot_nativescript_register_method(p_handle, "AndroidGDNative", "test",
        attributes, test);
}

// Our custom data that can be passed automatically between our custom functions,
// constructor, and destructor
struct UserData {
    char data[256];
};

// Called after creating a new instance of our registered AndroidGDNative class in GDScript
// Once our class is loaded (load()/preload), calling new() will cause
// this method to execute
void *android_gdnative_constructor(godot_object *p_instance, void *p_method_data)
{
    LOGI("android_gdnative_constructor");
    return api->godot_alloc(sizeof(UserData));
}

void android_gdnative_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data)
{
    LOGI("android_gdnative_destructor");
    api->godot_free(p_user_data);
}

// Passes string from native code to GDScript
godot_variant android_gdnative_test(godot_object *p_instance, void *p_method_data
    , void *p_user_data, int p_num_args, godot_variant **p_args)
{ 
    // Pass message to gdscript
    godot_variant ret;    
    godot_string gs = api->godot_string_chars_to_utf8("Hello from GDNative");    
    api->godot_variant_new_string(&ret, &gs);    
    api->godot_string_destroy(&gs);
    
    return ret;
}
}
