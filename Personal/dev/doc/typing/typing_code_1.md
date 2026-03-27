```
#include <cstdio>. #include <iostream>. #include <vector>. #include <string>. . int main() {. int a = 0, b =
1, c = 2, d = 3;. int x = 10, y = 20, z = 30, w = 40;. long long big = 1234567890LL;. double r = 3.14159, s
= 2.71828, t = 0.0001;. . a = b + c - d * x / y % 7;. b += 11 * 22 / 33 - 44;. c -= (55 / 5) * (66 - 6);. d
*= 77 / 7 + 88 - 8;. x /= 99 / 9 + 100 - 10;. y = a * b / (c + 1) - d * 2;. z = (x * y) / (z + 1) * (w -
2);. w = (w * w) / (z + 3) * 4;. . bool flag1 = (a == b);. bool flag2 = (c != d);. bool flag3 = (x <= y) &&
(z >= w);. bool flag4 = !flag1 (flag2 && flag3);. . int mask = 0x0F;. int bits = (a & b) (c ^ d);. bits <<=
2;. bits >>= 1;. bits = ~bits;. . char s0[] = "line1. line2. line3. ";. char s1[] = "escape\. here\\0end";.
char s2[] = "values: 123. 456. 789. 000. ";. char s3[] = "A\\0B\\0C\. D\. E\\0F. ";. const char *p0 = "one.
two. three. \0four. ";. const char *p1 = "path\\dir\\file. end\\0. ";. . char c0 = '. ';. char c1 = '\0';.
char c2 = '\\';. char c3 = '0';. char c4 = '9';. . std::string str = "test\. . another_line\\0. end_string.
";. str += "1234567890. ";. str += "\. \\0\. \\0. ";. . std::string nums = "0 1 2 3 4 5 6 7 8 9. 10 11 12 13
14. ";. nums += "15 16 17 18 19. 20 21 22 23 24 25. ";. . int arr[10] = {0,1,2,3,4,5,6,7,8,9};. for (int i =
0; i < 10; i++) {. arr[i] = (arr[i] * 2) + (i * i) - (i / 2);. arr[i] *= (i + 1);. arr[i] /= (i + 1 == 0 ? 1
: i + 1);. }. . for (int i = 0; i < 50; i++) {. int v = (i * 3) / (i + 1) + (i * i) - (i / 3);. v = v * 4 /
2 * 3 / 6;. if (v % 2 == 0 && v != 0) {. v = v * v / (i + 1);. } else {. v = (v + 1) * (v + 2) / (i + 2);.
}. a += v; b -= v; c *= (v == 0 ? 1 : v / (i + 1)); d /= ((i % 5) + 1);. }. . int matrix[5][5];. for (int i
= 0; i < 5; i++) {. for (int j = 0; j < 5; j++) {. matrix[i][j] = (i * 10 + j) * (i + j + 1) / (i + 1);.
matrix[i][j] = matrix[i][j] * 2 / 3 * 4 / 5;. }. }. . std::vector<int> vnum;. for (int i = 0; i < 100; i++)
{. int val = (i * 11) / 3 + (i * 7) / 5;. val = val * 2 / (i + 1) * 3 / (i + 2);. vnum.push_back(val);. }. .
std::string lines =. "row0. ". "row1\. . ". "row2\\0. ". "row3\. \\0. ". "row4. . ". "row5\. \. \\0. ";. .
const char raw0[] = "000. 111. 222. 333. \000\000. ";. const char raw1[] = "444\. 555\. 666\\0\\0. ";. const
char raw2[] = "777 888 999 123 456 789. ";. . int scattered =. 1 + 2 * 3 / 4 - 5 +. 6 * 7 / 8 - 9 +. 10 * 11
/ 12 - 13 +. 14 * 15 / 16 - 17 +. 18 * 19 / 20 - 21;. . scattered += 22 * 23 / 24 - 25;. scattered -= 26 *
27 / 28 - 29;. scattered *= 30 * 31 / 32;. scattered /= (33 * 34 / 35);. . double fractions =. 1.0 / 2.0 +
3.0 / 4.0 - 5.0 / 6.0 +. 7.0 / 8.0 - 9.0 / 10.0 +. 11.0 / 12.0 - 13.0 / 14.0;. . fractions = fractions * 2.0
/ 3.0 * 4.0 / 5.0;. fractions = (fractions == 0.0 ? 1.0 : fractions / 2.0);. . printf("a=%d. ", a);.
printf("b=%d. ", b);. printf("c=%d. ", c);. printf("d=%d. ", d);. printf("x=%d y=%d z=%d w=%d. ", x, y, z,
w);. printf("big=%lld. ", big);. printf("r=%f s=%f t=%f. ", r, s, t);. printf("flag1=%d flag2=%d flag3=%d
flag4=%d. ", flag1, flag2, flag3, flag4);. . printf("%s%s%s%s. ", s0, s1, s2, s3);. printf("%s. %s. ", p0,
p1);. printf("chars: %c%c%c%c%c. ", c0, c1, c2, c3, c4);. printf("%s%s. ", str.c_str(), nums.c_str());.
printf("%s. ", lines.c_str());. printf("%s%s%s. ", raw0, raw1, raw2);. . printf("fractions=%f. ",
fractions);. printf("scattered=%d. ", scattered);. . return 0;. }
```
