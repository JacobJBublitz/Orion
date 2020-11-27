#include "Orion/Version.hh"

#include "gtest/gtest.h"

namespace Orion {

TEST(VersionTest, Basic) {
  Version v{1, 0, 0};
  EXPECT_EQ(1, v.Major());
  EXPECT_EQ(0, v.Minor());
  EXPECT_EQ(0, v.Patch());
  EXPECT_EQ("", v.Prerelease());
  EXPECT_EQ("", v.BuildMetadata());
}

TEST(VersionTest, Prerelease) {
  Version v{0, 1, 0, "alpha.2"};
  EXPECT_EQ("alpha.2", v.Prerelease());
  EXPECT_EQ("alpha", v.Prerelease(0));
  EXPECT_EQ("2", v.Prerelease(1));

  for (int i = 2; i < 256; ++i)
    EXPECT_EQ("", v.Prerelease(i));
}

TEST(VersionTest, BuildMetadata) {
  Version v{0, 1, 0, "", "12345.3.dirty"};
  EXPECT_EQ("12345.3.dirty", v.BuildMetadata());
  EXPECT_EQ("12345", v.BuildMetadata(0));
  EXPECT_EQ("3", v.BuildMetadata(1));
  EXPECT_EQ("dirty", v.BuildMetadata(2));

  for (int i = 3; i < 256; ++i)
    EXPECT_EQ("", v.BuildMetadata(i));
}

TEST(VersionTest, ToString) {
  Version v{1, 0, 0, "rc.2", "12345"};
  std::stringstream ss;
  ss << v;

  EXPECT_EQ("1.0.0-rc.2+12345", ss.str());
}

} // namespace Orion