#include "game/effects/flicker.h"

#include "game/control.h"
#include "game/sound.h"
#include "global/vars.h"

void Flicker(ITEM_INFO *item)
{
    if (g_FlipTimer > 125) {
        FlipMap();
        g_FlipEffect = -1;
    } else if (
        g_FlipTimer == 90 || g_FlipTimer == 92 || g_FlipTimer == 105
        || g_FlipTimer == 107) {
        FlipMap();
    }
    g_FlipTimer++;
}
