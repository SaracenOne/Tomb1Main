#include "game/option.h"

#include "config.h"
#include "game/gameflow.h"
#include "game/input.h"
#include "game/screen.h"
#include "game/settings.h"
#include "game/text.h"
#include "global/vars.h"
#include "src/gfx/context.h"

#include <stdio.h>

#define TOP_Y -55
#define ROW_HEIGHT 25
#define ROW_WIDTH 280
#define OPTION_LENGTH 256

typedef enum GRAPHICS_TEXT {
    TEXT_PERSPECTIVE = 0,
    TEXT_BILINEAR = 1,
    TEXT_VSYNC = 2,
    TEXT_BRIGHTNESS = 3,
    TEXT_UI_TEXT_SCALE = 4,
    TEXT_UI_BAR_SCALE = 5,
    TEXT_RESOLUTION = 6,
    TEXT_TITLE = 7,
    TEXT_TITLE_BORDER = 8,
    TEXT_NUMBER_OF = 9,
    TEXT_OPTION_MIN = TEXT_PERSPECTIVE,
    TEXT_OPTION_MAX = TEXT_RESOLUTION,
} GRAPHICS_TEXT;

static bool m_IsTextInit = false;
static TEXTSTRING *m_Text[TEXT_NUMBER_OF] = { 0 };

static void Option_GraphicsInitText();
static void Option_GraphicsChangeTextOption(int32_t option_num);

static void Option_GraphicsInitText()
{
    char buf[OPTION_LENGTH];
    int32_t y = TOP_Y;
    m_Text[TEXT_TITLE_BORDER] = Text_Create(0, y - 2, " ");

    m_Text[TEXT_TITLE] =
        Text_Create(0, y, g_GameFlow.strings[GS_DETAIL_SELECT_DETAIL]);
    y += ROW_HEIGHT;

    sprintf(
        buf, g_GameFlow.strings[GS_DETAIL_PERSPECTIVE_FMT],
        g_GameFlow.strings
            [g_Config.rendering.enable_perspective_filter ? GS_MISC_ON
                                                          : GS_MISC_OFF]);
    m_Text[TEXT_PERSPECTIVE] = Text_Create(0, y, buf);
    y += ROW_HEIGHT;

    sprintf(
        buf, g_GameFlow.strings[GS_DETAIL_BILINEAR_FMT],
        g_GameFlow.strings
            [g_Config.rendering.enable_bilinear_filter ? GS_MISC_ON
                                                       : GS_MISC_OFF]);
    m_Text[TEXT_BILINEAR] = Text_Create(0, y, buf);
    y += ROW_HEIGHT;

    sprintf(
        buf, g_GameFlow.strings[GS_DETAIL_VSYNC_FMT],
        g_GameFlow.strings
            [g_Config.rendering.enable_vsync ? GS_MISC_ON : GS_MISC_OFF]);
    m_Text[TEXT_VSYNC] = Text_Create(0, y, buf);
    y += ROW_HEIGHT;

    sprintf(
        buf, g_GameFlow.strings[GS_DETAIL_BRIGHTNESS_FMT], g_Config.brightness);
    m_Text[TEXT_BRIGHTNESS] = Text_Create(0, y, buf);
    y += ROW_HEIGHT;

    sprintf(
        buf, g_GameFlow.strings[GS_DETAIL_UI_TEXT_SCALE_FMT],
        g_Config.ui.text_scale);
    m_Text[TEXT_UI_TEXT_SCALE] = Text_Create(0, y, buf);
    y += ROW_HEIGHT;

    sprintf(
        buf, g_GameFlow.strings[GS_DETAIL_UI_BAR_SCALE_FMT],
        g_Config.ui.bar_scale);
    m_Text[TEXT_UI_BAR_SCALE] = Text_Create(0, y, buf);
    y += ROW_HEIGHT;

    char tmp[10];
    sprintf(
        tmp, "%dx%d", Screen_GetPendingResWidth(),
        Screen_GetPendingResHeight());
    sprintf(buf, g_GameFlow.strings[GS_DETAIL_VIDEO_MODE_FMT], tmp);
    m_Text[TEXT_RESOLUTION] = Text_Create(0, y, buf);
    y += ROW_HEIGHT;

    if (g_OptionSelected < TEXT_OPTION_MIN) {
        g_OptionSelected = TEXT_OPTION_MIN;
    }
    if (g_OptionSelected > TEXT_OPTION_MAX) {
        g_OptionSelected = TEXT_OPTION_MAX;
    }

    Text_AddBackground(m_Text[TEXT_TITLE_BORDER], ROW_WIDTH, y - TOP_Y, 0, 0);
    Text_AddOutline(m_Text[TEXT_TITLE_BORDER], 1);

    Text_AddBackground(m_Text[TEXT_TITLE], ROW_WIDTH - 4, 0, 0, 0);
    Text_AddOutline(m_Text[TEXT_TITLE], 1);

    Text_AddBackground(m_Text[g_OptionSelected], ROW_WIDTH - 12, 0, 0, 0);
    Text_AddOutline(m_Text[g_OptionSelected], 1);

    for (int i = 0; i < TEXT_NUMBER_OF; i++) {
        Text_CentreH(m_Text[i], 1);
        Text_CentreV(m_Text[i], 1);
    }
}

