#define LOG_TAG "StrafeSprintFix"
#include <android/log_macros.h>
#include <dlfcn.h>
#include <libhat/scanner.hpp>
#include <link.h>

extern "C" [[gnu::visibility("default")]] void mod_preinit() {}

extern "C" [[gnu::visibility("default")]] void mod_init() {
    using namespace hat::literals::signature_literals;

    auto mcLib = dlopen("libminecraftpe.so", 0);

    std::span<std::byte> range;

    auto callback = [&](const dl_phdr_info& info) {
        if (auto h = dlopen(info.dlpi_name, RTLD_NOLOAD); dlclose(h), h != mcLib)
            return 0;
        range = {reinterpret_cast<std::byte*>(info.dlpi_addr + info.dlpi_phdr[1].p_vaddr), info.dlpi_phdr[1].p_memsz};
        return 1;
    };

    dl_iterate_phdr(
        [](dl_phdr_info* info, size_t, void* data) {
            return (*static_cast<decltype(callback)*>(data))(*info);
        },
        &callback);

    auto result = hat::find_pattern(range, "F3 0F 10 05 ? ? ? ? F3 0F 59 E0 0F 28 EA"_sig);

    if (!result.has_result()) {
        ALOGE("Pattern not found!");
        return;
    }

    auto value = reinterpret_cast<float*>(result.rel(4));

    if (*value != 0.70710677f) {
        ALOGE("Wrong value! Expected 0.70710677, got %.8f (address: %p)", *value, value);
        return;
    }

    *value = 0.7071068f;
    ALOGI("Patch applied successfully (address: %p)", value);
}
