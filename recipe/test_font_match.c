#include <stdio.h>
#include <string.h>
#include <fontconfig/fontconfig.h>

int main(void) {
    FcPattern *request, *match;
    FcResult result;
    FcChar8 *family, *path;
    FILE *font;
    int status = 1;
    if (!FcInit() || FcGetVersion() != FC_VERSION) return 1;
    request = FcPatternCreate();
    if (!request) return 1;
    if (!FcPatternAddString(request, FC_FAMILY, (const FcChar8 *)"DejaVu Sans Mono") ||
        !FcConfigSubstitute(NULL, request, FcMatchPattern)) goto cleanup_request;
    FcDefaultSubstitute(request);
    match = FcFontMatch(NULL, request, &result);
    if (!match) goto cleanup_request;
    if (FcPatternGetString(match, FC_FAMILY, 0, &family) != FcResultMatch ||
        strcmp((const char *)family, "DejaVu Sans Mono") != 0 ||
        FcPatternGetString(match, FC_FILE, 0, &path) != FcResultMatch) goto cleanup_match;
    font = fopen((const char *)path, "rb");
    if (!font) goto cleanup_match;
    status = fgetc(font) == EOF;
    fclose(font);
    printf("Matched %s at %s\n", family, path);
cleanup_match:
    FcPatternDestroy(match);
cleanup_request:
    FcPatternDestroy(request);
    FcFini();
    return status;
}
