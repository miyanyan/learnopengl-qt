#ifndef CONFIG_H
#define CONFIG_H

#define MACRO2STR(R) #R
#define STR_MACRO(R) MACRO2STR(R)
#define STR_SRC_PATH STR_MACRO(SRC_PATH) // get src file path

#endif // CONFIG_H
