/*
  Copyright (c) 2015, 2016, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MYSQLROUTER_GTEST_ROUTER_EXETEST_INCLUDED
#define MYSQLROUTER_GTEST_ROUTER_EXETEST_INCLUDED

#include "filesystem.h"

#include <memory>

#include "gmock/gmock.h"

using mysql_harness::Path;
using std::string;

class ConsoleOutputTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    using mysql_harness::Path;

    char *stage_dir_c = std::getenv("STAGE_DIR");
    if (stage_dir_c == nullptr) {
      stage_dir.reset(new Path("./stage"));
    } else {
      stage_dir.reset(new Path(stage_dir_c));
    }
#ifdef _WIN32
    if (origin_dir) {
      *stage_dir = stage_dir->join(origin_dir->basename());
    } else {
      *stage_dir = stage_dir->join("RelWithDebInfo");
    }
#endif
    plugin_dir.reset(new Path(*stage_dir));
    plugin_dir->append("lib");
#ifndef _WIN32
    plugin_dir->append("mysqlrouter");
#endif
    app_mysqlrouter.reset(new Path(*stage_dir));
    app_mysqlrouter->append("bin");
#ifdef _WIN32
    app_mysqlrouter->append("mysqlrouter.exe");
#else
    app_mysqlrouter->append("mysqlrouter");
#endif
    orig_cout_ = std::cout.rdbuf();
    std::cout.rdbuf(ssout.rdbuf());
  }

  virtual void TearDown() {
    if (orig_cout_) {
      std::cout.rdbuf(orig_cout_);
    }
  }

  void reset_ssout() {
    ssout.str("");
    ssout.clear();
  }

  void set_origin(const Path &origin) {
    origin_dir.reset(new Path(origin));
  }

  std::unique_ptr<Path> stage_dir;
  std::unique_ptr<Path> plugin_dir;
  std::unique_ptr<Path> app_mysqlrouter;
  std::unique_ptr<Path> origin_dir;

  std::stringstream ssout;
  std::streambuf *orig_cout_;
};

#endif // MYSQLROUTER_GTEST_ROUTER_EXETEST_INCLUDED
