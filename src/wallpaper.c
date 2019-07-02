#include <windows.h>
#include <node_api.h>

napi_value setWallpaper(napi_env env, napi_callback_info info) {
    napi_status status;
    size_t argc = 1;
    char16_t* fullPath;
    napi_value argv[1];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to parse arguments");
    }

    status = napi_get_value_int32(env, argv[0], &fullPath, sizeof(&fullPath));

    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Invalid path was passed as argument");
    }

    if (!SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, &fullPath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        napi_throw_error(env, NULL, "Failed to set desktop wallpaper");
    }
    return NULL;
}


napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, NULL, 0, setWallpaper, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap native function");
  }

  status = napi_set_named_property(env, exports, "setWallpaper", fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate exports");
  }

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)