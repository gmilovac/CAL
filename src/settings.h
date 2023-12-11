//
// Created by Gordan Milovac on 12/6/23.
//

#ifndef CAL_SETTINGS_H
#define CAL_SETTINGS_H

#include <QObject>
#include "rgba.h"

// Enumeration values for the Brush types from which the user can choose in the GUI.
enum BrushType {
    BRUSH_CONSTANT,
    BRUSH_FILL
};

/**
 * @struct Settings
 *
 * Stores application settings for the CS123 GUI.
 *
 * You can access all app settings through the "settings" global variable.
 * The settings will be automatically updated when things are changed in the
 * GUI (the reverse is not true however: changing the value of a setting does
 * not update the GUI).
*/
struct Settings {
    // Brush
    int brushType;      // The user's selected brush @see BrushType
    int brushRadius;    // The brush radius
    RGBA brushColor;
    int brushDensity; // This is for spray brush (extra credit)
    bool fixAlphaBlending; // Fix alpha blending (extra credit)
    bool cell;

    // Filter
    int filterType;                     // The selected filter @see FilterType
    float edgeDetectSensitivity;    // Edge detection sensitivity, from 0 to 1.
    int blurRadius;                 // Selected blur radius
    float scaleX;                   // Horizontal scale factor
    float scaleY;                   // Vertical scale factor
    int medianRadius;               // Median radius (extra credit)
    float rotationAngle;            // Rotation angle (extra credit)
    int bilateralRadius;            // Bilateral radius (extra credit)
    int rShift;                     // Chromatic aberration red channel shift (extra credit)
    int gShift;                     // Chromatic aberration green channel shift (extra credit)
    int bShift;                     // Chromatic aberration blue channel shift (extra credit)
    bool nonLinearMap;              // Use non-linear mapping function for tone mapping (extra credit)
    float gamma;                    // Gamma for tone mapping (extra credit)

    QString imagePath;

    void loadSettingsOrDefaults();
    void saveSettings();
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif //CAL_SETTINGS_H
