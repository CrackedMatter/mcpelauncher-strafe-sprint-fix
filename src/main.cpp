#define LOG_TAG "StrafeSprintFix"
#include <android/log_macros.h>
#include <libhat/process.hpp>
#include <libhat/scanner.hpp>

extern "C" [[gnu::visibility("default")]] void mod_preinit() {}

extern "C" [[gnu::visibility("default")]] void mod_init() {
    using namespace hat::signature_literals;

    auto mcLib = hat::process::get_module("libminecraftpe.so").value();

    auto value = [range = mcLib.get_executable_data()] -> float* {
        auto sigs = std::to_array<std::pair<hat::signature_view, std::size_t>>({
            {"0F 2E 1D ? ? ? ? B3 01"_sigv, 3},
            {"F3 0F 10 05 ? ? ? ? 41 0F 2E ? ? 76 ? EB"_sigv, 4},
            {"F3 0F 10 05 ? ? ? ? F3 0F 59 E0 0F 28 EA"_sigv, 4},
        });

        for (auto [sig, offset] : sigs) {
            auto result = hat::find_pattern(range, sig);

            if (result.has_result()) {
                ALOGD("Found pattern '%s' at %p", to_string(sig).c_str(), result.get());
                return reinterpret_cast<float*>(result.rel(offset));
            }
        }

        return nullptr;
    }();

    if (value == nullptr) {
        ALOGE("No pattern found!");
        return;
    }

    if (*value != 0.70710677f) {
        if (*value == 0.7071068f) {
            ALOGI("Patch already applied (address: %p)", value);
        } else {
            ALOGE("Wrong value! Expected 0.70710677, got %.8f (address: %p)", *value, value);
        }
        return;
    }

    *value = 0.7071068f;
    ALOGI("Patch applied successfully (address: %p)", value);
}
