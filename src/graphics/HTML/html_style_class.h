#pragma once
#include <Arduino.h>

class html_style_class {
private:
    String _class_declaration;
public:
    html_style_class(String _decl) : _class_declaration(_decl) {}
    String get_class_decl() const { return _class_declaration; }
    void set_class_decl(String _decl) { _class_declaration = _decl; }
};