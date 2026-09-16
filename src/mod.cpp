#include "d/actor/d_a_alink.h"
#include "mods/service.hpp"
#include "mods/svc/hook.h"
#include "mods/svc/hook.hpp"

DEFINE_MOD();
IMPORT_SERVICE(HookService, svc_hook);

DEFINE_HOOK(&daAlink_c::procFrontRollInit, LinkFrontRollAction);
DEFINE_HOOK(&daAlink_c::procCutJumpInit, LinkCutJumpAction);

extern "C" {

HookAction link_front_roll_pre(ModContext* ctx, void* args, void* retval, void*) {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link && link->checkAttentionLock()) {
        int cutDir = link->getCutDirection();
        if (cutDir == link->DIR_FORWARD) {
            link->procAutoJumpInit(1);

            if (retval != nullptr) {
                *static_cast<int*>(retval) = 1;
            }

            return HOOK_SKIP_ORIGINAL;
        }
    }
    return HOOK_CONTINUE;
}

HookAction link_cut_jump_pre(ModContext* ctx, void* args, void* retval, void*) {
    daAlink_c* link = daAlink_getAlinkActorClass();
    BOOL& isAir = mods::arg_ref<BOOL>(args, 1);
    if (link && !isAir && link->checkAttentionLock()) {
        int cutDir = link->getCutDirection();
        if (cutDir == link->DIR_FORWARD) {
            link->procAutoJumpInit(1);

            if (retval != nullptr) {
                *static_cast<int*>(retval) = 1;
            }

            return HOOK_SKIP_ORIGINAL;
        }
    }
    return HOOK_CONTINUE;
}

MOD_EXPORT ModResult mod_initialize(ModError*) {
    mods::hook::add_pre<LinkFrontRollAction>(link_front_roll_pre);
    mods::hook::add_pre<LinkCutJumpAction>(link_cut_jump_pre);
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    return MOD_OK;
}
}
