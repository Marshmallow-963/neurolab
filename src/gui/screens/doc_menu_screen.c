#include "raylib.h"
#include "raygui.h"
#include "gui/themes/gui_styles.h"
#include "gui/screens/doc_menu_screen.h"

static const char *DOC_TITLE = "NEUROLAB v1.0.0";

static const char *DOC_DESCRIPTION =
    "DESCRIPTION:\n"
    "Computational Neuronal Dynamics Simulator (v1.0).\n\n"
    "This tool is dedicated to the detailed analysis of individual\n"
    "neurons (Single-Cell Analysis).\n\n"
    "The software allows for an in-depth study of cellular excitability,\n"
    "providing real-time visualization of voltage response, biophysical\n"
    "gating variables, and phase space trajectories.";

static const char *DOC_FEATURES =
    "FEATURES:\n"
    "- Hodgkin-Huxley: Full biophysical simulation visualizing Sodium/\n"
    "  Potassium conductances and gating variables (m, n, h).\n"
    "- Izhikevich: Phenomenological analysis supporting multiple firing\n"
    "  patterns (Regular Spiking, Bursting, Chattering, etc).\n"
    "- Precise Math: RK4 Solver (4th Order Runge-Kutta) ensuring\n"
    "  numerical stability for differential equations.\n"
    "- Dynamic Analysis: Simultaneous plotting of V(t) and Phase Plane\n"
    "  (V vs U) to study limit cycles and equilibrium points.";

static const char *DOC_HISTORY =
    "VERSIONS HISTORY:\n"
    "1.0.0 - Initial Release focused on Unitary Dynamics. Implementation\n"
    "        of IZ and HH models with controlled current injection and\n"
    "        state variable visualization. Implemented by Alex Rodrigues.";


void ScreenDocMenuDraw(AppContext *ctx) {
    float screenwidth  = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    int fontSize = (int)G_UI_STYLES.layout.padding;
    int titleSize = fontSize * 2;

    Font font = GuiGetFont();
    float textSpacing = 1.0f;
    float sectionPadding = G_UI_STYLES.layout.padding * 2.0f;

    Rectangle panelBounds = {
        G_UI_STYLES.layout.padding,  G_UI_STYLES.layout.padding,
        screenwidth - (G_UI_STYLES.layout.padding * 2.0f),
        screenHeight - (G_UI_STYLES.layout.padding * 2.0f)
    };

    Vector2 titleSizeVec    = MeasureTextEx(font, DOC_TITLE, titleSize, textSpacing);
    Vector2 descSizeVec     = MeasureTextEx(font, DOC_DESCRIPTION, fontSize, textSpacing);
    Vector2 featuresSizeVec = MeasureTextEx(font, DOC_FEATURES, fontSize, textSpacing);
    Vector2 historySizeVec  = MeasureTextEx(font, DOC_HISTORY, fontSize, textSpacing);

    float totalContentHeight = G_UI_STYLES.layout.padding;
    totalContentHeight += titleSizeVec.y + sectionPadding;
    totalContentHeight += descSizeVec.y + sectionPadding;
    totalContentHeight += featuresSizeVec.y + sectionPadding;
    totalContentHeight += historySizeVec.y + G_UI_STYLES.layout.padding;

    Rectangle contentSize = { 0, 0, panelBounds.width - 20, totalContentHeight };

    Rectangle visibleContent = { 0 };
    GuiScrollPanel(panelBounds, NULL, contentSize, &ctx->app.docScroll, &visibleContent);

    float textPosX = visibleContent.x + G_UI_STYLES.layout.padding;
    float textPosY = visibleContent.y + G_UI_STYLES.layout.padding + ctx->app.docScroll.y;

    DrawText(DOC_TITLE, textPosX, textPosY, titleSize, G_UI_STYLES.colors.textColor);
    textPosY += titleSizeVec.y + sectionPadding;

    DrawText(DOC_DESCRIPTION, textPosX, textPosY, fontSize, G_UI_STYLES.colors.textColor);
    textPosY += descSizeVec.y + sectionPadding;

    DrawText(DOC_FEATURES, textPosX, textPosY, fontSize, G_UI_STYLES.colors.textColor);
    textPosY += featuresSizeVec.y + sectionPadding;

    DrawText(DOC_HISTORY, textPosX, textPosY, fontSize, G_UI_STYLES.colors.textColor);
}