static void Option_GraphicsChangeTextOption(int32_t option_num)
{
    char buf[OPTION_LENGTH];
    char tmp[10];

    switch (option_num) {
    case TEXT_PERSPECTIVE:
        sprintf(
            buf, g_GameFlow.strings[GS_DETAIL_PERSPECTIVE_FMT],
            g_GameFlow.strings
                [g_Config.rendering.enable_perspective_filter ? GS_MISC_ON
                                                              : GS_MISC_OFF]);
        Text_ChangeText(m_Text[TEXT_PERSPECTIVE], buf);
        break;

    case TEXT_BILINEAR:
        sprintf(
            buf, g_GameFlow.strings[GS_DETAIL_BILINEAR_FMT],
            g_GameFlow.strings
                [g_Config.rendering.enable_bilinear_filter ? GS_MISC_ON
                                                           : GS_MISC_OFF]);
        Text_ChangeText(m_Text[TEXT_BILINEAR], buf);
        break;

    case TEXT_VSYNC:
        sprintf(
            buf, g_GameFlow.strings[GS_DETAIL_VSYNC_FMT],
            g_GameFlow.strings
                [g_Config.rendering.enable_vsync ? GS_MISC_ON : GS_MISC_OFF]);
        Text_ChangeText(m_Text[TEXT_VSYNC], buf);
        break;

    case TEXT_BRIGHTNESS:
        sprintf(
            buf, g_GameFlow.strings[GS_DETAIL_BRIGHTNESS_FMT],
            g_Config.brightness);
        Text_ChangeText(m_Text[TEXT_BRIGHTNESS], buf);
        break;

    case TEXT_UI_TEXT_SCALE:
        sprintf(
            buf, g_GameFlow.strings[GS_DETAIL_UI_TEXT_SCALE_FMT],
            g_Config.ui.text_scale);
        Text_ChangeText(m_Text[TEXT_UI_TEXT_SCALE], buf);
        break;

    case TEXT_UI_BAR_SCALE:
        sprintf(
            buf, g_GameFlow.strings[GS_DETAIL_UI_BAR_SCALE_FMT],
            g_Config.ui.bar_scale);
        Text_ChangeText(m_Text[TEXT_UI_BAR_SCALE], buf);
        break;

    case TEXT_RESOLUTION:
        sprintf(
            tmp, "%dx%d", Screen_GetPendingResWidth(),
            Screen_GetPendingResHeight());
        sprintf(buf, g_GameFlow.strings[GS_DETAIL_VIDEO_MODE_FMT], tmp);
        Text_ChangeText(m_Text[TEXT_RESOLUTION], buf);
        break;
    }
}

