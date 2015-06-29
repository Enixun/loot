/*  LOOT

A load order optimisation tool for Oblivion, Skyrim, Fallout 3 and
Fallout: New Vegas.

Copyright (C) 2014-2015    WrinklyNinja

This file is part of LOOT.

LOOT is free software: you can redistribute
it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

LOOT is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LOOT.  If not, see
<http://www.gnu.org/licenses/>.
*/

#ifndef LOOT_TEST_BACKEND_METADATA_LOCATION
#define LOOT_TEST_BACKEND_METADATA_LOCATION

#include "backend/metadata/location.h"
#include "tests/fixtures.h"

using loot::Location;

TEST(Location, ConstructorsAndDataAccess) {
    Location loc;
    EXPECT_EQ("", loc.URL());

    loc = Location("http://www.example.com");
    EXPECT_EQ("http://www.example.com", loc.URL());

    loc = Location("http://www.example.com", {
        "1.0",
        "1.1"
    });
    EXPECT_EQ("http://www.example.com", loc.URL());
    EXPECT_EQ(std::vector<std::string>({
        "1.0",
        "1.1"
    }), loc.Versions());
}

TEST(Location, LessThanOperator) {
    Location loc1, loc2;
    EXPECT_FALSE(loc1 < loc2);
    EXPECT_FALSE(loc2 < loc1);

    loc1 = Location("http://www.example.com");
    loc2 = Location("HTTP://WWW.EXAMPLE.COM");
    EXPECT_FALSE(loc1 < loc2);
    EXPECT_FALSE(loc2 < loc1);

    loc1 = Location("http://www.example.com", {"1.0"});
    loc2 = Location("http://www.example.com", {"1.1"});
    EXPECT_FALSE(loc1 < loc2);
    EXPECT_FALSE(loc2 < loc1);

    loc1 = Location("http://www.example1.com");
    loc2 = Location("http://www.example2.com");
    EXPECT_TRUE(loc1 < loc2);
    EXPECT_FALSE(loc2 < loc1);
}

TEST(Location, YamlEmitter) {
    Location loc("http://www.example.com");
    YAML::Emitter e1;
    e1 << loc;
    EXPECT_STREQ("'http://www.example.com'", e1.c_str());

    loc = Location("http://www.example.com", {
        "1.0",
        "1.1"
    });
    YAML::Emitter e2;
    e2 << loc;
    EXPECT_STREQ("link: 'http://www.example.com'\nver:\n  - '1.0'\n  - '1.1'", e2.c_str());
}

TEST(Location, YamlEncode) {
    YAML::Node node;

    Location loc("http://www.example.com");
    node = loc;
    EXPECT_EQ("http://www.example.com", node["link"].as<std::string>());
    EXPECT_FALSE(node["ver"]);

    loc = Location("http://www.example.com", {
        "1.0",
        "1.1"
    });
    node = loc;
    EXPECT_EQ("http://www.example.com", node["link"].as<std::string>());
    EXPECT_EQ(std::vector<std::string>({
        "1.0",
        "1.1"
    }), node["ver"].as<std::vector<std::string>>());
}

TEST(Location, YamlDecode) {
    YAML::Node node;
    Location loc;

    node = YAML::Load("http://www.example.com");
    loc = node.as<Location>();
    EXPECT_EQ("http://www.example.com", loc.URL());
    EXPECT_EQ(std::vector<std::string>(), loc.Versions());

    node = YAML::Load("{link: http://www.example.com, ver: [ 1.0, 1.1]}");
    loc = node.as<Location>();
    EXPECT_EQ("http://www.example.com", loc.URL());
    EXPECT_EQ(std::vector<std::string>({
        "1.0",
        "1.1"
    }), loc.Versions());
}

#endif
