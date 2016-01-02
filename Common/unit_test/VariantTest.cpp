
#include <gtest/gtest.h>
#include <Variant.h>

bool TestVariant_Boolean1()
{
  common::Variant v(true);
  return v.ToBoolean();
}

bool TestVariant_Boolean2()
{
  common::Variant v(false);
  return v.ToBoolean();
}

uint8_t TestVariant_UInt8(uint8_t n)
{
  common::Variant v(n);
  return v.ToUInt8();
}

std::string TestVariant_StdString(std::string const& s)
{
  common::Variant v(s);
  return v.ToString();
}

TEST(VariantTest, Boolean) {
  EXPECT_TRUE(TestVariant_Boolean1());
  EXPECT_FALSE(TestVariant_Boolean2());
}

TEST(VariantTest, UInt8) {
  EXPECT_EQ(0, TestVariant_UInt8(0));
  EXPECT_EQ(10, TestVariant_UInt8(10));
  EXPECT_EQ(100, TestVariant_UInt8(100));
  EXPECT_EQ(200, TestVariant_UInt8(200));
  EXPECT_EQ(std::numeric_limits<uint8_t>::max(), TestVariant_UInt8(std::numeric_limits<uint8_t>::max()));

  for (uint8_t n = 0; n < std::numeric_limits<uint8_t>::max(); ++n)
  {
    EXPECT_EQ(n, TestVariant_UInt8(n));
  }
}

TEST(Variant, UInt8Array) {
  std::vector<uint8_t> arr;
  for (int i = 0; i < 10; ++i)
    arr.push_back(i);

  common::Variant v1(arr);
  EXPECT_EQ(v1.ToUInt8Array(), arr);
  EXPECT_EQ(v1.Length(), 10);
  EXPECT_TRUE(v1.IsArray());

  arr.clear();
  arr.push_back(10);
  arr.push_back(20);
  v1 = arr;
  EXPECT_TRUE(v1.IsArray());
  EXPECT_EQ(v1.Length(), 2);
  std::vector<uint8_t> arr2 = v1.ToUInt8Array();

  EXPECT_EQ(arr.size(), arr2.size());
  EXPECT_EQ(arr[0], arr2[0]);
  EXPECT_EQ(arr[1], arr2[1]);

  EXPECT_EQ(std::string("[10,20]"), v1.ToString());
}

TEST(VariantTest, String) {
  EXPECT_EQ(std::string("hello world"), TestVariant_StdString("hello world"));
  EXPECT_EQ(std::string(""), TestVariant_StdString(""));
  EXPECT_EQ(std::string(), TestVariant_StdString(""));
}

#if 0
TEST(VariantTest, StringCopyAssign) {
  common::Variant v1;
  common::Variant v2;

  v1 = "asdf123";
  v2 = "asdf123";
  EXPECT_EQ(v1.ToString(), v2.ToString());

  v2 = "";
  EXPECT_NE(v1.ToString(), v2.ToString());

  v1 = v2;
  EXPECT_EQ(v1.ToString(), v2.ToString());
}
#endif

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

