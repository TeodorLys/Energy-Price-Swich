#pragma once
#include <Arduino.h>

class html_element{
public:
    enum class TEXT_TYPES
    {
        H1,
        DIV,
        H2,
        P
    };

    enum class TEXT_STYLE
    {
        BOLD,
        ITALIC,
        UNDERLINE,
        NONE
    };

private:
    String _value;
    String _name;
    String _class;
    TEXT_TYPES _type;
    TEXT_STYLE _style;

public:
    html_element(String _v, String _n, String _c, TEXT_TYPES _t, TEXT_STYLE _st) : _value(_v), _name(_n), _class(_c), _type(_t), _style(_st){}

    String get_class() const { return _class; }
    String get_name() const { return _name; }
    String get_value() const { return _value; }

    void set_class(String _c) { _class = _c; }
    void set_name(String _n) { _name = _n; }
    void set_value(String _v) { _value = _v; }

    String construct_html_line(){
        String _buff = "<";
        switch (_type)
        {
        case TEXT_TYPES::DIV:
            _buff += "div";
            break;
        case TEXT_TYPES::H1:
            _buff += "h1";
        case TEXT_TYPES::H2:
            _buff += "h2";
        case TEXT_TYPES::P:
            _buff += "p";

        default:
            break;
        }
        _buff += " class=\"";
        _buff += _class + "\"";
        _buff += ">";
        switch (_style)
        {
        case TEXT_STYLE::BOLD:
            _buff += "<b>" + _value + "</b>";
            break;
        case TEXT_STYLE::ITALIC:
            _buff += "<i>" + _value + "</i>";
            break;
        case TEXT_STYLE::UNDERLINE:
            _buff += "<u>" + _value + "</u>";
            break;
        case TEXT_STYLE::NONE:
            _buff += _value;
            break;
        default:
            break;
        }

        switch (_type)
        {
        case TEXT_TYPES::DIV:
            _buff += "</div>";
            break;
        case TEXT_TYPES::H1:
            _buff += "</h1>";
        case TEXT_TYPES::H2:
            _buff += "</h2>";
        case TEXT_TYPES::P:
            _buff += "</p>";

        default:
            break;
        }
    }
};