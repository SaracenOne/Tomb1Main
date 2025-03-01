#include "game/objects/pickup.h"

#include "config.h"
#include "game/collide.h"
#include "game/control.h"
#include "game/draw.h"
#include "game/input.h"
#include "game/inv.h"
#include "game/items.h"
#include "game/lara.h"
#include "game/overlay.h"
#include "global/vars.h"

static PHD_VECTOR m_PickUpPosition = { 0, 0, -100 };
static PHD_VECTOR m_PickUpPositionUW = { 0, -200, -350 };

static int16_t m_PickUpBounds[12] = {
    -256, +256, -100, +100, -256, +256, -10 * PHD_DEGREE, +10 * PHD_DEGREE,
    0,    0,    0,    0,
};

static int16_t m_PickUpBoundsControlled[12] = {
    -256, +256, -200, +200, -256, +256, -10 * PHD_DEGREE, +10 * PHD_DEGREE,
    0,    0,    0,    0,
};

static int16_t m_PickUpBoundsUW[12] = {
    -512,
    +512,
    -512,
    +512,
    -512,
    +512,
    -45 * PHD_DEGREE,
    +45 * PHD_DEGREE,
    -45 * PHD_DEGREE,
    +45 * PHD_DEGREE,
    -45 * PHD_DEGREE,
    +45 * PHD_DEGREE,
};

static void PickUp_GetItem(
    int16_t item_num, ITEM_INFO *item, ITEM_INFO *lara_item);

static void PickUp_GetItem(
    int16_t item_num, ITEM_INFO *item, ITEM_INFO *lara_item)
{
    if (item->object_number == O_SHOTGUN_ITEM) {
        g_Lara.mesh_ptrs[LM_TORSO] =
            g_Meshes[g_Objects[O_SHOTGUN].mesh_index + LM_TORSO];
    }
    Overlay_AddPickup(item->object_number);
    Inv_AddItem(item->object_number);
    item->status = IS_INVISIBLE;
    RemoveDrawnItem(item_num);
    g_GameInfo.stats.pickup_count++;
    g_Lara.interact_target.is_moving = false;
}

void SetupPickupObject(OBJECT_INFO *obj)
{
    obj->draw_routine = DrawPickupItem;
    obj->collision = PickUpCollision;
    obj->save_flags = 1;
}

void PickUpCollision(int16_t item_num, ITEM_INFO *lara_item, COLL_INFO *coll)
{
    if (g_Config.walk_to_items) {
        PickUpCollisionControlled(item_num, lara_item, coll);
        return;
    }

    ITEM_INFO *item = &g_Items[item_num];
    item->pos.y_rot = lara_item->pos.y_rot;
    item->pos.z_rot = 0;

    if (g_Lara.water_status == LWS_ABOVEWATER) {
        item->pos.x_rot = 0;
        if (!TestLaraPosition(m_PickUpBounds, item, lara_item)) {
            return;
        }

        if (lara_item->current_anim_state == AS_PICKUP) {
            if (lara_item->frame_number != AF_PICKUP_ERASE) {
                return;
            }
            PickUp_GetItem(item_num, item, lara_item);
            return;
        }

        if (g_Input.action && g_Lara.gun_status == LGS_ARMLESS
            && !lara_item->gravity_status
            && lara_item->current_anim_state == AS_STOP) {
            AlignLaraPosition(&m_PickUpPosition, item, lara_item);
            AnimateLaraUntil(lara_item, AS_PICKUP);
            lara_item->goal_anim_state = AS_STOP;
            g_Lara.gun_status = LGS_HANDSBUSY;
            return;
        }
    } else if (g_Lara.water_status == LWS_UNDERWATER) {
        item->pos.x_rot = -25 * PHD_DEGREE;
        if (!TestLaraPosition(m_PickUpBoundsUW, item, lara_item)) {
            return;
        }

        if (lara_item->current_anim_state == AS_PICKUP) {
            if (lara_item->frame_number != AF_PICKUP_UW) {
                return;
            }
            PickUp_GetItem(item_num, item, lara_item);
            return;
        }

        if (g_Input.action && lara_item->current_anim_state == AS_TREAD) {
            if (!MoveLaraPosition(&m_PickUpPositionUW, item, lara_item)) {
                return;
            }
            AnimateLaraUntil(lara_item, AS_PICKUP);
            lara_item->goal_anim_state = AS_TREAD;
        }
    }
}

