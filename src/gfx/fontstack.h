// FontStack — the editor's two typefaces, loaded straight out of the plug-in
// bundle with FreeType and wrapped as Cairo font faces.
//
// Michroma is the title face and Roboto the body face, matching the original
// plug-in. Unlike app_server on Haiku, FreeType needs no font *installation*
// step: the .ttf files in Contents/Resources/fonts are opened in place, so the
// sibling port's installFonts() has no counterpart here.
//
// If a face fails to load, face() falls back to a generic Cairo "toy" face of
// the same weight so text still renders (one warning on stderr, then silence).

#pragma once

#include <cairo/cairo.h>

#include <memory>
#include <string>

namespace NAMix
{

//------------------------------------------------------------------------
class FontStack
{
public:
    FontStack() = default;
    ~FontStack();

    FontStack(const FontStack &) = delete;
    FontStack &operator=(const FontStack &) = delete;

    // Load both faces from <resourceDir>/fonts. Safe to call once; returns
    // false if either face fell back, having already warned.
    bool load(const std::string &resourceDir);

    cairo_font_face_t *title() const
    {
        return mTitle;
    }
    cairo_font_face_t *body() const
    {
        return mBody;
    }

private:
    // Returns a cairo face that OWNS the FT_Face behind it and a reference to the FreeType
    // library (see the comment on the destructor in fontstack.cpp). *outFt is set to the FT_Face
    // purely so load() can tell a real face from a toy fallback.
    cairo_font_face_t *loadFace(const std::string &path, bool bold, void **outFt);

    cairo_font_face_t *mTitle = nullptr;
    cairo_font_face_t *mBody = nullptr;

    // Observers, NOT owners: the FT_Face belongs to the cairo font face above and is freed by
    // cairo, which may be long after this object dies. Never FT_Done_Face these.
    void *mTitleFt = nullptr;
    void *mBodyFt = nullptr;

    // FT_Library, held by shared_ptr because each face keeps a reference of its own: the library
    // must outlive every face opened from it, and cairo decides when those die.
    std::shared_ptr<void> mLibrary;
};

} // namespace NAMix
