#pragma once

#include <string>

enum class value_type {
    TYPE_STRING,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_BOOL,
};

struct param_description {
    /// то, что быдуте отображаться в UI
    std::string description;
    /// имя по которому берутся данные
    std::string name;
    /// тип
    value_type type;
};
