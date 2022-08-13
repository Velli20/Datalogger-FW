/* AUTOMATICALLY GENERATED FILE - DO NOT MODIFY */

#ifndef resources_h
#define resources_h

// Includes

#include "digital_30.h"
#include "digital_20.h"
#include "digital_14.h"
#include "digital_24.h"

// Resource access macros.

#define READ_FONT_RESOURCE(FONT_NAME, SIZE) \
    &(font_##FONT_NAME##_##SIZE)

#define READ_BITMAP_RESOURCE(BITMAP) \
    &(BITMAP##_bmp)

#endif