void PickUpCollisionControlled(
    int16_t item_num, ITEM_INFO *lara_item, COLL_INFO *coll)
{
    ITEM_INFO *item = &g_Items[item_num];

    if (item->status == IS_INVISIBLE) {
        return;
    }

    bool have_item = false;
    int16_t rotx = item->pos.x_rot;
    int16_t roty = item->pos.y_rot;
    int16_t rotz = item->pos.z_rot;
    item->pos.y_rot = lara_item->pos.y_rot;
    item->pos.z_rot = 0;

    if (g_Lara.water_status == LWS_ABOVEWATER) {
        if ((g_Input.action && g_Lara.gun_status == LGS_ARMLESS
             && !lara_item->gravity_status
             && lara_item->current_anim_state == AS_STOP
             && !g_Lara.interact_target.is_moving)
            || (g_Lara.interact_target.is_moving
                && g_Lara.interact_target.item_num == item_num)) {

            have_item = false;
            item->pos.x_rot = 0;

            if (TestLaraPosition(m_PickUpBoundsControlled, item, lara_item)) {
                m_PickUpPosition.y = lara_item->pos.y - item->pos.y;
                if (MoveLaraPosition(&m_PickUpPosition, item, lara_item)) {
                    lara_item->anim_number = AA_PICKUP;
                    lara_item->current_anim_state = AS_PICKUP;
                    have_item = true;
                }
                g_Lara.interact_target.item_num = item_num;
            } else if (
                g_Lara.interact_target.is_moving
                && g_Lara.interact_target.item_num == item_num) {
                g_Lara.interact_target.is_moving = false;
                g_Lara.gun_status = LGS_ARMLESS;
            }
            if (have_item) {
                g_Lara.head_y_rot = 0;
                g_Lara.head_x_rot = 0;
                g_Lara.torso_y_rot = 0;
                g_Lara.torso_x_rot = 0;
                lara_item->frame_number =
                    g_Anims[lara_item->anim_number].frame_base;
                g_Lara.interact_target.is_moving = false;
                g_Lara.gun_status = LGS_HANDSBUSY;
            }
        } else if (
            g_Lara.interact_target.item_num == item_num
            && lara_item->current_anim_state == AS_PICKUP) {
            if (lara_item->frame_number == AF_PICKUP_ERASE) {
                PickUp_GetItem(item_num, item, lara_item);
            }
        }
    } else if (g_Lara.water_status == LWS_UNDERWATER) {
        item->pos.x_rot = -25 * PHD_DEGREE;

        if ((g_Input.action && lara_item->current_anim_state == AS_TREAD
             && g_Lara.gun_status == LGS_ARMLESS
             && !g_Lara.interact_target.is_moving)
            || (g_Lara.interact_target.is_moving
                && g_Lara.interact_target.item_num == item_num)) {

            if (TestLaraPosition(m_PickUpBoundsUW, item, lara_item)) {
                if (MoveLaraPosition(&m_PickUpPositionUW, item, lara_item)) {
                    lara_item->anim_number = AA_PICKUP_UW;
                    lara_item->current_anim_state = AS_PICKUP;

                    lara_item->goal_anim_state = AS_TREAD;
                    lara_item->frame_number =
                        g_Anims[lara_item->anim_number].frame_base;
                    g_Lara.interact_target.is_moving = false;
                    g_Lara.gun_status = LGS_HANDSBUSY;
                }
                g_Lara.interact_target.item_num = item_num;
            } else if (
                g_Lara.interact_target.is_moving
                && g_Lara.interact_target.item_num == item_num) {
                g_Lara.interact_target.is_moving = false;
                g_Lara.gun_status = LGS_ARMLESS;
            }
        } else if (
            g_Lara.interact_target.item_num == item_num
            && lara_item->current_anim_state == AS_PICKUP
            && lara_item->frame_number == AF_PICKUP_UW) {
            PickUp_GetItem(item_num, item, lara_item);
        }
    }
    item->pos.x_rot = rotx;
    item->pos.y_rot = roty;
    item->pos.z_rot = rotz;
}
