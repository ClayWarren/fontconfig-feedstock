#include <stdio.h>
#include <string.h>
#include <fontconfig/fontconfig.h>

int main(int argc, char **argv) {
    FcPattern *request, *match;
    FcResult result;
    FcChar8 *family, *path;
    FILE *font;
    int status = 1;
    if (argc != 2 || !FcInit() || FcGetVersion() != FC_VERSION) {
        fprintf(stderr, "Fontconfig initialization/version check failed\n");
        return 1;
    }
    if (!FcConfigAppFontAddFile(NULL, (const FcChar8 *)argv[1])) {
        fprintf(stderr, "Could not register test font: %s\n", argv[1]);
        return 1;
    }
    request = FcPatternCreate();
    if (!request) return 1;
    if (!FcPatternAddString(request, FC_FAMILY, (const FcChar8 *)"DejaVu Sans") ||
        !FcConfigSubstitute(NULL, request, FcMatchPattern)) goto cleanup_request;
    FcDefaultSubstitute(request);
    match = FcFontMatch(NULL, request, &result);
    if (!match) goto cleanup_request;
    if (FcPatternGetString(match, FC_FAMILY, 0, &family) != FcResultMatch ||
        strcmp((const char *)family, "DejaVu Sans") != 0 ||
        FcPatternGetString(match, FC_FILE, 0, &path) != FcResultMatch) goto cleanup_match;
    font = fopen((const char *)path, "rb");
    if (!font) { fprintf(stderr, "Cannot open matched font: %s\n", path); goto cleanup_match; }
    status = fgetc(font) == EOF;
    fclose(font);
    printf("Matched %s at %s\n", family, path);
cleanup_match:
    FcPatternDestroy(match);
cleanup_request:
    FcPatternDestroy(request);
    FcFini();
    if (status) fprintf(stderr, "Font matching or file-read check failed\n");
    return status;
}
