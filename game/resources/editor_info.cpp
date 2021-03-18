#include <resources/editor_info.h>
#include <resources/paths.h>
#include <resources/graphics.h>

#include <utility/assertion.h>

#include <nlohmann/json.hpp>

#include <fstream>

void EditorInfoImpl::Load() {
    LoadSpriteNames();
}

void EditorInfoImpl::LoadSpriteNames() {
    std::ifstream file(Paths::Editor + "sprite_names.json");
    if (!file.good()) {
        THROWERROR("Cannot open file " + Paths::Editor + "sprite_names.json");
    }
    nlohmann::json data;
    file >> data;
    for (auto& name : data) {
        SpriteNames.push_back(name);
    }
}


void EditorInfoImpl::Reset() {
    SpriteNames.clear();
}


const EditorInfoImpl& EditorInfo() {
    return EditorInfoMut();
}

EditorInfoImpl& EditorInfoMut() {
    static EditorInfoImpl impl;
    return impl;
}
