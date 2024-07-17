
#pragma once

#include "nlohmann/json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

namespace fcs::mos {
    using nlohmann::json;

    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }

    class EncDec {
        public:
        EncDec() = default;
        virtual ~EncDec() = default;

        private:
        std::string action;
        double tip;
        double tilt;

        std::optional<double> piston = std::nullopt;

        public:
        const std::string & get_action() const { return action; }
        std::string & get_mutable_action() { return action; }
        void set_action(const std::string & value) { this->action = value; }

        const double & get_tip() const { return tip; }
        double & get_mutable_tip() { return tip; }
        void set_tip(const double & value) { this->tip = value; }

        const double & get_tilt() const { return tilt; }
        double & get_mutable_tilt() { return tilt; }
        void set_tilt(const double & value) { this->tilt = value; }

        const double & get_piston() const { return piston.value(); }
        bool has_piston() const {return piston.has_value(); }
        double & get_mutable_piston() { return piston.value(); }
        void set_piston(const double & value) { this->piston = value; }
    };
}

namespace nlohmann {
    void from_json(const json & j, fcs::mos::EncDec & x);
    void to_json(json & j, const fcs::mos::EncDec & x);

    inline void from_json(const json & j, fcs::mos::EncDec& x) {
        x.set_action(j.at("action").get<std::string>());
        x.set_tip(j.at("tip").get<double>());
        x.set_tilt(j.at("tilt").get<double>());
        if (j.count("piston") != 0)
          {
              x.set_piston(j.at("piston").get<double>());
          }
    }

    inline void to_json(json & j, const fcs::mos::EncDec & x) {
        j = json::object();
        j["action"] = x.get_action();
        j["tip"] = x.get_tip();
        j["tilt"] = x.get_tilt();

        if (x.has_piston())
          {
            j["piston"] = x.get_piston();
          }
    }
}
