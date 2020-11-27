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

TEST(VersionTest, Parse) {
  auto v = Version::Parse("1.0.0");
  EXPECT_TRUE(v.has_value());
  EXPECT_EQ(1, v->Major());
  EXPECT_EQ(0, v->Minor());
  EXPECT_EQ(0, v->Patch());
  EXPECT_TRUE(v->Prerelease().empty());
  EXPECT_TRUE(v->BuildMetadata().empty());

  v = Version::Parse("0.1.42-beta.2+12345.dirty");
  EXPECT_TRUE(v.has_value());
  EXPECT_EQ(0, v->Major());
  EXPECT_EQ(1, v->Minor());
  EXPECT_EQ(42, v->Patch());
  EXPECT_EQ("beta.2", v->Prerelease());
  EXPECT_EQ("12345.dirty", v->BuildMetadata());

  v = Version::Parse("Not a version");
  EXPECT_FALSE(v.has_value());

  v = Version::Parse("1234.5678.9+12345abcdef");
  EXPECT_TRUE(v.has_value());
  EXPECT_EQ(1234, v->Major());
  EXPECT_EQ(5678, v->Minor());
  EXPECT_EQ(9, v->Patch());
  EXPECT_TRUE(v->Prerelease().empty());
  EXPECT_EQ("12345abcdef", v->BuildMetadata());
}

TEST(VersionTest, ToString) {
  Version v{1, 0, 0, "rc.2", "12345"};
  std::stringstream ss;
  ss << v;

  EXPECT_EQ("1.0.0-rc.2+12345", ss.str());
}

} // namespace Orion