void Option_Graphics(INVENTORY_ITEM *inv_item)
{
    if (!m_IsTextInit) {
        Option_GraphicsInitText();
        m_IsTextInit = true;
    }

    if (g_InputDB.forward && g_OptionSelected > TEXT_OPTION_MIN) {
        Text_RemoveOutline(m_Text[g_OptionSelected]);
        Text_RemoveBackground(m_Text[g_OptionSelected]);
        g_OptionSelected--;
        Text_AddOutline(m_Text[g_OptionSelected], 1);
        Text_AddBackground(m_Text[g_OptionSelected], ROW_WIDTH - 12, 0, 0, 0);
    }

    if (g_InputDB.back && g_OptionSelected < TEXT_OPTION_MAX) {
        Text_RemoveOutline(m_Text[g_OptionSelected]);
        Text_RemoveBackground(m_Text[g_OptionSelected]);
        g_OptionSelected++;
        Text_AddOutline(m_Text[g_OptionSelected], 1);
        Text_AddBackground(m_Text[g_OptionSelected], ROW_WIDTH - 12, 0, 0, 0);
    }

    int32_t reset = -1;

    if (g_InputDB.right) {
        switch (g_OptionSelected) {
        case TEXT_PERSPECTIVE:
            if (!g_Config.rendering.enable_perspective_filter) {
                g_Config.rendering.enable_perspective_filter = true;
                reset = TEXT_PERSPECTIVE;
            }
            break;

        case TEXT_BILINEAR:
            if (!g_Config.rendering.enable_bilinear_filter) {
                g_Config.rendering.enable_bilinear_filter = true;
                reset = TEXT_BILINEAR;
            }
            break;

        case TEXT_VSYNC:
            if (!g_Config.rendering.enable_vsync) {
                g_Config.rendering.enable_vsync = true;
                reset = TEXT_VSYNC;
                GFX_Context_SetVSync(g_Config.rendering.enable_vsync);
            }
            break;

        case TEXT_BRIGHTNESS:
            if (g_Config.brightness < MAX_BRIGHTNESS) {
                g_Config.brightness += 0.1f;
                reset = TEXT_BRIGHTNESS;
            }
            break;

        case TEXT_UI_TEXT_SCALE:
            if (g_Config.ui.text_scale < MAX_UI_SCALE) {
                g_Config.ui.text_scale += 0.1;
                reset = TEXT_UI_TEXT_SCALE;
            }
            break;

        case TEXT_UI_BAR_SCALE:
            if (g_Config.ui.bar_scale < MAX_UI_SCALE) {
                g_Config.ui.bar_scale += 0.1;
                reset = TEXT_UI_BAR_SCALE;
            }
            break;

        case TEXT_RESOLUTION:
            if (Screen_SetNextRes()) {
                reset = TEXT_RESOLUTION;
            }
            break;
        }
    }

    if (g_InputDB.left) {
        switch (g_OptionSelected) {
        case TEXT_PERSPECTIVE:
            if (g_Config.rendering.enable_perspective_filter) {
                g_Config.rendering.enable_perspective_filter = false;
                reset = TEXT_PERSPECTIVE;
            }
            break;

        case TEXT_BILINEAR:
            if (g_Config.rendering.enable_bilinear_filter) {
                g_Config.rendering.enable_bilinear_filter = false;
                reset = TEXT_BILINEAR;
            }
            break;

        case TEXT_VSYNC:
            if (g_Config.rendering.enable_vsync) {
                g_Config.rendering.enable_vsync = false;
                reset = TEXT_VSYNC;
                GFX_Context_SetVSync(g_Config.rendering.enable_vsync);
            }
            break;

        case TEXT_BRIGHTNESS:
            if (g_Config.brightness > MIN_BRIGHTNESS) {
                g_Config.brightness -= 0.1f;
                reset = TEXT_BRIGHTNESS;
            }
            break;

        case TEXT_UI_TEXT_SCALE:
            if (g_Config.ui.text_scale > MIN_UI_SCALE) {
                g_Config.ui.text_scale -= 0.1;
                reset = TEXT_UI_TEXT_SCALE;
            }
            break;

        case TEXT_UI_BAR_SCALE:
            if (g_Config.ui.bar_scale > MIN_UI_SCALE) {
                g_Config.ui.bar_scale -= 0.1;
                reset = TEXT_UI_BAR_SCALE;
            }
            break;

        case TEXT_RESOLUTION:
            if (Screen_SetPrevRes()) {
                reset = TEXT_RESOLUTION;
            }
            break;
        }
    }

    if (g_InputDB.deselect || g_InputDB.select) {
        reset = TEXT_NUMBER_OF;
    }

    if (reset == TEXT_NUMBER_OF) {
        for (int i = 0; i < TEXT_NUMBER_OF; i++) {
            Text_Remove(m_Text[i]);
            m_Text[i] = NULL;
        }
        m_IsTextInit = false;
        Settings_Write();
    } else if (reset > -1) {
        Option_GraphicsChangeTextOption(reset);
        Settings_Write();
    }
}
