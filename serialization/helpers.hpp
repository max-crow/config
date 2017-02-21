#pragma once

    namespace config {
        struct item_cfg: public config::serializable {
            std::wstring name;
            BEGIN_CONFIG_MAP()
                CONFIG_NAME(name);
            END_CONFIG_MAP()
        };
    }
